# Software Requirements Specification
## SOME/IP Communication Library and Brake Control Demo

**Project:** SOME/IP C++ Communication Framework  
**Repository:** eduycojr/some_ip_cplus_plus  
**Document Version:** 1.0  
**Date:** 2026-10-10  
**Status:** APPROVED

---

## 1. Introduction

### 1.1 Purpose

This Software Requirements Specification (SRS) defines the functional, non-functional, and interface requirements for the **SOME/IP Communication Library**, a C++ implementation of the SOME/IP (Scalable service-Oriented MiddlewarE over Internet Protocol) protocol. The library is designed to demonstrate automotive ECU communication patterns through request-response messaging, with a brake control use case as the primary demonstration.

### 1.2 Scope

This specification covers:
- The core SOME/IP protocol library (`someip`)
- The transport layer (UDP-based communication)
- Service registration and method routing
- Service discovery mechanisms
- Example client and server applications
- Unit and end-to-end testing infrastructure

**Out of scope:**
- Full AUTOSAR middleware stack
- Production-grade automotive security and compliance
- Advanced diagnostic or monitoring capabilities
- Non-UDP transport mechanisms (TCP, CAN, etc.)

### 1.3 Definitions, Acronyms, and Abbreviations

| Term | Definition |
|------|-----------|
| **SOME/IP** | Scalable service-Oriented MiddlewarE over Internet Protocol |
| **ECU** | Electronic Control Unit (automotive component) |
| **UDP** | User Datagram Protocol |
| **API** | Application Programming Interface |
| **SRS** | Software Requirements Specification |
| **RQ** | Requirement identifier |
| **SID** | Service ID (SOME/IP field) |
| **MID** | Method ID (SOME/IP field) |
| **CID** | Client ID (SOME/IP field) |
| **SESID** | Session ID (SOME/IP field) |
| **SD** | Service Discovery |
| **CMake** | Cross-platform build tool |
| **UnitTest** | Test of individual component/unit |
| **E2E Test** | End-to-end test |

### 1.4 Overview

The SOME/IP library provides a lightweight, portable C++ framework for building automotive-style distributed systems. The library abstracts network transport, serialization, and service routing, allowing developers to define services and methods using a simple API. The brake control demo illustrates how clients can issue commands (press, release, status) to a server that manages brake state.

---

## 2. Overall Description

### 2.1 Product Perspective

The SOME/IP library is a **standalone, self-contained component** for research, educational, and prototyping purposes. It may serve as:
- A reference implementation of SOME/IP core mechanics
- A foundation for automotive middleware projects
- A teaching tool for distributed system communication patterns

### 2.2 Product Features

| Feature | Description |
|---------|-------------|
| **SOME/IP Protocol Implementation** | Full support for 16-byte SOME/IP header and payload serialization |
| **UDP Transport Layer** | Cross-platform UDP socket handling with multicast support |
| **Service Registry** | Register and route methods by service and method ID |
| **Service Discovery** | Multicast-based service announcement and discovery |
| **Message Router** | Route incoming messages to registered handlers |
| **Brake Control Demo** | Example service with press, release, and status methods |
| **Interactive Client** | Console-based client for testing brake commands |
| **Server Application** | Standalone server process for serving brake requests |
| **Test Suite** | Unit and end-to-end tests for validation |
| **Cross-Platform Build** | CMake-based build for Windows, Linux, macOS |

### 2.3 User Classes and Characteristics

| User Class | Characteristics | Primary Use Cases |
|------------|-----------------|-------------------|
| **Library Developer** | Experienced C++ programmer, understands networking | Extend or modify the SOME/IP library |
| **Application Developer** | Implements services using the library API | Build new ECU communication services |
| **QA/Test Engineer** | Validates protocol compliance and behavior | Run tests and verify packet exchange |
| **Researcher/Student** | Learning automotive middleware concepts | Understand SOME/IP communication patterns |

### 2.4 Operating Environment

| Component | Requirement |
|-----------|-------------|
| **OS** | Windows (via WinSock), Linux, macOS |
| **Compiler** | GCC, Clang, MSVC (C++17 compatible) |
| **CMake** | Version 3.10 or later |
| **Dependencies** | Standard C++ library, platform socket APIs |
| **Network** | UDP connectivity (localhost or LAN) |

### 2.5 Design and Implementation Constraints

- **Language**: C++17 (no C++20 features)
- **Portability**: Must support Windows and POSIX-compliant systems
- **Minimal Dependencies**: Avoid heavy third-party libraries
- **Message Format**: Binary SOME/IP protocol (as per SOME/IP specification)
- **Concurrency**: Thread-safe service registry with mutex protection
- **Networking**: UDP-only for transport (multicast optional for SD)

### 2.6 Assumptions and Dependencies

- Users have basic knowledge of networking concepts (sockets, UDP, IP addressing)
- The build environment has a compatible C++17 compiler installed
- Target systems support IPv4 UDP communication
- Multicast is available on the network (for service discovery, when used)

