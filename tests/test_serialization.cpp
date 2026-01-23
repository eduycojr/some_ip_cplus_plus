#include "someip/someip_header.hpp"
#include <cassert>
#include <iostream>

using namespace someip;

int main() {
    SomeIpHeader h;
    h.service_id = 0xABCD;
    h.method_id = 0x1234;
    h.length = SomeIpHeader::MIN_LENGTH + 5;
    h.client_id = 0x0010;
    h.session_id = 0x0020;
    h.protocol_version = 1;
    h.interface_version = 1;
    h.message_type = static_cast<uint8_t>(MessageType::REQUEST);
    h.return_code = 0;

    Payload serialized = h.serialize();
    SomeIpHeader parsed = SomeIpHeader::deserialize(serialized);
    assert(parsed.service_id == h.service_id);
    assert(parsed.method_id == h.method_id);
    assert(parsed.length == h.length);
    std::cout << "test_serialization passed\n";
    return 0;
}