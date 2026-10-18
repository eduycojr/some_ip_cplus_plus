# WBS 3.5 Change Note

## Summary

This change set documents the repository’s current SOME/IP implementation and records the verification evidence used to validate the generated API/module documentation.

## Scope

The work in this branch is documentation and verification-only. No functional protocol logic was intentionally changed in the committed docs work; instead, the documentation was generated from the source and cross-checked against the implementation.

## Source evidence used

The documentation set was produced from the following verified sources:

- include/someip/types.hpp
- include/someip/service.hpp
- include/someip/message_router.hpp
- include/someip/someip_header.hpp
- include/someip/someip_message.hpp
- src/message_router.cpp
- tests/test_serialization.cpp
- tests/test_message_router_error.cpp

## Verified claims

The following claims are supported by source and tests:

- `SomeIpHeader` is a 16-byte fixed-size header; this is asserted by code and the serialization test.
- `SomeIpMessage` verifies header and payload length before consuming a message.
- `MessageRouter` emits `ERR` packets for unknown handlers with `ReturnCode::E_UNKNOWN`.
- `tests/test_serialization.cpp` verifies a valid round-trip serialization and deserialization.
- `tests/test_message_router_error.cpp` verifies canonical router error behavior.

## AI mistakes and corrections

The repository README language is broader and more informal than the actual protocol implementation. In particular:

- The actual protocol constant is `ERR` (`0x81`), not a generic `ERROR` type.
- The runtime behavior is bounded by the minimal implementation in `src/message_router.cpp`; it does not implement a full protocol stack or all service-discovery semantics.
- Documentation must not describe unsupported behavior as implemented unless the code clearly demonstrates it.

## Verification status

This document is a process record for WBS-3.5. It remains a human-owned documentation artifact until the final PR review, approval, and merge are completed.