---

## 3. Functional Requirements

### 3.1 Protocol Support (RQ-01)

**Requirement ID:** RQ-01-001  
**Title:** SOME/IP Header Format Support  
**Priority:** Critical  
**Description:**  
The library shall implement the SOME/IP 16-byte header format with the following fields:
- Service ID (uint16, bytes 0-1)
- Method ID (uint16, bytes 2-3)
- Payload Length (uint16, bytes 4-5)
- Client ID (uint16, bytes 6-7)
- Session ID (uint16, bytes 8-9)
- Message Type (uint8, byte 10) — REQUEST, RESPONSE, ERROR
- Protocol Version (uint8, byte 11)
- Interface Version (uint16, bytes 12-13)
- Serializer Version (uint8, byte 14)
- Reserved (uint8, byte 15)

**Acceptance Criteria:**
- Header struct defined in `include/someip/someip_header.hpp`
- All fields serializable and deserializable in network byte order
- Header size verified to be exactly 16 bytes
- Tests validate field assignment and retrieval

---

**Requirement ID:** RQ-01-002  
**Title:** Message Type Support  
**Priority:** High  
**Description:**  
The library shall support three message types via the Message Type field (byte 10):
1. **REQUEST** (0x00): Sent by client to invoke a method
2. **RESPONSE** (0x01): Sent by server to acknowledge or return results
3. **ERROR** (0x02): Sent by server to indicate method failure

**Acceptance Criteria:**
- Message type enum defined in `include/someip/types.hpp`
- Handlers correctly interpret message type
- Error responses propagate error information to caller

---

**Requirement ID:** RQ-01-003  
**Title:** Payload Serialization  
**Priority:** High  
**Description:**  
The library shall serialize and deserialize binary payload data, supporting:
- Byte-level packing/unpacking
- Network byte order (big-endian) for header fields
- Variable payload sizes up to 65535 bytes (limited by uint16 payload length field)

**Acceptance Criteria:**
- Serialization functions defined in `include/someip/serialization.hpp`
- Payload encoded/decoded without data corruption
- Unit tests validate serialization correctness
- Brake control demo transmits status bytes correctly

---

### 3.2 Transport Layer (RQ-02)

**Requirement ID:** RQ-02-001  
**Title:** UDP Socket Management  
**Priority:** Critical  
**Description:**  
The transport layer shall manage UDP sockets for both sending and receiving:
- Bind to a local IP address and port
- Send packets to a remote endpoint (IP and port)
- Receive packets from any source on the bound port
- Handle platform-specific socket APIs (WinSock on Windows, POSIX sockets on Linux/macOS)

**Acceptance Criteria:**
- `UdpEndpoint` class implemented in `include/someip/transport.hpp` and `src/transport.cpp`
- Constructor accepts bind IP and port
- `send_to()` method transmits data to specified endpoint
- `receive_loop()` thread safely handles incoming packets
- Windows and Linux builds link required socket libraries

---

**Requirement ID:** RQ-02-002  
**Title:** Endpoint Lifecycle Management  
**Priority:** High  
**Description:**  
UDP endpoints shall support initialization, start, and stop operations:
- Constructor initializes the socket without binding
- `start()` spawns a receive thread and binds to the configured address
- `stop()` terminates the receive thread and closes the socket
- Destructor cleans up resources

**Acceptance Criteria:**
- `start()` returns true on success, false on failure
- `stop()` is idempotent
- Destructor calls `stop()` if not already stopped
- No resource leaks on abnormal termination

---

**Requirement ID:** RQ-02-003  
**Title:** Multicast Support  
**Priority:** Medium  
**Description:**  
The transport layer shall optionally support multicast group membership for service discovery:
- `join_multicast()` subscribes the socket to a multicast group
- `leave_multicast()` unsubscribes from a multicast group
- Multiple multicast groups may be joined simultaneously

**Acceptance Criteria:**
- Multicast join/leave methods in `UdpEndpoint`
- Multicast packets received on the bound port are delivered to callbacks
- Graceful handling if multicast is not available

---

**Requirement ID:** RQ-02-004  
**Title:** Receive Callback Mechanism  
**Priority:** High  
**Description:**  
Upon receiving a complete SOME/IP message, the transport layer shall invoke a registered callback with:
- The received `SomeIpMessage` (parsed)
- Source endpoint (IP and port)
- Destination endpoint (IP and port)
- Transport protocol identifier

**Acceptance Criteria:**
- `set_callback()` method allows registration of a `TransportCallback`
- Callback invoked for every complete received message
- Callback execution does not block the receive thread

---

### 3.3 Service Model and Routing (RQ-03)

**Requirement ID:** RQ-03-001  
**Title:** Service Method Registration  
**Priority:** Critical  
**Description:**  
The service registry shall allow registration and lookup of methods by service ID and method ID:
- `register_method(ServiceId, MethodId, MethodHandler)` adds a method handler
- `find_handler(ServiceId, MethodId)` returns a handler if registered, or `std::nullopt` if not
- `unregister_method(ServiceId, MethodId)` removes a handler

