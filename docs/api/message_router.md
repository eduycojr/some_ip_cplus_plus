# Message Router API

## Purpose

`MessageRouter` is the routing layer used by the repository’s minimal SOME/IP message flow. Its role is to receive a message, dispatch it to a registered handler, and then send either a response or an error packet.

## Verified behavior

The following facts are directly supported by the source:

- `MessageRouter` takes a `std::shared_ptr<UdpEndpoint>` and a `ServiceRegistry&` in `include/someip/message_router.hpp:12-28`.
- `route()` only handles `REQUEST` messages and ignores other types for now in `src/message_router.cpp:9-22`.
- If the service/method pair is not found, `route()` calls `send_error(msg, ReturnCode::E_UNKNOWN, src);` and returns in `src/message_router.cpp:11-16`.
- `send_response(...)` constructs a response header with `message_type = RESPONSE` and a length based on payload size + `SomeIpHeader::MIN_LENGTH` in `src/message_router.cpp:24-40`.
- `send_error(...)` constructs canonical error packets with `message_type = ERR` and `return_code = rc` in `src/message_router.cpp:42-56`.
- The error test confirms this path by asserting `actual_type == MessageType::ERR` and `actual_rc == ReturnCode::E_UNKNOWN` in `tests/test_message_router_error.cpp:131-153`.

## Public interface

- `MessageRouter(std::shared_ptr<UdpEndpoint> endpoint, ServiceRegistry& registry);`
- `void route(const SomeIpMessage& msg, const Endpoint& src, const Endpoint& dst, TransportProtocol proto);`
- `void send_response(const SomeIpMessage& request, const MethodResult& result, const Endpoint& dest);`
- `void send_error(const SomeIpMessage& request, ReturnCode rc, const Endpoint& dest);`

Source: `include/someip/message_router.hpp:12-28`.

## AI correction note

The README and some informal descriptions use a more general “error” or “status” language. The actual code path is narrower: with the current minimal implementation, unsupported service/method requests return a canonical `ERR` packet and `E_UNKNOWN`, not a generic error object or richer status model. The source and test both confirm this behavior.
