#include "someip/api.hpp"
#include "someip/someip_header.hpp"
#include "someip/someip_message.hpp"
#include <iostream>
#include <thread>
#include <chrono>

using namespace someip;

int main() {
    // Create UDP endpoint on a different port to receive responses
    auto ep = create_udp_endpoint("127.0.0.1", 3002);
    if (!ep) {
        std::cerr << "Failed to create UDP endpoint\n";
        return 1;
    }

    // Construct a simple request to server 127.0.0.1:3000 service 0x1234 method 0x0123
    SomeIpHeader h;
    h.service_id = 0x1234;
    h.method_id = 0x0123;
    Payload payload = { 0x11, 0x22, 0x33 };
    h.length = (Uint32)(payload.size() + SomeIpHeader::MIN_LENGTH);
    h.client_id = 0x0001;
    h.session_id = 0x0001;
    h.protocol_version = 1;
    h.interface_version = 1;
    h.message_type = static_cast<uint8_t>(MessageType::REQUEST);
    h.return_code = 0;

    SomeIpMessage req{h, payload};
    Payload out = req.serialize();

    Endpoint server_ep("127.0.0.1", 3000);
    ep->send_to(out, server_ep);

    // Receive callback
    ep->set_callback([&](const SomeIpMessage& msg, const Endpoint& src, const Endpoint& dst, TransportProtocol proto){
        log_info("Client received message: " + msg.header.to_string());
        if (msg.header.message_type == static_cast<uint8_t>(MessageType::RESPONSE)) {
            std::cout << "Client: response payload size=" << msg.payload.size() << "\n";
        }
    });

    // wait to receive
    std::this_thread::sleep_for(std::chrono::seconds(3));
    return 0;
}