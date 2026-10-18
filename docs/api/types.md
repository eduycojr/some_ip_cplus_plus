# SOME/IP Types and Protocol Enums

## Purpose

`include/someip/types.hpp` defines the type aliases, transport type, SOME/IP message enums, and method callback contracts used by the rest of the codebase.

## Verified behavior

The following facts are directly supported by the source:

- `Payload` is a `std::vector<uint8_t>` in `include/someip/types.hpp:28-29`.
- `Endpoint` is a tuple of `(std::string, uint16_t)` in `include/someip/types.hpp:28-29`.
- `MessageType` enum values are `REQUEST = 0x00`, `NOTIFICATION = 0x02`, `RESPONSE = 0x80`, `ERR = 0x81` in `include/someip/types.hpp:44-49`.
- `ReturnCode` values are `E_OK = 0x00`, `E_NOT_OK = 0x01`, `E_UNKNOWN = 0xFF` in `include/someip/types.hpp:52-57`.
- `MethodResult` contains a `ReturnCode return_code` and a `Payload payload` in `include/someip/types.hpp:59-63`.
- `MethodHandler` is `std::function<MethodResult(const Payload&, const Endpoint&)>` in `include/someip/types.hpp:65-66`.

## Logging helpers

The file includes minimal logging helpers:

- `log_info`, `log_debug`, `log_warn`, `log_error`

These are defined in `include/someip/types.hpp:68-72` and print to stdout/stderr using `fprintf`.

## AI correction note

The README’s prose talks about “ERROR” and an informal brake-status model, but the typed protocol model in this repo uses `ERR` and an explicit `ReturnCode` enum. The code is therefore more precise than the prose. These definitions are the source of truth for protocol values.
