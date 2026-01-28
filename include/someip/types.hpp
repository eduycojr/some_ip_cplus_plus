#ifndef SOMEIP_TYPES_HPP
#define SOMEIP_TYPES_HPP

#include <cstdint>
#include <vector>
#include <string>
#include <functional>
#include <tuple>
#include <chrono>
#include <optional>

namespace someip {

// Basic types
using Uint8  = uint8_t;
using Uint16 = uint16_t;
using Uint32 = uint32_t;
using Uint64 = uint64_t;
using Sint8  = int8_t;
using Sint16 = int16_t;
using Sint32 = int32_t;
using Sint64 = int64_t;
using Float32 = float;
using Float64 = double;
using Boolean = bool;

using Payload = std::vector<uint8_t>;
using Endpoint = std::tuple<std::string, uint16_t>; // (ip, port)

// SOME/IP identifiers
using ServiceId = Uint16;
using MethodId = Uint16;
using ClientId = Uint16;
using SessionId = Uint16;
using InstanceId = Uint16;

// Transport
enum class TransportProtocol : uint8_t {
    UDP = 0,
    TCP = 1
};

// SOME/IP message types (simplified)
enum class MessageType : uint8_t {
    REQUEST = 0x00,
    NOTIFICATION = 0x02,
    RESPONSE = 0x80,
    ERR = 0x81
};

// Return codes (partial)
enum class ReturnCode : uint8_t {
    E_OK = 0x00,
    E_NOT_OK = 0x01,
    E_UNKNOWN = 0xFF
};

// Method result
struct MethodResult {
    ReturnCode return_code;
    Payload payload;
};

// Method handler
using MethodHandler = std::function<MethodResult(const Payload&, const Endpoint&)>;

// Simple logging helper
inline void log_info(const std::string& s) { fprintf(stdout, "[INFO] %s\n", s.c_str()); }
inline void log_debug(const std::string& s) { fprintf(stdout, "[DEBUG] %s\n", s.c_str()); }
inline void log_error(const std::string& s) { fprintf(stderr, "[ERROR] %s\n", s.c_str()); }

// SOME/IP SD defaults
constexpr const char* DEFAULT_SD_MULTICAST = "224.224.224.245";
constexpr uint16_t DEFAULT_SD_PORT = 30490;

} // namespace someip

#endif // SOMEIP_TYPES_HPP