**Acceptance Criteria:**
- `ServiceRegistry` class implemented in `include/someip/service.hpp`
- Thread-safe via mutex protection
- Multiple handlers can be registered for different service/method pairs
- Handler is a callable (function pointer or std::function)

---

**Requirement ID:** RQ-03-002  
**Title:** Method Handler Invocation  
**Priority:** Critical  
**Description:**  
When a request message is received, the message router shall:
1. Extract the service ID and method ID from the SOME/IP header
2. Query the service registry for a matching handler
3. Invoke the handler with request payload and metadata
4. Return the handler's response or invoke an error handler if not found

**Acceptance Criteria:**
- `MessageRouter` class implemented in `include/someip/message_router.hpp`
- Router accepts a `UdpEndpoint` and `ServiceRegistry`
- Router invokes correct handler for registered service/method pairs
- Router sends error response for unregistered methods

---

**Requirement ID:** RQ-03-003  
**Title:** Thread-Safe Service Registry  
**Priority:** High  
**Description:**  
The service registry shall be thread-safe to allow concurrent registration and lookup from multiple threads (e.g., main thread registering methods, receive thread invoking handlers).

**Acceptance Criteria:**
- All public methods protected by mutex locks
- No deadlock risk under concurrent access
- Register/unregister operations do not block handler invocation

---

### 3.4 Service Discovery (RQ-04)

**Requirement ID:** RQ-04-001  
**Title:** Service Discovery Component  
**Priority:** Medium  
**Description:**  
The library shall provide a service discovery mechanism that:
- Listens on a multicast address and port (default: `239.224.224.245:30490`)
- Announces available services and methods
- Responds to discovery queries from clients

**Acceptance Criteria:**
- `ServiceDiscovery` class implemented in `include/someip/service_discovery.hpp` and `src/service_discovery.cpp`
- Uses UDP multicast transport
- Announces service availability periodically or on-demand
- Clients can discover available services by sending multicast queries

---

**Requirement ID:** RQ-04-002  
**Title:** Service Metadata Exchange  
**Priority:** Medium  
**Description:**  
Service discovery shall communicate:
- Service ID
- Method ID
- Server endpoint (IP and port)
- Protocol version and interface version

**Acceptance Criteria:**
- Discovery messages in SOME/IP SD format
- Metadata sufficient for client to establish direct communication
- No assumptions about prior knowledge of server addresses

---

### 3.5 Brake Control Use Case (RQ-05)

**Requirement ID:** RQ-05-001  
**Title:** Brake Service Definition  
**Priority:** High  
**Description:**  
The brake control demonstration shall define and implement a brake service with:
- **Service ID**: `0x1300`
- **Methods**:
  - `0x0010` → Press Brake
  - `0x0020` → Release Brake
  - `0x0030` → Query Brake Status
- **Server Port**: `3000`
- **Client Port**: `3002`

**Acceptance Criteria:**
- Service and method IDs defined as constants in the codebase
- Server listens on port 3000
- Client binds to or connects through port 3002
- Constants used consistently across server and client applications

---

**Requirement ID:** RQ-05-002  
**Title:** Brake Command Processing  
**Priority:** High  
**Description:**  
The server shall process brake commands:
1. **Press Brake**: Updates internal brake state to "pressed" and returns a confirmation response
2. **Release Brake**: Updates internal brake state to "released" and returns a confirmation response
3. **Query Brake Status**: Returns current brake state (pressed/released) and any associated data

**Acceptance Criteria:**
- Server maintains brake state variable
- Methods update and return state correctly
- No race conditions in state access (use atomics or locks if needed)
- Responses contain appropriate status bytes or error codes

---

**Requirement ID:** RQ-05-003  
**Title:** Brake Status Query Response  
**Priority:** High  
**Description:**  
The Query Brake Status method shall return a payload containing:
- **Byte 0**: Status indicator
  - `0x01` = Brake pressed
  - `0x00` = Brake released
- **Byte 1 (optional)**: Pressure level or additional state information

**Acceptance Criteria:**
- Response payload correctly encodes brake state
- Status query does not modify brake state
- Response generated within 1 second of request receipt

---

### 3.6 Client Application (RQ-06)

**Requirement ID:** RQ-06-001  
**Title:** Interactive Command Interface  
**Priority:** High  
**Description:**  
The client application (`examples/client_app.cpp`) shall provide a console-based interface allowing users to:
- Enter commands: `press`, `release`, `status`, `exit`
- Send corresponding SOME/IP requests to the server
- Display responses and acknowledgements

**Acceptance Criteria:**
- Executable named `client_app` builds from `examples/client_app.cpp`
- Prompts user: `"Enter brake command (press/release/status/exit):"`
- Each command sends correct service ID and method ID
- Displays received response or error message
- `exit` command terminates the application

---

