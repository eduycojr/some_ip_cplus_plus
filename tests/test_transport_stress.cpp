#include "someip/transport.hpp"
#include <atomic>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <thread>
#include <vector>

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

static SomeIpMessage make_valid_request(uint16_t client_id,
                                       uint16_t service_id,
                                       uint16_t method_id,
                                       uint8_t tag) {
    SomeIpMessage msg{};
    msg.header.service_id = service_id;
    msg.header.method_id = method_id;
    msg.header.client_id = client_id;
    msg.header.session_id = 0x1234;
    msg.header.protocol_version = 1;
    msg.header.interface_version = 1;
    msg.header.message_type = static_cast<uint8_t>(MessageType::REQUEST);
    msg.header.return_code = static_cast<uint8_t>(ReturnCode::E_OK);

    Payload payload;
    payload.push_back(tag);
    payload.push_back(0xAA);
    payload.push_back(0xBB);
    payload.push_back(0xCC);

    msg.payload = payload;
    msg.header.length = static_cast<Uint32>(SomeIpHeader::MIN_LENGTH + payload.size());
    return msg;
}

int main(int argc, char** argv) {
    const int num_clients = (argc > 1) ? std::atoi(argv[1]) : 8;
    const int sends_per_client = (argc > 2) ? std::atoi(argv[2]) : 128;

    const uint16_t port = find_free_port();
    std::atomic<int> received_count{0};

    UdpEndpoint server("127.0.0.1", port);
    server.set_callback([&](const SomeIpMessage& /*msg*/,
                           const Endpoint& /*src*/,
                           const Endpoint& /*dst*/,
                           TransportProtocol /*proto*/) {
        received_count.fetch_add(1, std::memory_order_relaxed);
    });

    if (!server.start()) {
        std::fprintf(stderr, "Failed to start server endpoint for stress test\n");
        return 1;
    }

    std::vector<std::thread> workers;
    workers.reserve(num_clients);

    for (int i = 0; i < num_clients; ++i) {
        workers.emplace_back([i, sends_per_client, port]() {
            UdpEndpoint client("127.0.0.1", 0);
            if (!client.start()) {
                return;
            }

            for (int j = 0; j < sends_per_client; ++j) {
                SomeIpMessage req = make_valid_request(
                    static_cast<uint16_t>(0x1000 + i),
                    static_cast<uint16_t>(0x2000),
                    static_cast<uint16_t>(0x3000 + j),
                    static_cast<uint8_t>(i + j)
                );

                Payload pkt = req.serialize();
                Endpoint dest("127.0.0.1", port);
                client.send_to(pkt, dest);
                std::this_thread::yield();
            }

            client.stop();
        });
    }

    for (auto& t : workers) {
        t.join();
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    server.stop();

    std::printf("Stress test summary: clients=%d sends_per_client=%d received=%d\n",
                num_clients, sends_per_client, received_count.load());

    if (received_count.load() <= 0) {
        std::fprintf(stderr, "ERROR: no valid SOME/IP payloads were received during stress test\n");
        return 2;
    }

    std::puts("PASS: multi-client stress test completed without crash and received messages");
    return 0;
}
