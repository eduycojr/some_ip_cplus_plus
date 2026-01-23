#ifndef SOMEIP_HEADER_HPP
#define SOMEIP_HEADER_HPP

#include "types.hpp"
#include "serialization.hpp"
#include <string>

namespace someip {

// SOME/IP header: 16 bytes
struct SomeIpHeader {
    Uint16 service_id;
    Uint16 method_id;
    Uint32 length; // length of payload + 8
    Uint16 client_id;
    Uint16 session_id;
    Uint8 protocol_version;
    Uint8 interface_version;
    Uint8 message_type;
    Uint8 return_code;

    static constexpr size_t SIZE = 16;
    static constexpr Uint32 MIN_LENGTH = 8;

    Payload serialize() const {
        SerializationBuffer buf;
        buf.write_uint16(service_id);
        buf.write_uint16(method_id);
        buf.write_uint32(length);
        buf.write_uint16(client_id);
        buf.write_uint16(session_id);
        buf.write_uint8(protocol_version);
        buf.write_uint8(interface_version);
        buf.write_uint8(message_type);
        buf.write_uint8(return_code);
        return std::move(buf.buf);
    }

    static SomeIpHeader deserialize(const Payload& data) {
        if (data.size() < SIZE) throw std::runtime_error("header: too small");
        DeserializationBuffer db(data);
        SomeIpHeader h;
        h.service_id = db.read_uint16();
        h.method_id  = db.read_uint16();
        h.length     = db.read_uint32();
        h.client_id  = db.read_uint16();
        h.session_id = db.read_uint16();
        h.protocol_version = db.read_uint8();
        h.interface_version = db.read_uint8();
        h.message_type = db.read_uint8();
        h.return_code = db.read_uint8();
        if (h.length < MIN_LENGTH) throw std::runtime_error("header: length < 8");
        return h;
    }

    std::string to_string() const {
        char tmp[200];
        snprintf(tmp, sizeof(tmp),
                 "SomeIpHeader{svc=0x%04X, mth=0x%04X, len=%u, client=0x%04X, sess=0x%04X, proto=%u, iface=%u, type=0x%02X, rc=0x%02X}",
                 service_id, method_id, length, client_id, session_id, protocol_version, interface_version, message_type, return_code);
        return std::string(tmp);
    }
};

} // namespace someip

#endif // SOMEIP_HEADER_HPP