**Requirement ID:** RQ-06-002  
**Title:** Client Endpoint Configuration  
**Priority:** High  
**Description:**  
The client shall:
- Bind to localhost port `3002`
- Send requests to localhost port `3000` (server)
- Wait for responses with a configurable timeout (default 5 seconds)

**Acceptance Criteria:**
- Client endpoint creation in `client_app.cpp` uses port 3002
- Server endpoint specified as `127.0.0.1:3000`
- Timeout handling prevents indefinite blocking
- Clear error message if server is unreachable

---

**Requirement ID:** RQ-06-003  
**Title:** Request Payload Formatting  
**Priority:** Medium  
**Description:**  
Client requests shall correctly format payloads:
- **Press/Release**: No payload (0-byte payload)
- **Status Query**: No payload (0-byte payload)

**Acceptance Criteria:**
- Payload length field in SOME/IP header set to 0x0000 for all commands
- No spurious bytes sent

---

### 3.7 Server Application (RQ-07)

**Requirement ID:** RQ-07-001  
**Title:** Server Initialization and Listening  
**Priority:** Critical  
**Description:**  
The server application (`examples/server_app.cpp`) shall:
- Create a UDP endpoint bound to localhost port `3000`
- Register brake control methods in the service registry
- Start the transport layer and message router
- Log initialization to console

**Acceptance Criteria:**
- Executable named `server_app` builds from `examples/server_app.cpp`
- Logs: `"[INFO] Server listening on port 3000."`
- Successfully binds to port 3000
- Handles errors if port is already in use

---

**Requirement ID:** RQ-07-002  
**Title:** Brake Method Handler Registration  
**Priority:** Critical  
**Description:**  
The server shall register method handlers for:
1. **Service `0x1300`, Method `0x0010`** → Press Brake Handler
2. **Service `0x1300`, Method `0x0020`** → Release Brake Handler
3. **Service `0x1300`, Method `0x0030`** → Query Brake Status Handler

**Acceptance Criteria:**
- All three handlers registered in `ServiceRegistry` on startup
- Handlers are callable and execute without exceptions
- Unregistered methods result in error response

---

**Requirement ID:** RQ-07-003  
**Title:** Response Generation  
**Priority:** High  
**Description:**  
For each received request, the server shall generate a response SOME/IP message with:
- Same Service ID and Method ID as the request
- Message Type set to RESPONSE (0x01) or ERROR (0x02)
- Same Client ID and Session ID as the request
- Appropriate payload (if any)
- Correct payload length

**Acceptance Criteria:**
- Response messages correctly echo request identifiers
- Response payload matches handler output
- Message router sends response back to client
- Timestamps and ordering preserved for transaction tracking

---

**Requirement ID:** RQ-07-004  
**Title:** Concurrent Request Handling  
**Priority:** Medium  
**Description:**  
The server shall handle multiple concurrent client requests:
- Receive thread processes incoming messages without blocking
- Handlers execute asynchronously or in a thread pool if needed
- No requests are dropped due to processing delays

**Acceptance Criteria:**
- Multiple clients can connect simultaneously
- Requests are serviced within reasonable latency (< 100 ms under light load)
- No deadlocks or race conditions in shared state access

---

### 3.8 Error Handling (RQ-08)

**Requirement ID:** RQ-08-001  
**Title:** Unregistered Method Handling  
**Priority:** High  
**Description:**  
When a client sends a request for an unregistered service or method, the server shall:
- Detect the missing handler in the service registry
- Generate an error response with Message Type set to ERROR (0x02)
- Include an error code or reason in the payload (if applicable)
- Log the error to console

**Acceptance Criteria:**
- Error response sent within 1 second
- Server does not crash or hang
- Client receives and can process the error response
- Console logs: `"[ERROR] Method not found: Service 0xXXXX, Method 0xXXXX"`

---

**Requirement ID:** RQ-08-002  
**Title:** Malformed Message Handling  
**Priority:** High  
**Description:**  
If the server receives a malformed SOME/IP message (e.g., header too short, invalid structure), the server shall:
- Detect the malformation during deserialization
- Discard the message or respond with an error
- Log the malformed message
- Continue processing subsequent messages

**Acceptance Criteria:**
- No crashes on malformed input
- Deserializer raises exception or returns error code
- Server continues operating after receiving malformed message

---

**Requirement ID:** RQ-08-003  
**Title:** Network Error Resilience  
**Priority:** Medium  
**Description:**  
The transport layer shall handle network errors gracefully:
- Socket errors (e.g., bind failure, send failure)
- Timeout on receive operations
- Connection loss (if applicable for UDP)

**Acceptance Criteria:**
- Errors logged with diagnostic information
- Application does not crash on network errors
- User receives clear error message
- Server/client can restart and reconnect

---

### 3.9 API Surface (RQ-09)

