#ifndef NET_TCP_STREAM_HPP_INCLUDE
#define NET_TCP_STREAM_HPP_INCLUDE

#include "ip/ipv4.hpp"
#include "net/tcp_socket.hpp"

namespace net {

    struct TcpStream {
        TcpStream(TcpSocket&&) noexcept;

        template<typename Buffer>
        auto read(Buffer& b) noexcept -> SocketResult<size_t> {
            return socket_.read(b);
        }
        
        template<typename Buffer>
        auto write(Buffer const& b) noexcept
            -> SocketResult<size_t>
        {
            return socket_.write(b);
        }

    private:
        TcpSocket socket_;
    };
}
#endif //NET_TCP_STREAM_HPP_INCLUDE
