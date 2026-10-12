#include "someip/transport.hpp"
#include "someip/types.hpp"
#include "someip/someip_message.hpp"
#include "someip/someip_header.hpp"
#include <iostream>
#include <thread>

using namespace someip;

int main() {
    std::cout << "Taillight client starting (SOME/IP framed)" << std::endl;
    UdpEndpoint ep("127.0.0.1", 0); // bind ephemeral port
    if (!ep.start()) {
        std::cerr << "Failed to start endpoint" << std::endl;
        return 1;
    }

    // Prepare a valid SOME/IP request message for the tail light service.
    SomeIpHeader h;
    h.service_id = 0x1234;
    h.method_id = 0x0001;
    std::string payload_str = "TAILLIGHT_PULSE";
    Payload payload(payload_str.begin(), payload_str.end());
    h.length = SomeIpHeader::MIN_LENGTH + static_cast<Uint32>(payload.size());
    h.client_id = 0x0001;
    h.session_id = 0x0001;
    h.protocol_version = 1;
    h.interface_version = 1;
    h.message_type = static_cast<Uint8>(MessageType::REQUEST);
    h.return_code = static_cast<Uint8>(ReturnCode::E_OK);

    SomeIpMessage msg{h, payload};
    Payload out = msg.serialize();

    Endpoint dest(std::string("127.0.0.1"), 34000);

    bool ok = ep.send_to(out, dest);
    if (ok) std::cout << "Taillight client sent SOME/IP payload to server" << std::endl;
    else std::cerr << "Taillight client failed to send SOME/IP payload" << std::endl;

    ep.stop();
    return ok ? 0 : 1;
}