**Requirement ID:** RQ-09-001  
**Title:** Public API Functions  
**Priority:** High  
**Description:**  
The library shall expose convenience functions in `include/someip/api.hpp`:
- `create_udp_endpoint(bind_ip, bind_port, multicast_addr)` → Creates and returns a `UdpEndpoint`
- `create_service_discovery(multicast, port)` → Creates and returns a `ServiceDiscovery`
- `create_message_router(endpoint, registry)` → Creates and returns a `MessageRouter`

**Acceptance Criteria:**
- All functions declared in `api.hpp`
- Implemented in `src/api.cpp`
- Return smart pointers (unique_ptr or shared_ptr) for memory safety
- Functions used in example applications

---

**Requirement ID:** RQ-09-002  
**Title:** Type Definitions  
**Priority:** High  
**Description:**  
The library shall define core types in `include/someip/types.hpp`:
- `ServiceId` (uint16)
- `MethodId` (uint16)
- `ClientId` (uint16)
- `SessionId` (uint16)
- `MessageType` (enum)
- `TransportProtocol` (enum)
- `Payload` (std::vector<uint8_t>)
- `Endpoint` (struct with IP and port)
- `MethodHandler` (std::function for callbacks)

**Acceptance Criteria:**
- All types defined and documented
- Types used consistently throughout codebase
- Aliases simplify API signatures

---

## 4. Non-Functional Requirements

### 4.1 Performance (RQ-10)

**Requirement ID:** RQ-10-001  
**Title:** Message Latency  
**Priority:** Medium  
**Description:**  
The round-trip time from client request to server response shall be:
- **Target**: < 10 ms on localhost (no network congestion)
- **Acceptable**: < 100 ms on LAN (normal conditions)

**Acceptance Criteria:**
- Client timing test measures request → response time
- Latency logged and reported
- Performance benchmarks documented

---

**Requirement ID:** RQ-10-002  
**Title:** Throughput  
**Priority:** Low  
**Description:**  
The library shall support at least 1,000 request-response exchanges per second (single-threaded, localhost).

**Acceptance Criteria:**
- Load test script available
- Throughput measured and logged
- No packet loss under sustained load

---

**Requirement ID:** RQ-10-003  
**Title:** Memory Footprint  
**Priority:** Medium  
**Description:**  
The library and applications shall have a minimal memory footprint:
- **Library**: < 10 MB static binary size
- **Server**: < 5 MB resident memory at startup

**Acceptance Criteria:**
- Binary sizes measured after release build
- Memory usage monitored during idle and active periods
- No memory leaks detected (valgrind or AddressSanitizer)

---

### 4.2 Reliability and Availability (RQ-11)

**Requirement ID:** RQ-11-001  
**Title:** Packet Delivery Guarantee  
**Priority:** High  
**Description:**  
All SOME/IP messages shall be transmitted and received without corruption:
- Serialization produces valid binary format
- Deserialization correctly reconstructs message
- No data loss during transmission (UDP, same network)

**Acceptance Criteria:**
- Serialization round-trip test (serialize → deserialize → compare)
- End-to-end test verifies payload integrity
- CRC or checksum verification (optional)

---

**Requirement ID:** RQ-11-002  
**Title:** Server Uptime  
**Priority:** Medium  
**Description:**  
The server application shall remain operational for extended periods (minimum 1 hour continuous operation under normal load).

**Acceptance Criteria:**
- Stress test runs for 1 hour
- No crashes, hangs, or resource exhaustion
- All requests processed correctly throughout

---

### 4.3 Maintainability and Code Quality (RQ-12)

**Requirement ID:** RQ-12-001  
**Title:** Code Organization  
**Priority:** High  
**Description:**  
The codebase shall follow a clear directory structure:
```
some_ip_cplus_plus/
├── include/someip/       (public headers)
├── src/                  (implementation)
├── examples/             (client and server apps)
├── tests/                (unit and E2E tests)
├── CMakeLists.txt        (build configuration)
└── readme.md / REQUIREMENTS.md
```

**Acceptance Criteria:**
- Headers in `include/someip/`
- Implementation in `src/`
- Example applications in `examples/`
- Tests in `tests/`
- No circular dependencies

---

**Requirement ID:** RQ-12-002  
**Title:** Documentation  
**Priority:** High  
**Description:**  
Code shall include:
- API header comments (function/class descriptions)
- Implementation comments for non-obvious logic
- README.md with build and usage instructions
- REQUIREMENTS.md (this document)

**Acceptance Criteria:**
- All public functions documented
- Complex algorithms explained
- Build instructions clear and tested
- Example usage provided

---

**Requirement ID:** RQ-12-003  
**Title:** Coding Standards  
**Priority:** Medium  
**Description:**  
The codebase shall follow C++ best practices:
- Use of `const` correctness
- Smart pointers (std::unique_ptr, std::shared_ptr) for memory management
- RAII principles for resource management
- Consistent naming conventions (snake_case for functions, CamelCase for classes)

**Acceptance Criteria:**
- No raw `new` or `delete` without justification
- Smart pointers used for dynamically allocated objects
- Resources cleaned up in destructors
- Code review passes on style and best practices

---

### 4.4 Security and Safety (RQ-13)

