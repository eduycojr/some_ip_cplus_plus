# SOME/IP Header API

## Purpose

`SomeIpHeader` is the fixed-size SOME/IP message header defined in `include/someip/someip_header.hpp`. It models the 16-byte SOME/IP header used by the protocol implementation in this repository.

## Verified behavior

The following facts are directly supported by the source:

- The header is a 16-byte structure, with the comment `// SOME/IP header: 16 bytes` and `static constexpr size_t SIZE = 16;` in `include/someip/someip_header.hpp:10-23`.
- The serialized header writes all fields in order: service_id, method_id, length, client_id, session_id, protocol_version, interface_version, message_type, return_code in `include/someip/someip_header.hpp:25-36`.
- The header deserializer validates `data.size() >= SIZE` and rejects too-small payloads in `include/someip/someip_header.hpp:39-53`.
- The minimum accepted length is 8 (`MIN_LENGTH = 8`) and the code rejects lengths below that in `include/someip/someip_header.hpp:22-23` and `:52`.
- The header is used as the canonical 16-byte framing layer for the message format in `include/someip/someip_message.hpp:18-29`.

## Public interface

### Structure fields

- `Uint16 service_id`
- `Uint16 method_id`
- `Uint32 length`
- `Uint16 client_id`
- `Uint16 session_id`
- `Uint8 protocol_version`
- `Uint8 interface_version`
- `Uint8 message_type`
- `Uint8 return_code`

These are declared in `include/someip/someip_header.hpp:11-20`.

### Constants

- `SIZE = 16` — fixed header size
- `MIN_LENGTH = 8` — minimum valid header length

Defined in `include/someip/someip_header.hpp:22-23`.

### Serialization and deserialization

- `serialize()` writes the binary header in order using `SerializationBuffer`.
- `deserialize(const Payload&)` enforces a minimum buffer length and rejects invalid small lengths.

Evidence: `include/someip/someip_header.hpp:25-53`.

## AI correction note

A common misconception is that the protocol uses a generic `ERROR` message type name. The actual code defines `enum class MessageType` with `ERR = 0x81` in `include/someip/types.hpp:44-49`. The test also asserts `MessageType::ERR` specifically in `tests/test_message_router_error.cpp:137-153`.

This means the accurate literal is `ERR`, not a generic `ERROR` field name.
