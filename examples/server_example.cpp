#include "someip/api.hpp"
#include "someip/service.hpp"
#include "someip/message_router.hpp"
#include <thread>
#include <chrono>
#include <iostream>

using namespace someip;

int main() {
    // Create a UDP endpoint bound to 127.0.0.1:3000 and join SD multicast
    auto endpoint = create_udp_endpoint("127.0.0.1", 3000, DEFAULT_SD_MULTICAST);
    if (!endpoint) {
        std::cerr << "Failed to create UDP endpoint\n";
        return 1;
    }

    ServiceRegistry registry;

    // Register a simple echo method: returns payload back with E_OK
    registry.register_method(0x1234, 0x0123, [](const Payload& p, const Endpoint& src) -> MethodResult {
        log_info("Server: method called, echoing payload");
        return { ReturnCode::E_OK, p };
    });

    // Message router that will send responses using the same UDP endpoint
    auto router = create_message_router(endpoint, registry);

    // Attach transport callback to router
    endpoint->set_callback([&router](const SomeIpMessage& msg, const Endpoint& src, const Endpoint& dst, TransportProtocol proto){
        log_debug("Server received message: " + msg.header.to_string());
        router->route(msg, src, dst, proto);
    });

    // Offer service via SD
    auto sd = create_service_discovery();
    if (sd) {
        SdOffer offer;
        offer.service_id = 0x1234;
        offer.instance_id = 0x5678;
        offer.ip = "127.0.0.1";
        offer.port = 3000;
        offer.ttl = 5;
        sd->offer_service(offer);
        sd->set_found_callback([](const SdOffer& o){
            log_info("Server SD: found service " + std::to_string(o.service_id));
        });
    }

    log_info("Server running. Press Ctrl+C to exit.");
    // Keep running
    while (true) std::this_thread::sleep_for(std::chrono::seconds(1));

    return 0;
}