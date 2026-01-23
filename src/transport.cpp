#include "someip/transport.hpp"
#include "someip/someip_message.hpp"
#include <iostream>
#include <cstring>
#include <vector>

namespace someip {

#ifdef _WIN32
  #include <winsock2.h>
  #include <ws2tcpip.h>
  // Link to ws2_32 in CMake (see CMakeLists.txt)
  static bool winsock_initialized = false;
#endif

#ifndef _WIN32
inline int close_socket(int s) { return close(s); }
#endif

UdpEndpoint::UdpEndpoint(const std::string& bind_ip, uint16_t bind_port)
    : bind_ip_(bind_ip), bind_port_(bind_port) {}

UdpEndpoint::~UdpEndpoint() {
    stop();
}

bool UdpEndpoint::start() {
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
    if (
#ifdef _WIN32
        sock_ == INVALID_SOCKET
#else
        sock_ < 0
#endif
    ) {
        log_error("socket() failed");
        return false;
    }

    int reuse = 1;
#ifdef _WIN32
    setsockopt(sock_, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse));
#else
    setsockopt(sock_, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
#endif

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(bind_port_);
    addr.sin_addr.s_addr = inet_addr(bind_ip_.c_str());

    if (bind(sock_, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        log_error("bind() failed");
#ifdef _WIN32
        closesocket(sock_);
        sock_ = INVALID_SOCKET;
#else
        close(sock_);
        sock_ = -1;
#endif
        return false;
    }

    running_ = true;
    recv_thread_ = std::thread(&UdpEndpoint::receive_loop, this);
    return true;
}

void UdpEndpoint::stop() {
    if (!running_) return;
    running_ = false;
#ifdef _WIN32
    if (sock_ != INVALID_SOCKET) {
        closesocket(sock_);
        sock_ = INVALID_SOCKET;
    }
#else
    if (sock_ >= 0) {
        ::close(sock_);
        sock_ = -1;
    }
#endif
    if (recv_thread_.joinable()) recv_thread_.join();

#ifdef _WIN32
    // Note: Do not call WSACleanup per-socket; it's safe to leave it until process end.
    // Optionally call WSACleanup() at program shutdown if desired.
#endif
}

bool UdpEndpoint::join_multicast(const std::string& mcast_addr) {
    struct ip_mreq mreq;
    mreq.imr_multiaddr.s_addr = inet_addr(mcast_addr.c_str());
    mreq.imr_interface.s_addr = inet_addr(bind_ip_.c_str());
    if (setsockopt(sock_, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mreq, sizeof(mreq)) < 0) {
        log_error("setsockopt IP_ADD_MEMBERSHIP failed");
        return false;
    }
    return true;
}

bool UdpEndpoint::leave_multicast(const std::string& mcast_addr) {
    struct ip_mreq mreq;
    mreq.imr_multiaddr.s_addr = inet_addr(mcast_addr.c_str());
    mreq.imr_interface.s_addr = inet_addr(bind_ip_.c_str());
    setsockopt(sock_, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char*)&mreq, sizeof(mreq));
    return true;
}

bool UdpEndpoint::send_to(const Payload& data, const Endpoint& dest) {
    std::string ip; uint16_t port;
    std::tie(ip, port) = dest;
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip.c_str());

    ssize_t sent;
#ifdef _WIN32
    sent = sendto(sock_, (const char*)data.data(), (int)data.size(), 0, (struct sockaddr*)&addr, sizeof(addr));
#else
    sent = sendto(sock_, (const char*)data.data(), data.size(), 0, (struct sockaddr*)&addr, sizeof(addr));
#endif
    return (sent == (ssize_t)data.size());
}

void UdpEndpoint::receive_loop() {
    while (running_) {
        uint8_t buffer[65536];
        sockaddr_in src;
#ifdef _WIN32
        int slen = sizeof(src);
        int r = recvfrom(sock_, (char*)buffer, (int)sizeof(buffer), 0, (struct sockaddr*)&src, &slen);
#else
        socklen_t slen = sizeof(src);
        ssize_t r = recvfrom(sock_, (char*)buffer, sizeof(buffer), 0, (struct sockaddr*)&src, &slen);
#endif
        if (r <= 0) {
            if (!running_) break;
            continue;
        }
        Payload p(buffer, buffer + r);
        try {
            SomeIpMessage msg = SomeIpMessage::deserialize(p);
            char srcip[INET_ADDRSTRLEN];
#ifdef _WIN32
            inet_ntop(AF_INET, &(src.sin_addr), srcip, INET_ADDRSTRLEN);
#else
            inet_ntop(AF_INET, &(src.sin_addr), srcip, INET_ADDRSTRLEN);
#endif
            Endpoint src_ep(std::string(srcip), ntohs(src.sin_port));
            Endpoint dst_ep(bind_ip_, bind_port_);
            if (callback_) callback_(msg, src_ep, dst_ep, TransportProtocol::UDP);
        } catch (const std::exception& e) {
            log_error(std::string("Failed to parse SOME/IP message: ") + e.what());
        }
    }
}

} // namespace someip