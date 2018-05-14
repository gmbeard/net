#ifndef NET_TCP_SOCKET_HPP_INCLUDED
#define NET_TCP_SOCKET_HPP_INCLUDED

#include "ip/ipv4.hpp"
#include "result/result.hpp"
#include <system_error>

namespace net {

    template<typename T>
    using SocketResult = 
        result::Result<T, std::error_code>;

    struct TcpSocket {

        TcpSocket();
        TcpSocket(TcpSocket&&) noexcept;
        TcpSocket(TcpSocket const&) = delete;
        ~TcpSocket();
        auto operator=(TcpSocket&&) noexcept -> TcpSocket&;
        auto operator=(TcpSocket const&) -> TcpSocket& = delete;
        auto bind(ip::IPv4 addr, uint16_t port) noexcept ->
            SocketResult<void>;
        auto listen() noexcept -> SocketResult<void>;
        auto set_nonblocking(bool) noexcept 
            -> SocketResult<void>;

    private:
        int handle_;
    };
}
#endif //NET_TCP_SOCKET_HPP_INCLUDED