**Requirement ID:** RQ-13-001  
**Title:** Input Validation  
**Priority:** High  
**Description:**  
The library shall validate input data to prevent exploits:
- SOME/IP header fields checked for valid ranges
- Payload length field validated (cannot exceed buffer size)
- IP addresses and ports validated for format

**Acceptance Criteria:**
- Deserialization rejects oversized payloads
- Invalid IP addresses logged and rejected
- Fuzzing tests pass (no crashes on malformed input)

---

**Requirement ID:** RQ-13-002  
**Title:** Memory Safety  
**Priority:** High  
**Description:**  
The implementation shall be free of common memory errors:
- No buffer overflows
- No use-after-free
- No uninitialized variables

**Acceptance Criteria:**
- AddressSanitizer or Valgrind runs clean
- Code review checks for memory safety
- Static analysis (e.g., Clang Tidy) passes

---

**Requirement ID:** RQ-13-003  
**Title:** Thread Safety  
**Priority:** High  
**Description:**  
All shared state shall be protected from data races:
- Service registry access guarded by mutex
- Atomic operations used for simple flags
- No concurrent modification of containers without synchronization

**Acceptance Criteria:**
- ThreadSanitizer runs clean
- All mutable shared state identified and protected
- Concurrent stress tests pass

---

### 4.5 Portability (RQ-14)

**Requirement ID:** RQ-14-001  
**Title:** Cross-Platform Compilation  
**Priority:** Critical  
**Description:**  
The library shall compile successfully on:
- **Windows**: Visual Studio 2019 or later, MSVC C++17
- **Linux**: GCC 7+ or Clang 5+, C++17
- **macOS**: Clang 5+, C++17

**Acceptance Criteria:**
- CMake configures without errors on all platforms
- `cmake . && make` (or MSBuild) produces working binaries
- No platform-specific code outside conditional compilation blocks

---

**Requirement ID:** RQ-14-002  
**Title:** Platform Abstraction  
**Priority:** High  
**Description:**  
Platform-specific code (sockets, threads, etc.) shall be isolated and abstracted:
- Windows socket code guarded by `#ifdef _WIN32`
- POSIX socket code in `#else` or `#ifdef __unix__`
- Single interface used by platform-agnostic code

**Acceptance Criteria:**
- All platform-specific code clearly marked
- Headers included conditionally
- No compiler errors due to missing headers
- No linking errors on any platform

---

**Requirement ID:** RQ-14-003  
**Title:** Dependency Constraints  
**Priority:** High  
**Description:**  
The library shall have minimal external dependencies:
- Only standard C++ library
- Platform socket APIs (WinSock on Windows, POSIX on Unix)
- CMake for build configuration
- No dependency on boost, Qt, or other heavy libraries

**Acceptance Criteria:**
- CMakeLists.txt contains no external package dependencies
- Library builds standalone
- Example apps link only to the someip library

---

### 4.6 Build and Deployment (RQ-15)

**Requirement ID:** RQ-15-001  
**Title:** CMake Build System  
**Priority:** Critical  
**Description:**  
The project shall use CMake 3.10+ as the build system:
- CMakeLists.txt at repository root
- Targets defined for library, examples, and tests
- Install targets for library and headers

**Acceptance Criteria:**
- `cmake ..` configures the project
- `make` (or equivalent) builds all targets
- `make install` installs library and headers
- Build directory can be separate from source

---

**Requirement ID:** RQ-15-002  
**Title:** Executable Generation  
**Priority:** High  
**Description:**  
The build shall produce the following executables:
- `server_app` — Server application
- `client_app` — Client application
- `test_serialization` — Serialization tests
- `test_endtoend` — End-to-end tests
- `server_example` and `client_example` (optional)

**Acceptance Criteria:**
- All executables generated in build output directory
- Executables are runnable without external dependencies
- Tests can be executed with `./test_serialization` and `./test_endtoend`

---

**Requirement ID:** RQ-15-003  
**Title:** Release Build Optimization  
**Priority:** Medium  
**Description:**  
Release builds shall be optimized for performance and size:
- `-O2` or `-O3` optimization level
- `-DNDEBUG` to disable assertions
- Symbol stripping for reduced binary size

**Acceptance Criteria:**
- Release build smaller than debug build
- Performance tests show optimization applied
- Stack traces still useful with debug symbols (option to keep them)

---

## 5. External Interface Requirements

### 5.1 User Interfaces (RQ-16)

**Requirement ID:** RQ-16-001  
**Title:** Client Console Interface  
**Priority:** High  
**Description:**  
The client application presents a simple text-based interface:

```
[INFO] Client listening on port 3002.

Enter brake command (press/release/status/exit): press
[INFO] Sent brake press request.
[RESPONSE] Server accepted brake press request.

Enter brake command (press/release/status/exit): status
[INFO] Sent brake status request.
[RESPONSE] Brake Status: PRESSED (0x01)

Enter brake command (press/release/status/exit): release
[INFO] Sent brake release request.
[RESPONSE] Server accepted brake release request.

Enter brake command (press/release/status/exit): exit
[INFO] Client application terminated.
```

