#include "net/tcp_socket.hpp"

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace net;

constexpr int INVALID_SOCKET_VALUE = -1;

TcpSocket::TcpSocket() :
    handle_{::socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)}
{
    if (handle_ == INVALID_SOCKET_VALUE) {
        throw std::system_error {
            std::error_code { errno, std::system_category() }
        };
    }
}

TcpSocket::~TcpSocket() {
    ::close(handle_);
}

TcpSocket::TcpSocket(TcpSocket&& other) noexcept :
    handle_{other.handle_}
{
    other.handle_ = INVALID_SOCKET_VALUE;
}

auto TcpSocket::operator=(TcpSocket&& other) noexcept 
    -> TcpSocket& 
{
    using std::swap;
    auto tmp = std::move(other);
    swap(handle_, tmp.handle_);
    return *this;
}

auto TcpSocket::bind(ip::IPv4 addr, 
                     uint16_t port) noexcept 
    -> SocketResult<void>
{
    sockaddr_in a = {};
    a.sin_addr.s_addr = htonl(static_cast<uint32_t>(addr));
    a.sin_port = htons(port);

    auto e = ::bind(handle_, 
                    reinterpret_cast<sockaddr*>(&a),
                    sizeof(a));
    if (e) {
        return result::err(std::error_code{errno, std::system_category()});
    }

    return result::ok();
}

auto TcpSocket::listen() noexcept -> SocketResult<void> {
    return result::err(
        std::make_error_code(std::errc::network_unreachable));
}

auto TcpSocket::set_nonblocking(bool flag) noexcept 
    -> SocketResult<void>
{
    (void)flag;
    return result::err(
        std::make_error_code(std::errc::network_unreachable));
}
