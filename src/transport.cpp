#include "someip/transport.hpp"
#include "someip/someip_message.hpp"
#include <iostream>
#include <cstring>
#include <vector>

namespace someip {

#ifdef _WIN32
static bool winsock_initialized = false;
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
    recv_thread_ = std::thread(&UdpEndpoint::receive_loop, this);
    return true;
}

void UdpEndpoint::stop() {
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
    if (recv_thread_.joinable()) recv_thread_.join();
}

bool UdpEndpoint::join_multicast(const std::string& mcast_addr) {
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

    int sent;
#ifdef _WIN32
    sent = sendto(sock_, (const char*)data.data(), (int)data.size(), 0, (struct sockaddr*)&addr, sizeof(addr));
#else
    sent = sendto(sock_, (const char*)data.data(), data.size(), 0, (struct sockaddr*)&addr, sizeof(addr));
#endif
    return (sent == (int)data.size());
}

void UdpEndpoint::receive_loop() {
    while (running_) {
        uint8_t buffer[65536];
        sockaddr_in src{};
#ifdef _WIN32
        int slen = sizeof(src);
        int r = recvfrom(sock_, (char*)buffer, (int)sizeof(buffer), 0, (struct sockaddr*)&src, &slen);
#else
        socklen_t slen = sizeof(src);
        int r = recvfrom(sock_, (char*)buffer, sizeof(buffer), 0, (struct sockaddr*)&src, &slen);
#endif
        if (r <= 0) {
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
            if (callback_) callback_(msg, src_ep, dst_ep, TransportProtocol::UDP);
        } catch (const std::exception& e) {
            log_error(std::string("Failed to parse SOME/IP message: ") + e.what());
        }
    }
}

} // namespace someip