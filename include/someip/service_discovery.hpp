#ifndef SOMEIP_SERVICE_DISCOVERY_HPP
#define SOMEIP_SERVICE_DISCOVERY_HPP

#include "types.hpp"
#include "someip_message.hpp"
#include "transport.hpp"
#include <map>
#include <set>
#include <mutex>
#include <thread>
#include <atomic>

namespace someip {

// Minimal SD entry representation (very small subset)
struct SdOffer {
    ServiceId service_id;
    InstanceId instance_id;
    std::string ip;
    uint16_t port;
    uint32_t ttl;
};

class ServiceDiscovery {
public:
    ServiceDiscovery(const std::string& multicast = DEFAULT_SD_MULTICAST, uint16_t port = DEFAULT_SD_PORT);
    ~ServiceDiscovery();

    bool start();
    void stop();

    // Offer a service (sends initial offer and then periodically)
    void offer_service(const SdOffer& offer);

    // Stop offering
    void stop_offer(ServiceId svc, InstanceId inst);

    // Callback when we discover a service on the network
    using FoundCallback = std::function<void(const SdOffer&)>;
    void set_found_callback(FoundCallback cb) { found_cb_ = std::move(cb); }

private:
    void periodic_offer_loop();
    void handle_incoming(const SomeIpMessage& msg, const Endpoint& src, const Endpoint& dst, TransportProtocol proto);

    std::string mcast_addr_;
    uint16_t mcast_port_;
    std::unique_ptr<UdpEndpoint> mcast_endpoint_;
    std::map<std::pair<ServiceId, InstanceId>, SdOffer> offered_;
    std::map<std::pair<std::string, uint16_t>, SdOffer> found_;
    FoundCallback found_cb_;
    std::mutex mutex_;
    std::thread offer_thread_;
    std::atomic<bool> running_{false};
};

} // namespace someip

#endif // SOMEIP_SERVICE_DISCOVERY_HPP