#ifndef NET_TCP_STREAM_HPP_INCLUDE
#define NET_TCP_STREAM_HPP_INCLUDE

#include "ip/ipv4.hpp"
#include "net/tcp_socket.hpp"

namespace net {

    struct TcpStream {
        TcpStream(TcpSocket&&) noexcept;

        template<typename OutputIterator>
        auto read(OutputIterator first, OutputIterator last) noexcept 
            -> SocketResult<size_t> 
        {
            return socket_.read(first, last);
        }
        
        template<typename InputIterator>
        auto write(InputIterator first, InputIterator last) noexcept
            -> SocketResult<size_t>
        {
            return socket_.write(first, last);
        }

    private:
        TcpSocket socket_;
    };
}
#endif //NET_TCP_STREAM_HPP_INCLUDE
