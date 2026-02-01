# SOME/IP Communication Program - Mini Project

## Overview

This project implements a **SOME/IP communication program** in C++ simulating a **automotive ECU communication**. The program showcases **request-response messaging** for use cases like **Brake Control** and **Tail Light Signaling**, adhering to the SOME/IP protocol standards.

---

## Features

### Core Features
1. **SOME/IP Protocol Implementation**:
   - **16-byte Header**: Includes mandatory fields like Service ID, Method ID, Payload Length, Client ID, Session ID, and Message Type (`REQUEST`, `RESPONSE`, `ERROR`).
   - **Payload Serialization**: Implements binary packing/unpacking for strict compliance with SOME/IP standards.
   - **UDP Transport**: Uses sockets for packet communication between client and server.

2. **Brake Control Use Case**:
   - Simulates pressing and releasing a brake from the client to a server.
   - Server handles brake requests (`press`, `release`) and provides feedback confirming the operation.
   - Supports querying brake status (`active`, `inactive`, brake pressure) via the client.

3. **Real-Time Communication**:
   - Interactive console-based client app for sending commands and observing server response.

4. **Packet Inspection and Validation**:
   - Tools like **Wireshark** and **tcpdump** can monitor and decode UDP packets to confirm SOME/IP compliance.

---

## Updates and Enhancements
This release includes the **Brake Control Demo** with the following updates:
1. **Client Application**:
   - Added interactive user input-based brake control options:
     - `press`: Sends a request to press the brake.
     - `release`: Sends a request to release the brake.
     - `status`: Queries the server for brake status (e.g., `active`, pressure level).
     - `exit`: Terminates the client console application.

2. **Server Application**:
   - Registered the following methods to handle brake commands:
     - `0x0010` → Press Brake.
     - `0x0020` → Release Brake.
     - `0x0030` → Query Brake Status.

3. **Message Structures**:
   - **Service ID**: `0x1300` for Brake Control.
   - **Method IDs**:
     - `Press Brake`: `0x0010`.
     - `Release Brake`: `0x0020`.
     - `Status Query`: `0x0030`.
   - **Payload**:
     - Method calls (Press/Release) do not require a payload.
     - Status responses include payload (`0x01` for pressed, `0x00` for released).

---

## Implementation Details

### System Components
| Component               | Description                                                | Files                                |
|--------------------------|------------------------------------------------------------|--------------------------------------|
| **Transport Layer**      | Handles UDP communication for transmitting and receiving packets.  | `transport.hpp/cpp`                  |
| **Serialization**        | Encodes/decodes SOME/IP header and payload.               | `serialization.hpp`                  |
| **Protocol Header**      | Defines the SOME/IP 16-byte header format.                | `someip_header.hpp`                  |
| **Message Handling**     | Combines header and payload into messages.                | `someip_message.hpp`                 |
| **Service Registry**     | Registers methods and routes requests to handlers.        | `service.hpp`                        |
| **Message Routing**      | Routes incoming messages and sends responses.             | `message_router.hpp/cpp`             |
| **Client Application**   | Sends `press`, `release`, `status`, or `exit` commands via console. | `client_app.cpp`                     |
| **Server Application**   | Processes brake-related requests and responds.              | `server_app.cpp`                     |

### Communication Workflow
**Request-Response Flow**:
1. **Client** sends brake commands (`press/release/status`) to the server over UDP.
2. **Server** processes the request via `ServiceRegistry` and responds with confirmation or brake status.

---

## Installation Instructions

### Prerequisites
1. **Compilers**:
   - C++17-compatible compiler (e.g., `g++` or `clang++`).
2. **Build Tools**:
   - **CMake** 3.10 or higher.

---

### Steps to Build and Run
1. **Clone the Repository**:
   ```bash
   git clone <repository_url>
   cd some_ip_cplus_plus
   ```

2. **Build the Program Using CMake**:
   Before running the application, ensure the **Visual Studio 17 2022** or another suitable C++17-compatible compiler is installed.
   
   mkdir build
   cd build
   cmake .. -G "Visual Studio 17 2022" -A x64
   MSBuild.exe someip_cpp.sln /p:Configuration=Release
   ```

3. **Run the Applications**:
   1. **Start the Server**:
      ```bash
      ./server_app
      ```
   2. **Start the Client**:
      ```bash
      ./client_app
      ```

---

## Verification Using Traffic Inspection

### Packet Monitoring Tools
1. **Wireshark**:
   - Capture UDP packets:
     ```plaintext
     udp.port == 3000 || udp.port == 3002
     ```
   - Validate SOME/IP header and payload compliance.
2. **tcpdump**:
   - Monitor packets in the terminal:
     ```bash
     sudo tcpdump -i lo udp port 3000 or port 3002 -X
     ```
3. **Python Decoding Script**:
   - Use `scapy` to programmatically extract and decode SOME/IP headers and payloads.

---

## Functional Tests: Brake Control Test Case

### Client Commands
| Command    | Description                               |
|------------|-------------------------------------------|
| `press`    | Requests the server to press the brake.  |
| `release`  | Requests the server to release the brake. |
| `status`   | Queries current brake status.            |
| `exit`     | Terminates the client application.       |

**Sample Client Console:**
```plaintext
[INFO] Client listening on port 3002.

Enter brake command (press/release/status/exit): press
[INFO] Sent brake press request.

Enter brake command (press/release/status/exit): status
[INFO] Sent brake status request.

Enter brake command (press/release/status/exit): release
[INFO] Sent brake release request.

Enter brake command (press/release/status/exit): exit
[INFO] Client application terminated.
```

**Sample Server Console:**
```plaintext
[INFO] Server listening on port 3000.
[INFO] Server: Press Brake request received.
[INFO] Server: Brake Status request received.
[INFO] Server: Release Brake request received.
```

---
