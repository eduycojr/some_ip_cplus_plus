#include "someip/api.hpp"
#include "someip/someip_header.hpp"
#include "someip/someip_message.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>

using namespace someip;

int main() {
    std::cout << "[INFO] Starting SOME/IP Client...\n";
    std::cout.flush();

    // Create UDP endpoint on a different port to receive responses
    std::cout << "[INFO] Creating UDP endpoint on 127.0.0.1:3002...\n";
    std::cout.flush();
    
    auto ep = create_udp_endpoint("127.0.0.1", 3002);
    if (!ep) {
        std::cerr << "[ERROR] Failed to create UDP endpoint\n";
        std::cout << "Press Enter to exit...\n";
        std::cin.get();
        return 1;
    }
    std::cout << "[INFO] UDP endpoint created successfully.\n";
    std::cout.flush();

    std::atomic<bool> response_received{false};

    // Receive callback - set this BEFORE sending
    ep->set_callback([&](const SomeIpMessage& msg, const Endpoint& src, const Endpoint& dst, TransportProtocol proto){
        std::cout << "[INFO] Client received message: " << msg.header.to_string() << "\n";
        std::cout.flush();
        if (msg.header.message_type == static_cast<uint8_t>(MessageType::RESPONSE)) {
            std::cout << "[INFO] Response payload size=" << msg.payload.size() << "\n";
            std::cout << "[INFO] Response payload: ";
            for (auto b : msg.payload) {
                printf("0x%02X ", b);
            }
            std::cout << "\n";
            std::cout.flush();
            response_received = true;
        } else if (msg.header.message_type == static_cast<uint8_t>(MessageType::ERR)) {
            std::cout << "[ERROR] Server returned error, return_code=" 
                      << (int)msg.header.return_code << "\n";
            std::cout.flush();
            response_received = true;
        }
    });

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

    std::cout << "[INFO] Sending request to server 127.0.0.1:3000...\n";
    std::cout << "[INFO] Request: " << h.to_string() << "\n";
    std::cout << "[INFO] Payload: ";
    for (auto b : payload) {
        printf("0x%02X ", b);
    }
    std::cout << "\n";
    std::cout.flush();

    Endpoint server_ep("127.0.0.1", 3000);
    bool sent = ep->send_to(out, server_ep);
    
    if (sent) {
        std::cout << "[INFO] Request sent successfully, waiting for response...\n";
    } else {
        std::cout << "[ERROR] Failed to send request!\n";
    }
    std::cout.flush();

    // Wait for response (max 5 seconds)
    int wait_count = 0;
    while (!response_received && wait_count < 50) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        wait_count++;
    }

    if (!response_received) {
        std::cout << "[WARNING] No response received within timeout!\n";
    }

    std::cout << "\n[INFO] Client finished.\n";
    std::cout << "Press Enter to exit...\n";
    std::cout.flush();
    std::cin.get();
    
    return 0;
}