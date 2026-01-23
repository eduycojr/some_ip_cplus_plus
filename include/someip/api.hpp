#ifndef SOMEIP_API_HPP
#define SOMEIP_API_HPP

#include "transport.hpp"
#include "service.hpp"
#include "service_discovery.hpp"
#include "message_router.hpp"
#include <memory>

namespace someip {

// Simple API helpers

// Create and start a UDP endpoint bound to ip:port. If multicast_addr is non-empty, join it.
std::shared_ptr<UdpEndpoint> create_udp_endpoint(const std::string& bind_ip, uint16_t bind_port, const std::string& multicast_addr = "");

// Create a service discovery object (uses a multicast UDP endpoint internally)
std::unique_ptr<ServiceDiscovery> create_service_discovery(const std::string& multicast = DEFAULT_SD_MULTICAST, uint16_t port = DEFAULT_SD_PORT);

// Create a message router attached to a UDP endpoint and a service registry
std::unique_ptr<MessageRouter> create_message_router(std::shared_ptr<UdpEndpoint> endpoint, ServiceRegistry& registry);

} // namespace someip

#endif // SOMEIP_API_HPP