**Acceptance Criteria:**
- Prompts are clear and concise
- Responses logged with timestamps or indicators
- Input parsing is case-insensitive or clearly specifies case
- Invalid input handled with helpful error message

---

**Requirement ID:** RQ-16-002  
**Title:** Server Console Interface  
**Priority:** High  
**Description:**  
The server application logs key events to the console:

```
[INFO] Server listening on port 3000.
[INFO] Service 0x1300 registered.
[INFO] Method 0x0010 (Press Brake) registered.
[INFO] Method 0x0020 (Release Brake) registered.
[INFO] Method 0x0030 (Query Brake Status) registered.
[INFO] Server: Press Brake request received. [ClientID: 0x0001, SessionID: 0x0001]
[INFO] Brake pressed. Current state: PRESSED
[INFO] Server: Query Brake Status request received.
[INFO] Brake Status: PRESSED (0x01)
[INFO] Server: Release Brake request received.
[INFO] Brake released. Current state: RELEASED
```

**Acceptance Criteria:**
- Key events logged with timestamps
- Client and session IDs included for tracing
- Error conditions clearly marked with `[ERROR]` prefix
- Log levels (INFO, WARNING, ERROR) used appropriately

---

### 5.2 Hardware Interfaces (RQ-17)

**Requirement ID:** RQ-17-001  
**Title:** Network Interface  
**Priority:** Critical  
**Description:**  
The library shall communicate via standard network interfaces:
- IPv4 UDP sockets
- Localhost (127.0.0.1) for testing
- LAN IP addresses for distributed scenarios

**Acceptance Criteria:**
- Sockets created with `AF_INET` and `SOCK_DGRAM`
- Both 127.0.0.1 and non-loopback addresses supported
- Multicast support for service discovery (optional)

---

### 5.3 Software Interfaces (RQ-18)

**Requirement ID:** RQ-18-001  
**Title:** Library Linking  
**Priority:** High  
**Description:**  
External applications shall link against the someip library:
- Static library (`libsomeip.a` or `someip.lib`)
- Public headers in `include/someip/`
- CMake target `someip` exportable

**Acceptance Criteria:**
- `target_link_libraries(my_app PRIVATE someip)` succeeds
- Public headers compile without errors
- Library symbols resolved at link time

---

**Requirement ID:** RQ-18-002  
**Title:** Callback Interface  
**Priority:** High  
**Description:**  
Client code shall define method handlers as `std::function` callables:

```cpp
using MethodHandler = std::function<Payload(const Payload&, const SomeIpMessage&)>;

MethodHandler press_handler = [](const Payload& request, const SomeIpMessage& msg) {
    // Update brake state
    return Payload(); // Empty response
};
```

**Acceptance Criteria:**
- Handler signature defined in `types.hpp`
- Handlers can be lambdas, function pointers, or std::function
- Registry accepts any callable matching the signature

---

## 6. Other Nonfunctional Requirements

### 6.1 Compliance and Standards (RQ-19)

**Requirement ID:** RQ-19-001  
**Title:** C++ Standard Compliance  
**Priority:** High  
**Description:**  
The codebase shall conform to C++17 standard:
- No C++20 features
- Code compilable with `-std=c++17` flag
- Use of standard library features available in C++17

**Acceptance Criteria:**
- Compilation succeeds with `-std=c++17 -pedantic`
- No compiler warnings (with `-Wall -Wextra`)
- No deprecation warnings

---

**Requirement ID:** RQ-19-002  
**Title:** SOME/IP Protocol Compliance  
**Priority:** High  
**Description:**  
The implementation shall comply with the SOME/IP protocol specification:
- 16-byte header format as specified
- Message types and serialization as per standard
- Payload encoding conventions respected

**Acceptance Criteria:**
- Wireshark can parse generated packets
- tcpdump output shows correct structure
- Third-party protocol analyzers recognize messages

---

### 6.2 Testability (RQ-20)

**Requirement ID:** RQ-20-001  
**Title:** Unit Test Coverage  
**Priority:** High  
**Description:**  
The library shall include unit tests for core components:
- Serialization round-trip tests
- Service registry registration and lookup
- Message router dispatch logic

**Acceptance Criteria:**
- `test_serialization` executable in build
- Tests cover main code paths
- All tests pass in CI/CD pipeline

---

**Requirement ID:** RQ-20-002  
**Title:** End-to-End Tests  
**Priority:** High  
**Description:**  
The suite shall include end-to-end tests:
- Start server and client in same test process or separate processes
- Send requests and verify responses
- Validate brake control flow

**Acceptance Criteria:**
- `test_endtoend` executable in build
- Tests exercise brake press, release, status commands
- Server and client integration verified

---

