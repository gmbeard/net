#ifndef NET_SYS_TCP_SOCKET_HPP_INCLUDED
#define NET_SYS_TCP_SOCKET_HPP_INCLUDED

#include "ip/ipv4.hpp"
#include "result/result.hpp"

#ifdef _WIN32
# include <winsock2.h>
# include <ws2tcpip.h>
# include <windows.h>
#else
# include <sys/socket.h>
# include <sys/types.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <fcntl.h>
#endif

namespace net { namespace sys {

#ifdef _WIN32
using SocketHandle = SOCKET;
constexpr SocketHandle INVALID_SOCKET_VALUE = INVALID_SOCKET;
#else
using SocketHandle = int;
constexpr SocketHandle INVALID_SOCKET_VALUE = -1;
#endif

template<typename T>
using SocketResult = 
    result::Result<T, std::error_code>;

auto set_cloexec(SocketHandle handle) noexcept -> SocketResult<void>;
auto create_tcp_socket() noexcept -> SocketResult<SocketHandle>;
auto close_socket(SocketHandle) noexcept -> void;
auto bind_socket(SocketHandle, ip::IPv4, uint16_t) noexcept 
    -> SocketResult<void>;
auto connect_socket(SocketHandle, ip::IPv4, uint16_t) noexcept 
    -> SocketResult<void>;
auto listen_on_socket(SocketHandle) noexcept -> SocketResult<void>;
auto set_nonblocking(SocketHandle, bool) noexcept -> SocketResult<void>;
auto set_reuseaddr(SocketHandle, bool) noexcept -> SocketResult<void>;
auto accept_socket(SocketHandle) noexcept -> SocketResult<SocketHandle>;
auto read_socket(SocketHandle, uint8_t*, size_t) noexcept
    -> SocketResult<size_t>;
auto write_socket(SocketHandle, uint8_t const*, size_t) noexcept
            -> SocketResult<size_t>;
}}
#endif //NET_SYS_TCP_SOCKET_HPP_INCLUDED
