#ifndef NET_TCP_LISTENER_HPP_INCLUDED
#define NET_TCP_LISTENER_HPP_INCLUDED

#include "net/tcp_stream.hpp"
#include "ip/ipv4.hpp"

namespace net {

    struct TcpListener {
        TcpListener(ip::IPv4, uint16_t);
        auto accept() noexcept -> SocketResult<TcpStream>;
    private:
        TcpSocket socket_;
    };
}
#endif //NET_TCP_LISTENER_HPP_INCLUDED