**Requirement ID:** RQ-20-003  
**Title:** Test Execution  
**Priority:** High  
**Description:**  
Tests shall be executable via standard methods:
- `./test_serialization` from command line
- `./test_endtoend` from command line
- Tests produce clear pass/fail output

**Acceptance Criteria:**
- Exit code 0 on all tests passing
- Exit code non-zero on any test failure
- Each test result logged with pass/fail status

---

## 7. Design Constraints and Considerations

### 7.1 Design Decisions (RQ-21)

**Requirement ID:** RQ-21-001  
**Title:** UDP-Only Transport  
**Priority:** High  
**Description:**  
The library shall use UDP as the only transport layer (no TCP, CAN, Ethernet):
- Rationale: Simplicity, low overhead, suitable for automotive simulation
- Trade-off: No guaranteed delivery (acceptable for demo/educational use)

**Acceptance Criteria:**
- No TCP or alternative transport code present
- UDP documented as transport choice in README

---

**Requirement ID:** RQ-21-002  
**Title:** Synchronous Request-Response Model  
**Priority:** High  
**Description:**  
The client-server communication model is synchronous:
- Client sends request and waits for response
- No asynchronous callback model (initially)
- Timeout prevents indefinite blocking

**Acceptance Criteria:**
- Client `send_request()` blocks until response or timeout
- Timeout configurable (default 5 seconds)
- Timeout results in exception or error code

---

**Requirement ID:** RQ-21-003  
**Title:** Single-Process Service Discovery  
**Priority:** Medium  
**Description:**  
Service discovery is optional for this demo:
- Server and client can be configured with IP:port directly
- SD can be enabled for larger deployments

**Acceptance Criteria:**
- Example apps work without SD
- SD can be optionally enabled
- Configuration documented

---

## 8. Appendix

### 8.1 Traceability Matrix

| RQ ID | Component | Test Case | Status |
|-------|-----------|-----------|--------|
| RQ-01-001 | someip_header.hpp | test_serialization | TODO |
| RQ-01-002 | types.hpp | test_serialization | TODO |
| RQ-01-003 | serialization.hpp | test_serialization | TODO |
| RQ-02-001 | transport.hpp/cpp | test_endtoend | TODO |
| RQ-02-002 | UdpEndpoint::start/stop | test_transport | TODO |
| RQ-02-003 | UdpEndpoint::join/leave_multicast | test_sd | TODO |
| RQ-02-004 | TransportCallback | test_endtoend | TODO |
| RQ-03-001 | service.hpp | test_service_registry | TODO |
| RQ-03-002 | message_router.hpp/cpp | test_message_routing | TODO |
| RQ-03-003 | ServiceRegistry (mutex) | test_concurrent | TODO |
| RQ-04-001 | service_discovery.hpp/cpp | test_sd | TODO |
| RQ-05-001 | brake service constants | server_app.cpp | TODO |
| RQ-05-002 | brake handlers | server_app.cpp | TODO |
| RQ-05-003 | brake status query | server_app.cpp | TODO |
| RQ-06-001 | client_app.cpp | manual test | TODO |
| RQ-06-002 | client endpoint | client_app.cpp | TODO |
| RQ-07-001 | server_app.cpp | manual test | TODO |
| RQ-07-002 | handler registration | server_app.cpp | TODO |
| RQ-08-001 | error handling | test_error_cases | TODO |
| RQ-09-001 | api.hpp | examples | TODO |
| RQ-14-001 | CMakeLists.txt | CI builds | TODO |
| RQ-15-001 | CMakeLists.txt | build test | TODO |
| RQ-20-001 | test_serialization | make test | TODO |
| RQ-20-002 | test_endtoend | make test | TODO |

### 8.2 Glossary

- **Brake Control Demo**: Example application simulating automotive brake command/status system
- **SOME/IP**: Protocol for automotive middleware communication
- **Service**: A named entity that provides methods (e.g., "Brake Service")
- **Method**: A callable operation on a service (e.g., "Press Brake")
- **Handler**: A C++ function registered to process method calls
- **Payload**: Binary data transmitted in SOME/IP message body
- **Endpoint**: Network address (IP + port)
- **Serialization**: Converting structured data to binary format
- **Deserialization**: Parsing binary data back to structured format
- **Transport**: Network layer handling packet transmission

### 8.3 References

- AUTOSAR SOME/IP Protocol Specification
- C++17 Standard (ISO/IEC 14882:2017)
- IEEE 802.3 (Ethernet)
- RFC 3986 (IPv4 addressing)
- Project README.md
- GitHub repository: https://github.com/eduycojr/some_ip_cplus_plus

### 8.4 Document History

| Version | Date | Author | Changes |
|---------|------|--------|---------|
| 1.0 | 2026-10-10 | eduycojr | Initial SRS; comprehensive requirements baseline |

---

**Document Approval**

| Role | Name | Date | Signature |
|------|------|------|-----------|
| Author | eduycojr | 2026-10-10 | ✓ |
| Project Lead | — | — | — |
| QA Review | — | — | — |

---

**End of Software Requirements Specification**
