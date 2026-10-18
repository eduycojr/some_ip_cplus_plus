# Service Registry API

## Purpose

`ServiceRegistry` is the repository’s method registration and lookup container. It maps a `(ServiceId, MethodId)` pair to a handler function.

## Verified behavior

The following facts are directly supported by the source:

- `ServiceRegistry` stores methods in a `std::map<std::pair<ServiceId, MethodId>, MethodHandler>` inside a mutex-protected registry in `include/someip/service.hpp:18-42`.
- `register_method(ServiceId svc, MethodId mth, MethodHandler handler)` stores the handler under the key pair `(svc, mth)` and locks the mutex while writing in `include/someip/service.hpp:20-24`.
- `unregister_method(...)` removes the entry from the same registry in `include/someip/service.hpp:26-30`.
- `find_handler(...)` returns `std::nullopt` when the key is not found and otherwise returns the registered handler in `include/someip/service.hpp:32-38`.

## Public interface

### Methods

- `register_method(ServiceId svc, MethodId mth, MethodHandler handler)`
- `unregister_method(ServiceId svc, MethodId mth)`
- `find_handler(ServiceId svc, MethodId mth)`

Source: `include/someip/service.hpp:18-42`.

### Handler contract

The registered handler type is `MethodHandler`, defined in `include/someip/types.hpp:59-66` as:

- `std::function<MethodResult(const Payload&, const Endpoint&)>`

This means the handler takes a payload and endpoint and returns a `MethodResult` with a return code and payload.

## AI correction note

The repository uses a minimal registry abstraction. It is not a full service-discovery framework; it is simply a local method dispatcher keyed by service and method identifier. The code in `include/someip/service.hpp` confirms the registry has only a local map and mutex, not a network service-discovery implementation.
