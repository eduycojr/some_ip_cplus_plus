# SOME/IP Message API

## Purpose

`SomeIpMessage` combines the fixed-size `SomeIpHeader` with a payload byte vector. It is the repository’s message wrapper for packaging and unpacking protocol messages.

## Verified behavior

The following facts are directly supported by the source:

- `SomeIpMessage` contains a `SomeIpHeader header;` and `Payload payload;` in `include/someip/someip_message.hpp:8-10`.
- `serialize()` appends the payload to the serialized header in `include/someip/someip_message.hpp:12-16`.
- `deserialize(const Payload&)` requires at least `SomeIpHeader::SIZE` bytes and computes payload length as `h.length - SomeIpHeader::MIN_LENGTH` in `include/someip/someip_message.hpp:18-29`.
- If the incoming data is shorter than the required header or payload, it throws `std::runtime_error` with messages such as `"message: too small"` and `"message: payload short"` in `include/someip/someip_message.hpp:19-24`.

## Public interface

### Structure fields

- `SomeIpHeader header`
- `Payload payload`

Declared in `include/someip/someip_message.hpp:8-10`.

### Serialization

`SomeIpMessage::serialize()` produces a payload that starts with the header and then appends the message payload. The implementation is straightforward and verified in `include/someip/someip_message.hpp:12-16`.

### Deserialization

`SomeIpMessage::deserialize` performs the following checks:

1. `data.size() < SomeIpHeader::SIZE` => reject as too small
2. Header is read into `SomeIpHeader h`
3. `p_len = h.length - SomeIpHeader::MIN_LENGTH`
4. If `data.size() < SomeIpHeader::SIZE + p_len`, reject as payload short
5. The payload bytes are sliced and stored in `msg.payload`

Evidence: `include/someip/someip_message.hpp:18-29`.

## AI correction note

The repo does not implement a generic “message type name” system beyond the protocol enums in `include/someip/types.hpp`. The actual message framing is header + payload; the message-type semantics are tracked by the header field and the router behavior in `src/message_router.cpp:9-19`.
