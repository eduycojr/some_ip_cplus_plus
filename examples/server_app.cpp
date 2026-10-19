#include "someip/api.hpp"
#include "someip/service.hpp"
#include "someip/message_router.hpp" 
#include <optional>
#include <iostream>

using namespace someip;

// Tail light demo state:
//   false   -> OK
//   true    -> FAULT
//   nullopt -> indeterminate (error path, not a third status)
static std::optional<bool> tail_light_state = false;

// Handler for pressing the brake
MethodResult press_brake(const Payload&, const Endpoint&) {
    std::cout << "[INFO] Server: Press Brake request received.\n";
    MethodResult result;
    result.return_code = ReturnCode::E_OK;  // Success
    result.payload = {1}; // Brake pressed
    return result;
}

// Handler for releasing the brake
MethodResult release_brake(const Payload&, const Endpoint&) {
    std::cout << "[INFO] Server: Release Brake request received.\n";
    MethodResult result;
    result.return_code = ReturnCode::E_OK;  // Success
    result.payload = {0}; // Brake released
    return result;
}

// Handler for querying brake status
MethodResult brake_status(const Payload&, const Endpoint&) {
    std::cout << "[INFO] Server: Brake Status request received.\n";
    MethodResult result;
    result.return_code = ReturnCode::E_OK;  // Success
    result.payload = {1}; // Brake pressed
    return result;
}

// Handler for querying tail light fault status
MethodResult tail_light_status(const Payload&, const Endpoint&) {
    std::cout << "[INFO] Server: Tail Light Status request received.\n";

    MethodResult result;

    if (!tail_light_state.has_value()) {
        result.return_code = ReturnCode::E_NOT_OK;
        result.payload = {};
        return result;
    }

    if (tail_light_state.value() == false) {
        // OK
        result.return_code = ReturnCode::E_OK;
        result.payload = {0x00, 0x00};
        return result;
    }

    // FAULT
    result.return_code = ReturnCode::E_OK;
    result.payload = {0x01, 0x01};
    return result;
}

int main() {
    const std::string server_ip = "127.0.0.1";
    const uint16_t server_port = 3000;

    auto server = create_udp_endpoint(server_ip, server_port);
    if (!server) {
        std::cerr << "[ERROR] Failed to create UDP server endpoint on port " << server_port << ".\n";
        return 1;
    }
    std::cout << "[INFO] Server listening on port " << server_port << ".\n";

    ServiceRegistry registry;

    registry.register_method(0x1300, 0x0010, press_brake);  // Press Brake
    registry.register_method(0x1300, 0x0020, release_brake); // Release Brake
    registry.register_method(0x1300, 0x0030, brake_status);  // Brake Status
    registry.register_method(0x1310, 0x0010, tail_light_status); // Tail Light Status

    auto router = create_message_router(server, registry);

    server->set_callback([&](const SomeIpMessage& msg, const Endpoint& src, const Endpoint& dest, TransportProtocol proto) {
        router->route(msg, src, dest, proto);
    });

    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(2)); // Simulating idle operation
    }

    return 0;
}
