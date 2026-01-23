#include "someip/api.hpp"
#include "someip/transport.hpp"
#include <memory>

namespace someip {

std::shared_ptr<UdpEndpoint> create_udp_endpoint(const std::string& bind_ip, uint16_t bind_port, const std::string& multicast_addr) {
    auto ep = std::make_shared<UdpEndpoint>(bind_ip, bind_port);
    if (!ep->start()) return nullptr;
    if (!multicast_addr.empty()) ep->join_multicast(multicast_addr);
    return ep;
}

std::unique_ptr<ServiceDiscovery> create_service_discovery(const std::string& multicast, uint16_t port) {
    auto sd = std::make_unique<ServiceDiscovery>(multicast, port);
    if (!sd->start()) return nullptr;
    return sd;
}

std::unique_ptr<MessageRouter> create_message_router(std::shared_ptr<UdpEndpoint> endpoint, ServiceRegistry& registry) {
    return std::make_unique<MessageRouter>(endpoint, registry);
}

} // namespace someip