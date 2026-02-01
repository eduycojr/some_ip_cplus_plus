
#include <chrono>
#include <thread>
#include <iostream>
#include <string>
#include <memory>  // For std::shared_ptr
#include "someip/api.hpp"            // For create_udp_endpoint
#include "someip/message_router.hpp" // For SomeIpHeader, MessageRouter
#include "someip/service.hpp"        // For SOME/IP services

using namespace someip;

// Sends brake press OR release command to the server
bool send_brake_request(std::shared_ptr<UdpEndpoint> client, const std::string& server_ip, uint16_t server_port, bool press) {
    SomeIpHeader h;
    h.service_id = 0x1300;            // Brake Control Service ID
    h.method_id = press ? 0x0010 : 0x0020;  // Press (0x0010) or Release (0x0020)
    h.client_id = 0x01;               // Client ID
    h.session_id = 0x01;              // Session ID
    h.length = 8;                     // Header only (payload not needed)
    h.protocol_version = 1;
    h.interface_version = 1;
    h.message_type = static_cast<uint8_t>(MessageType::REQUEST);

    SomeIpMessage req{h, {}};
    auto server_endpoint = std::make_pair(server_ip, server_port);

    return client->send_to(req.serialize(), server_endpoint);
}

// Sends brake status query command to the server
bool request_brake_status(std::shared_ptr<UdpEndpoint> client, const std::string& server_ip, uint16_t server_port) {
    SomeIpHeader h;
    h.service_id = 0x1300;           // Brake Control Service ID
    h.method_id = 0x0030;            // Brake status method ID
    h.client_id = 0x01;              // Client ID
    h.session_id = 0x01;             // Session ID
    h.length = 8;                    // Header only (payload not needed)
    h.protocol_version = 1;
    h.interface_version = 1;
    h.message_type = static_cast<uint8_t>(MessageType::REQUEST);

    SomeIpMessage req{h, {}};
    auto server_endpoint = std::make_pair(server_ip, server_port);

    return client->send_to(req.serialize(), server_endpoint);
}

int main() {
    const std::string server_ip = "127.0.0.1";
    const uint16_t server_port = 3000;
    const uint16_t client_port = 3002;

    auto client = create_udp_endpoint("127.0.0.1", client_port);
    if (!client) {
        std::cerr << "[ERROR] Failed to create UDP client endpoint on port " << client_port << ".\n";
        return 1;
    }
    std::cout << "[INFO] Client listening on port " << client_port << ".\n";

    bool running = true;

    while (running) {
        std::string input;
        std::cout << "\nEnter brake command (press/release/status/exit): ";
        std::cin >> input;

        if (input == "exit") {
            running = false;
        } else if (input == "press") {
            bool ok = send_brake_request(client, server_ip, server_port, true);
            if (ok) {
                std::cout << "[INFO] Sent brake press request.\n";
            } else {
                std::cerr << "[ERROR] Failed to send brake press request.\n";
            }
        } else if (input == "release") {
            bool ok = send_brake_request(client, server_ip, server_port, false);
            if (ok) {
                std::cout << "[INFO] Sent brake release request.\n";
            } else {
                std::cerr << "[ERROR] Failed to send brake release request.\n";
            }
        } else if (input == "status") {
            bool ok = request_brake_status(client, server_ip, server_port);
            if (ok) {
                std::cout << "[INFO] Sent brake status request.\n";
            } else {
                std::cerr << "[ERROR] Failed to send brake status request.\n";
            }
        } else {
            std::cerr << "[ERROR] Unknown command. Use 'press', 'release', 'status', or 'exit'.\n";
        }

        std::this_thread::sleep_for(std::chrono::seconds(1)); // Optional delay between requests
    }

    std::cout << "[INFO] Client application terminated.\n";
    return 0;
}