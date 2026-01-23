#include "someip/service_discovery.hpp"
#include "someip/api.hpp"
#include <chrono>
#include <thread>

namespace someip {

ServiceDiscovery::ServiceDiscovery(const std::string& multicast, uint16_t port)
    : mcast_addr_(multicast), mcast_port_(port) {}

ServiceDiscovery::~ServiceDiscovery() {
    stop();
}

bool ServiceDiscovery::start() {
    std::lock_guard<std::mutex> lk(mutex_);
    if (running_) return true;
    mcast_endpoint_.reset(new UdpEndpoint("0.0.0.0", mcast_port_));
    mcast_endpoint_->set_callback([this](const SomeIpMessage& msg, const Endpoint& src, const Endpoint& dst, TransportProtocol proto){
        this->handle_incoming(msg, src, dst, proto);
    });
    if (!mcast_endpoint_->start()) return false;
    if (!mcast_endpoint_->join_multicast(mcast_addr_)) {
        log_error("Failed to join multicast group");
    }
    running_ = true;
    offer_thread_ = std::thread(&ServiceDiscovery::periodic_offer_loop, this);
    return true;
}

void ServiceDiscovery::stop() {
    {
        std::lock_guard<std::mutex> lk(mutex_);
        if (!running_) return;
        running_ = false;
    }
    if (mcast_endpoint_) mcast_endpoint_->stop();
    if (offer_thread_.joinable()) offer_thread_.join();
}

void ServiceDiscovery::offer_service(const SdOffer& offer) {
    std::lock_guard<std::mutex> lk(mutex_);
    offered_[std::make_pair(offer.service_id, offer.instance_id)] = offer;
    // send immediate offer: create VERY simple SD-like SOME/IP message (not spec-complete)
    SomeIpHeader h;
    h.service_id = 0xFFFF; // SD service
    h.method_id = 0x8100;
    Payload body;
    // For simplicity: body = service_id(2) instance(2) ip_len(1) ip bytes port(2) ttl(4)
    SerializationBuffer sb;
    sb.write_uint16(offer.service_id);
    sb.write_uint16(offer.instance_id);
    sb.write_uint8((Uint8)offer.ip.size());
    sb.write_bytes(reinterpret_cast<const uint8_t*>(offer.ip.data()), offer.ip.size());
    sb.write_uint16(offer.port);
    sb.write_uint32(offer.ttl);
    body = std::move(sb.buf);
    h.length = (Uint32)(body.size() + SomeIpHeader::MIN_LENGTH);
    h.client_id = 0;
    h.session_id = 1;
    h.protocol_version = 1;
    h.interface_version = 1;
    h.message_type = static_cast<uint8_t>(MessageType::NOTIFICATION);
    h.return_code = 0;
    SomeIpMessage msg{h, body};
    if (mcast_endpoint_) {
        Payload out = msg.serialize();
        mcast_endpoint_->send_to(out, std::make_pair(mcast_addr_, mcast_port_));
    }
}

void ServiceDiscovery::stop_offer(ServiceId svc, InstanceId inst) {
    std::lock_guard<std::mutex> lk(mutex_);
    offered_.erase(std::make_pair(svc, inst));
    // TODO: send stop offer SD entry
}

void ServiceDiscovery::handle_incoming(const SomeIpMessage& msg, const Endpoint& src, const Endpoint& dst, TransportProtocol proto) {
    // minimal parsing for our simple SD format
    if (msg.header.service_id != 0xFFFF) return;
    try {
        DeserializationBuffer db(msg.payload);
        SdOffer o;
        o.service_id = db.read_uint16();
        o.instance_id = db.read_uint16();
        Uint8 ip_len = db.read_uint8();
        auto ip_bytes = db.read_bytes(ip_len);
        o.ip = std::string(ip_bytes.begin(), ip_bytes.end());
        o.port = db.read_uint16();
        o.ttl  = db.read_uint32();

        {
            std::lock_guard<std::mutex> lk(mutex_);
            found_[std::make_pair(o.ip, o.port)] = o;
        }
        if (found_cb_) found_cb_(o);
    } catch (...) {
        // ignore malformed SD
    }
}

void ServiceDiscovery::periodic_offer_loop() {
    while (running_) {
        {
            std::lock_guard<std::mutex> lk(mutex_);
            for (auto& kv : offered_) {
                offer_service(kv.second);
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}

} // namespace someip