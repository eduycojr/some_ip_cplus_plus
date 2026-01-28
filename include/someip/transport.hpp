#ifndef SOMEIP_TRANSPORT_HPP
#define SOMEIP_TRANSPORT_HPP

#include "types.hpp"
#include "someip_message.hpp"
#include <functional>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>

#ifdef _WIN32
  #ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
  #endif
  #include <winsock2.h>
  #include <ws2tcpip.h>
  using socket_t = SOCKET;
  using ssize_t = int;  // Windows doesn't have ssize_t
  #define INVALID_SOCKET_VAL INVALID_SOCKET
#else
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <arpa/inet.h>
  #include <unistd.h>
  using socket_t = int;
  #define INVALID_SOCKET_VAL (-1)
#endif

namespace someip {

// Callback invoked when a full SOME/IP message is received
using TransportCallback = std::function<void(const SomeIpMessage&, const Endpoint& src, const Endpoint& dst, TransportProtocol proto)>;

// Simple UDP endpoint supporting multicast listening and sendto
class UdpEndpoint {
public:
    UdpEndpoint(const std::string& bind_ip, uint16_t bind_port);
    ~UdpEndpoint();

    // Start listening (spawn receive thread)
    bool start();

    // Stop listening
    void stop();

    // Send raw bytes to dest (ip,port)
    bool send_to(const Payload& data, const Endpoint& dest);

    // Join multicast group
    bool join_multicast(const std::string& mcast_addr);

    // Leave multicast group
    bool leave_multicast(const std::string& mcast_addr);

    void set_callback(TransportCallback cb) { callback_ = std::move(cb); }

    std::string local_ip() const { return bind_ip_; }
    uint16_t local_port() const { return bind_port_; }

private:
    void receive_loop();

    std::string bind_ip_;
    uint16_t bind_port_;
    socket_t sock_ = INVALID_SOCKET_VAL;
    TransportCallback callback_;
    std::thread recv_thread_;
    std::atomic<bool> running_{false};
};

} // namespace someip

#endif // SOMEIP_TRANSPORT_HPP