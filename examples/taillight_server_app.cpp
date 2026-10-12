#include "someip/transport.hpp"
#include "someip/types.hpp"
#include <iostream>
#include <thread>
#include <chrono>

using namespace someip;

int main() {
    std::cout << "Taillight server starting on 127.0.0.1:34000" << std::endl;
    auto ep = std::make_shared<UdpEndpoint>("127.0.0.1", 34000);
    ep->set_callback([](const SomeIpMessage& msg, const Endpoint& src, const Endpoint& dst, TransportProtocol proto) {
        (void)dst; (void)proto;
        std::string from = std::get<0>(src) + ":" + std::to_string(std::get<1>(src));
        log_info(std::string("Taillight server received message from ") + from);
    });
    if (!ep->start()) {
        std::cerr << "Failed to start endpoint" << std::endl;
        return 1;
    }
    // Run server for 60 seconds for demo purposes
    std::this_thread::sleep_for(std::chrono::seconds(60));
    ep->stop();
    std::cout << "Taillight server stopping" << std::endl;
    return 0;
}
