#include "someip/api.hpp"
#include "someip/service.hpp"
#include "someip/message_router.hpp"
#include <thread>
#include <chrono>
#include <cassert>
#include <iostream>

using namespace someip;

int main() {
    // Start server endpoint
    auto server_ep = create_udp_endpoint("127.0.0.1", 4000);
    assert(server_ep);
    ServiceRegistry registry;
    registry.register_method(0x1000, 0x0001, [](const Payload& p, const Endpoint& src) -> MethodResult {
        MethodResult r;
        r.return_code = ReturnCode::E_OK;
        r.payload = {0xAA};
        return r;
    });
    auto router = create_message_router(server_ep, registry);
    server_ep->set_callback([&router](const SomeIpMessage& msg, const Endpoint& src, const Endpoint& dst, TransportProtocol proto){
        router->route(msg, src, dst, proto);
    });

    // Start client endpoint
    auto client_ep = create_udp_endpoint("127.0.0.1", 4002);
    assert(client_ep);
    bool received = false;
    client_ep->set_callback([&](const SomeIpMessage& msg, const Endpoint&, const Endpoint&, TransportProtocol){
        if (msg.header.message_type == static_cast<uint8_t>(MessageType::RESPONSE)) {
            if (!msg.payload.empty() && msg.payload[0] == 0xAA) received = true;
        }
    });

    // Send request
    SomeIpHeader h;
    h.service_id = 0x1000;
    h.method_id  = 0x0001;
    h.length = SomeIpHeader::MIN_LENGTH + 0;
    h.client_id = 0x1;
    h.session_id = 0x1;
    h.protocol_version = 1;
    h.interface_version = 1;
    h.message_type = static_cast<uint8_t>(MessageType::REQUEST);
    h.return_code = 0;
    SomeIpMessage req{h, {}};
    client_ep->send_to(req.serialize(), std::make_pair(std::string("127.0.0.1"), (uint16_t)4000));

    // wait a bit
    std::this_thread::sleep_for(std::chrono::seconds(1));
    assert(received);
    std::cout << "test_endtoend passed\n";
    return 0;
}