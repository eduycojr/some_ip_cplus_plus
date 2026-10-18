#include "someip/transport.hpp"
#include "someip/someip_message.hpp"
#include <iostream>
#include <cstring>
#include <vector>
#include <thread>
#include <chrono>
#include <mutex>

#ifndef _WIN32
#include <sys/time.h>
#endif

namespace someip {

#ifdef _WIN32
static bool winsock_initialized = false;
#endif

// Defaults approved by WBS 2.5
constexpr int DEFAULT_TIMEOUT_MS = 2000;
constexpr int DEFAULT_MAX_RETRIES = 3;
constexpr int DEFAULT_BACKOFF_MS = 100;

// Coarse-grained transport mutex to protect socket and internal state
static std::mutex g_transport_mutex;

UdpEndpoint::UdpEndpoint(const std::string& bind_ip, uint16_t bind_port)
    : bind_ip_(bind_ip), bind_port_(bind_port) {}

UdpEndpoint::~UdpEndpoint() {
    stop();
}

bool UdpEndpoint::start() {
    {
        std::lock_guard<std::mutex> guard(g_transport_mutex);
        if (running_) return true;

#ifdef _WIN32
        if (!winsock_initialized) {
            WSADATA wsaData;
            if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
                log_error("WSAStartup failed");
                return false;
            }
            winsock_initialized = true;
        }
#endif

        sock_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (sock_ == INVALID_SOCKET_VAL) {
            log_error("socket() failed");
            return false;
        }

        int reuse = 1;
#ifdef _WIN32
        setsockopt(sock_, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse));
#else
        setsockopt(sock_, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
#endif

        // Wake recvfrom periodically so stop() can join on all platforms.
#ifdef _WIN32
        DWORD rcv_to = 100; // milliseconds
        setsockopt(sock_, SOL_SOCKET, SO_RCVTIMEO, (const char*)&rcv_to, sizeof(rcv_to));
#else
        timeval rcv_to{};
        rcv_to.tv_sec = 0;
        rcv_to.tv_usec = 100000; // 100 ms
        setsockopt(sock_, SOL_SOCKET, SO_RCVTIMEO, &rcv_to, sizeof(rcv_to));
#endif

        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(bind_port_);
        inet_pton(AF_INET, bind_ip_.c_str(), &addr.sin_addr);

        if (bind(sock_, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
            log_error("bind() failed");
#ifdef _WIN32
            closesocket(sock_);
            sock_ = INVALID_SOCKET_VAL;
#else
            close(sock_);
            sock_ = -1;
#endif
            return false;
        }

        running_ = true;
    }

    // Start receive thread outside the lock
    recv_thread_ = std::thread(&UdpEndpoint::receive_loop, this);
    return true;
}

void UdpEndpoint::stop() {
    // set running_ false and close socket under lock, then join
    {
        std::lock_guard<std::mutex> guard(g_transport_mutex);
        if (!running_) return;
        running_ = false;
#ifdef _WIN32
        if (sock_ != INVALID_SOCKET_VAL) {
            closesocket(sock_);
            sock_ = INVALID_SOCKET_VAL;
        }
#else
        if (sock_ >= 0) {
            ::close(sock_);
            sock_ = -1;
        }
#endif
    }
    if (recv_thread_.joinable()) recv_thread_.join();
}

bool UdpEndpoint::join_multicast(const std::string& mcast_addr) {
    std::lock_guard<std::mutex> guard(g_transport_mutex);
    struct ip_mreq mreq{};
    inet_pton(AF_INET, mcast_addr.c_str(), &mreq.imr_multiaddr);
    inet_pton(AF_INET, bind_ip_.c_str(), &mreq.imr_interface);
    if (setsockopt(sock_, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mreq, sizeof(mreq)) < 0) {
        log_error("setsockopt IP_ADD_MEMBERSHIP failed");
        return false;
    }
    return true;
}

bool UdpEndpoint::leave_multicast(const std::string& mcast_addr) {
    std::lock_guard<std::mutex> guard(g_transport_mutex);
    struct ip_mreq mreq{};
    inet_pton(AF_INET, mcast_addr.c_str(), &mreq.imr_multiaddr);
    inet_pton(AF_INET, bind_ip_.c_str(), &mreq.imr_interface);
    setsockopt(sock_, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char*)&mreq, sizeof(mreq));
    return true;
}

bool UdpEndpoint::send_to(const Payload& data, const Endpoint& dest) {
    std::string ip; uint16_t port;
    std::tie(ip, port) = dest;
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);

    // Implement retry/backoff using the approved defaults (coarse-grained protection)
    for (int attempt = 0; attempt < DEFAULT_MAX_RETRIES; ++attempt) {
        int sent = 0;
        {
            std::lock_guard<std::mutex> guard(g_transport_mutex);
#ifdef _WIN32
            if (sock_ == INVALID_SOCKET_VAL) return false;
            sent = sendto(sock_, (const char*)data.data(), (int)data.size(), 0, (struct sockaddr*)&addr, sizeof(addr));
#else
            if (sock_ < 0) return false;
            sent = sendto(sock_, (const char*)data.data(), data.size(), 0, (struct sockaddr*)&addr, sizeof(addr));
#endif
        }
        if (sent == (int)data.size()) return true;
        // backoff before retrying
        std::this_thread::sleep_for(std::chrono::milliseconds(DEFAULT_BACKOFF_MS));
    }
    return false;
}

void UdpEndpoint::receive_loop() {
    while (true) {
        {
            std::lock_guard<std::mutex> guard(g_transport_mutex);
            if (!running_) break;
        }

        uint8_t buffer[65536];
        sockaddr_in src{};
        socket_t s;
        {
            std::lock_guard<std::mutex> guard(g_transport_mutex);
            s = sock_;
        }
#ifdef _WIN32
        int slen = sizeof(src);
        int r = recvfrom(s, (char*)buffer, (int)sizeof(buffer), 0, (struct sockaddr*)&src, &slen);
#else
        socklen_t slen = sizeof(src);
        int r = recvfrom(s, (char*)buffer, sizeof(buffer), 0, (struct sockaddr*)&src, &slen);
#endif
        if (r <= 0) {
            // continue unless stop requested
            std::lock_guard<std::mutex> guard(g_transport_mutex);
            if (!running_) break;
            continue;
        }
        Payload p(buffer, buffer + r);
        try {
            SomeIpMessage msg = SomeIpMessage::deserialize(p);
            char srcip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &(src.sin_addr), srcip, INET_ADDRSTRLEN);
            Endpoint src_ep(std::string(srcip), ntohs(src.sin_port));
            Endpoint dst_ep(bind_ip_, bind_port_);

            // Copy callback under lock then invoke outside lock to avoid deadlocks
            TransportCallback cb;
            {
                std::lock_guard<std::mutex> guard(g_transport_mutex);
                cb = callback_;
            }
            if (cb) {
                try {
                    cb(msg, src_ep, dst_ep, TransportProtocol::UDP);
                } catch (const std::exception& e) {
                    log_error(std::string("Callback threw exception: ") + e.what());
                }
            }
        } catch (const std::exception& e) {
            log_error(std::string("Failed to parse SOME/IP message: ") + e.what());
        }
    }
}

} // namespace someip
