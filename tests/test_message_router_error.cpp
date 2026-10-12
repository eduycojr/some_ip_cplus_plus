#include "someip/message_router.hpp"
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <cstdio>
#include <mutex>
#include <string>
#include <tuple>

using namespace someip;

static uint16_t find_free_port() {
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        return 34000;
    }
#endif

    socket_t sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == INVALID_SOCKET_VAL) {
#ifdef _WIN32
        WSACleanup();
#endif
        return 34000;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    addr.sin_port = 0;

    if (bind(sock, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {
#ifdef _WIN32
        closesocket(sock);
        WSACleanup();
#else
        close(sock);
#endif
        return 34000;
    }

    sockaddr_in bound{};
    socklen_t len = sizeof(bound);
    if (getsockname(sock, reinterpret_cast<sockaddr*>(&bound), &len) < 0) {
#ifdef _WIN32
        closesocket(sock);
        WSACleanup();
#else
        close(sock);
#endif
        return 34000;
    }

    uint16_t port = ntohs(bound.sin_port);

#ifdef _WIN32
    closesocket(sock);
    WSACleanup();
#else
    close(sock);
#endif
    return port;
}

static SomeIpMessage make_request_message(uint16_t service_id,
                                         uint16_t method_id,
                                         uint16_t client_id,
                                         uint16_t session_id) {
    SomeIpMessage msg{};
    msg.header.service_id = service_id;
    msg.header.method_id = method_id;
    msg.header.client_id = client_id;
    msg.header.session_id = session_id;
    msg.header.protocol_version = 1;
    msg.header.interface_version = 1;
    msg.header.message_type = static_cast<uint8_t>(MessageType::REQUEST);
    msg.header.return_code = static_cast<uint8_t>(ReturnCode::E_OK);

    Payload payload = {0x10, 0x20, 0x30, 0x40};
    msg.payload = payload;
    msg.header.length = static_cast<Uint32>(SomeIpHeader::MIN_LENGTH + msg.payload.size());
    return msg;
}

int main() {
    const uint16_t port = find_free_port();

    auto sender = std::make_shared<UdpEndpoint>("127.0.0.1", port);
    if (!sender->start()) {
        std::fprintf(stderr, "Failed to start sender endpoint on port %u\n", port);
        return 1;
    }

    std::mutex m;
    std::condition_variable cv;
    bool received = false;
    SomeIpMessage received_msg{};

    sender->set_callback([&](const SomeIpMessage& msg,
                             const Endpoint& /*src*/,
                             const Endpoint& /*dst*/,
                             TransportProtocol /*proto*/) {
        std::lock_guard<std::mutex> lock(m);
        received_msg = msg;
        received = true;
        cv.notify_one();
    });

    ServiceRegistry registry;
    MessageRouter router(sender, registry);

    Endpoint src("127.0.0.1", port);
    Endpoint dst("127.0.0.1", port);

    SomeIpMessage request = make_request_message(0x1000, 0x0101, 0x0001, 0x0022);
    router.route(request, src, dst, TransportProtocol::UDP);

    {
        std::unique_lock<std::mutex> lock(m);
        if (!cv.wait_for(lock, std::chrono::seconds(3), [&] { return received; })) {
            std::fprintf(stderr, "Timed out waiting for router error response\n");
            sender->stop();
            return 2;
        }
    }

    sender->stop();

    const uint8_t actual_type = received_msg.header.message_type;
    const uint8_t actual_rc = received_msg.header.return_code;

    std::printf("Received message type=0x%02X, return_code=0x%02X\n",
                actual_type, actual_rc);

    if (actual_type != static_cast<uint8_t>(MessageType::ERR)) {
        std::fprintf(stderr,
                     "ERROR: expected MessageType::ERR (0x%02X), got 0x%02X\n",
                     static_cast<uint8_t>(MessageType::ERR),
                     actual_type);
        return 3;
    }

    if (actual_rc != static_cast<uint8_t>(ReturnCode::E_UNKNOWN)) {
        std::fprintf(stderr,
                     "ERROR: expected E_UNKNOWN (0x%02X), got 0x%02X\n",
                     static_cast<uint8_t>(ReturnCode::E_UNKNOWN),
                     actual_rc);
        return 4;
    }

    std::puts("PASS: MessageRouter error path emits canonical MessageType::ERR = 0x81");
    return 0;
}
