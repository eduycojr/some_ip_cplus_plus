#include "someip/api.hpp"
#include "someip/service.hpp"
#include "someip/message_router.hpp"
#include <thread>
#include <chrono>
#include <iostream>
#include <csignal>

using namespace someip;

static volatile bool running = true;

void signal_handler(int signum) {
    std::cout << "\n[INFO] Caught signal " << signum << ", shutting down...\n";
    running = false;
}

int main() {
    std::cout << "[INFO] Starting SOME/IP Server...\n";
    std::cout.flush();

    // Setup signal handler for clean shutdown
    signal(SIGINT, signal_handler);

    // Create a UDP endpoint bound to 127.0.0.1:3000
    std::cout << "[INFO] Creating UDP endpoint on 127.0.0.1:3000...\n";
    std::cout.flush();
    
    auto endpoint = create_udp_endpoint("127.0.0.1", 3000);
    if (!endpoint) {
        std::cerr << "[ERROR] Failed to create UDP endpoint\n";
        std::cout << "Press Enter to exit...\n";
        std::cin.get();
        return 1;
    }
    std::cout << "[INFO] UDP endpoint created successfully.\n";
    std::cout.flush();

    ServiceRegistry registry;

    // Register a simple echo method: returns payload back with E_OK
    registry.register_method(0x1234, 0x0123, [](const Payload& p, const Endpoint& src) -> MethodResult {
        std::cout << "[INFO] Server: method 0x0123 called, payload size=" << p.size() << ", echoing back\n";
        std::cout.flush();
        return { ReturnCode::E_OK, p };
    });
    std::cout << "[INFO] Registered method 0x0123 for service 0x1234\n";
    std::cout.flush();

    // Message router that will send responses using the same UDP endpoint
    auto router = create_message_router(endpoint, registry);

    // Attach transport callback to router
    endpoint->set_callback([&router](const SomeIpMessage& msg, const Endpoint& src, const Endpoint& dst, TransportProtocol proto){
        std::cout << "[DEBUG] Server received message: " << msg.header.to_string() << "\n";
        std::cout.flush();
        router->route(msg, src, dst, proto);
    });

    std::cout << "[INFO] Server running on 127.0.0.1:3000. Press Ctrl+C to exit.\n";
    std::cout.flush();

    // Keep running
    while (running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::cout << "[INFO] Server stopped.\n";
    return 0;
}