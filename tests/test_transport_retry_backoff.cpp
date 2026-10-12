#include "someip/transport.hpp"
#include <chrono>
#include <cstdio>
#include <iostream>
#include <string>

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

int main() {
    constexpr int kMaxRetries = 3;
    constexpr int kBackoffMs = 100;
    constexpr int kMinExpectedBackoffTotal = (kMaxRetries - 1) * kBackoffMs;

    const uint16_t local_port = find_free_port();
    UdpEndpoint endpoint("127.0.0.1", local_port);

    if (!endpoint.start()) {
        std::fprintf(stderr, "Failed to start UDP endpoint for retry/backoff test\n");
        return 1;
    }

    // UDP sendto of a payload above the maximum datagram size reliably fails
    // and exercises the retry loop in transport.cpp.
    Payload oversized_payload(70000, 0xAB);

    Endpoint dest("127.0.0.1", 65534);  // port likely unused; payload size should fail by itself
    auto start = std::chrono::steady_clock::now();

    bool ok = endpoint.send_to(oversized_payload, dest);

    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - start);

    endpoint.stop();

    std::printf("send_to result=%s, elapsed_ms=%lld\n",
                ok ? "true" : "false",
                static_cast<long long>(elapsed.count()));

    if (ok) {
        std::fprintf(stderr, "ERROR: oversized payload unexpectedly succeeded\n");
        return 2;
    }

    // With max_retries=3, the implementation sleeps after each failed attempt
    // except the last one. A send failure should consume ~200ms total backoff.
    if (elapsed.count() < kMinExpectedBackoffTotal - 25) {
        std::fprintf(stderr,
                     "ERROR: retry/backoff timing too short. expected >= %d ms, got %lld ms\n",
                     kMinExpectedBackoffTotal,
                     static_cast<long long>(elapsed.count()));
        return 3;
    }

    std::puts("PASS: retry/backoff path fails after retries and observes expected backoff delay");
    return 0;
}
