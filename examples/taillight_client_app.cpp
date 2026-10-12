#include "someip/transport.hpp"
#include "someip/types.hpp"
#include <iostream>
#include <thread>

using namespace someip;

int main() {
    std::cout << "Taillight client starting" << std::endl;
    UdpEndpoint ep("127.0.0.1", 0); // bind ephemeral port
    if (!ep.start()) {
        std::cerr << "Failed to start endpoint" << std::endl;
        return 1;
    }

    std::string msg = "TAILLIGHT_PULSE";
    Payload p(msg.begin(), msg.end());
    Endpoint dest(std::string("127.0.0.1"), 34000);

    bool ok = ep.send_to(p, dest);
    if (ok) std::cout << "Taillight client sent payload to server" << std::endl;
    else std::cerr << "Taillight client failed to send payload" << std::endl;

    ep.stop();
    return ok ? 0 : 1;
}
