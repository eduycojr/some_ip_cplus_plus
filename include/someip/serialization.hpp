#ifndef SOMEIP_SERIALIZATION_HPP
#define SOMEIP_SERIALIZATION_HPP

#include "types.hpp"
#include <cstring>
#include <stdexcept>

namespace someip {
namespace endian {

// Big-endian conversions (explicit)
inline Uint16 hton16(Uint16 v) {
    return (Uint16)((v << 8) | (v >> 8));
}
inline Uint16 ntoh16(Uint16 v) { return hton16(v); }

inline Uint32 hton32(Uint32 v) {
    return ((v & 0x000000FFU) << 24) |
           ((v & 0x0000FF00U) << 8)  |
           ((v & 0x00FF0000U) >> 8)  |
           ((v & 0xFF000000U) >> 24);
}
inline Uint32 ntoh32(Uint32 v) { return hton32(v); }

inline Uint64 hton64(Uint64 v) {
    return ((Uint64)hton32((Uint32)(v & 0xFFFFFFFFULL)) << 32) |
           (Uint64)hton32((Uint32)(v >> 32));
}
inline Uint64 ntoh64(Uint64 v) { return hton64(v); }

} // namespace endian

// Simple write buffer
class SerializationBuffer {
public:
    Payload buf;
    void write_uint8(Uint8 v) { buf.push_back(v); }
    void write_uint16(Uint16 v) {
        Uint16 w = endian::hton16(v);
        uint8_t *p = reinterpret_cast<uint8_t*>(&w);
        buf.insert(buf.end(), p, p + sizeof(w));
    }
    void write_uint32(Uint32 v) {
        Uint32 w = endian::hton32(v);
        uint8_t *p = reinterpret_cast<uint8_t*>(&w);
        buf.insert(buf.end(), p, p + sizeof(w));
    }
    void write_bytes(const Payload& p) { buf.insert(buf.end(), p.begin(), p.end()); }
    void write_bytes(const uint8_t* p, size_t n) { buf.insert(buf.end(), p, p + n); }
};

// Simple read buffer
class DeserializationBuffer {
public:
    const Payload& buf;
    size_t pos;
    DeserializationBuffer(const Payload& b) : buf(b), pos(0) {}
    void ensure(size_t n) {
        if (pos + n > buf.size()) throw std::runtime_error("buffer underflow");
    }
    Uint8 read_uint8() {
        ensure(1);
        return buf[pos++];
    }
    Uint16 read_uint16() {
        ensure(2);
        Uint16 v;
        std::memcpy(&v, buf.data() + pos, 2);
        pos += 2;
        return endian::ntoh16(v);
    }
    Uint32 read_uint32() {
        ensure(4);
        Uint32 v;
        std::memcpy(&v, buf.data() + pos, 4);
        pos += 4;
        return endian::ntoh32(v);
    }
    Payload read_bytes(size_t n) {
        ensure(n);
        Payload p(buf.begin() + pos, buf.begin() + pos + n);
        pos += n;
        return p;
    }
    size_t remaining() const { return buf.size() - pos; }
};

} // namespace someip

#endif // SOMEIP_SERIALIZATION_HPP