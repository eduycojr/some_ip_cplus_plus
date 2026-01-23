#ifndef SOMEIP_MESSAGE_ROUTER_HPP
#define SOMEIP_MESSAGE_ROUTER_HPP

#include "types.hpp"
#include "service.hpp"
#include "someip_message.hpp"
#include "transport.hpp"
#include <memory>

namespace someip {

class MessageRouter {
public:
    MessageRouter(std::shared_ptr<UdpEndpoint> endpoint, ServiceRegistry& registry);

    // Route an incoming message (called by transport callback)
    void route(const SomeIpMessage& msg, const Endpoint& src, const Endpoint& dst, TransportProtocol proto);

    // Helper to construct and send a response
    void send_response(const SomeIpMessage& request, const MethodResult& result, const Endpoint& dest);

    // Helper to send errors
    void send_error(const SomeIpMessage& request, ReturnCode rc, const Endpoint& dest);

private:
    std::shared_ptr<UdpEndpoint> endpoint_;
    ServiceRegistry& registry_;
};

} // namespace someip

#endif // SOMEIP_MESSAGE_ROUTER_HPP