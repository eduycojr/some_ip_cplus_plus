#ifndef SOMEIP_MESSAGE_HPP
#define SOMEIP_MESSAGE_HPP

#include "someip_header.hpp"

namespace someip {

struct SomeIpMessage {
    SomeIpHeader header;
    Payload payload;

    Payload serialize() const {
        Payload out = header.serialize();
        out.insert(out.end(), payload.begin(), payload.end());
        return out;
    }

    static SomeIpMessage deserialize(const Payload& data) {
        if (data.size() < SomeIpHeader::SIZE) throw std::runtime_error("message: too small");
        Payload header_bytes(data.begin(), data.begin() + SomeIpHeader::SIZE);
        SomeIpHeader h = SomeIpHeader::deserialize(header_bytes);

        size_t p_len = h.length - SomeIpHeader::MIN_LENGTH;
        if (data.size() < SomeIpHeader::SIZE + p_len) throw std::runtime_error("message: payload short");

        SomeIpMessage msg;
        msg.header = h;
        msg.payload = Payload(data.begin() + SomeIpHeader::SIZE, data.begin() + SomeIpHeader::SIZE + p_len);
        return msg;
    }
};

} // namespace someip

#endif // SOMEIP_MESSAGE_HPP