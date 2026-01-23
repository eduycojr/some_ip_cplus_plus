#include "someip/message_router.hpp"
#include "someip/serialization.hpp"

namespace someip {

MessageRouter::MessageRouter(std::shared_ptr<UdpEndpoint> endpoint, ServiceRegistry& registry)
    : endpoint_(std::move(endpoint)), registry_(registry) {}

void MessageRouter::route(const SomeIpMessage& msg, const Endpoint& src, const Endpoint& dst, TransportProtocol proto) {
    // Only handle REQUEST types for this minimal implementation
    if (msg.header.message_type == static_cast<uint8_t>(MessageType::REQUEST)) {
        auto handler_opt = registry_.find_handler(msg.header.service_id, msg.header.method_id);
        if (!handler_opt) {
            send_error(msg, ReturnCode::E_UNKNOWN, src);
            return;
        }
        MethodResult res = (*handler_opt)(msg.payload, src);
        send_response(msg, res, src);
    } else {
        // ignore other types for brevity
    }
}

void MessageRouter::send_response(const SomeIpMessage& request, const MethodResult& result, const Endpoint& dest) {
    SomeIpHeader h;
    h.service_id = request.header.service_id;
    h.method_id  = request.header.method_id;
    h.client_id  = request.header.client_id;
    h.session_id = request.header.session_id;
    h.protocol_version = request.header.protocol_version;
    h.interface_version = request.header.interface_version;
    h.message_type = static_cast<uint8_t>(MessageType::RESPONSE);
    h.return_code = static_cast<uint8_t>(result.return_code);
    h.length = (Uint32)(result.payload.size() + SomeIpHeader::MIN_LENGTH);
    SomeIpMessage resp{h, result.payload};
    Payload out = resp.serialize();
    if (endpoint_) endpoint_->send_to(out, dest);
}

void MessageRouter::send_error(const SomeIpMessage& request, ReturnCode rc, const Endpoint& dest) {
    SomeIpHeader h;
    h.service_id = request.header.service_id;
    h.method_id  = request.header.method_id;
    h.client_id  = request.header.client_id;
    h.session_id = request.header.session_id;
    h.protocol_version = request.header.protocol_version;
    h.interface_version = request.header.interface_version;
    h.message_type = static_cast<uint8_t>(MessageType::ERROR);
    h.return_code = static_cast<uint8_t>(rc);
    h.length = SomeIpHeader::MIN_LENGTH;
    SomeIpMessage err{h, {}};
    Payload out = err.serialize();
    if (endpoint_) endpoint_->send_to(out, dest);
}

} // namespace someip