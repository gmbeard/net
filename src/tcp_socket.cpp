#include "net/tcp_socket.hpp"
#include "net/sys/tcp_socket.hpp"

#define TRY_EXCEPT(expr) \
do { \
    auto const& e = (expr); \
    if (!e.is_ok()) { \
        throw ::std::system_error { e.error() }; \
    } \
} while (0)

#define TRY(expr) \
do { \
    auto& e = (expr); \
    if (!e.is_ok()) { \
        return ::result::err(::std::move(e.error())); \
    } \
} while (0)

using namespace net;

_private::OsHandle::OsHandle(sys::SocketHandle h) noexcept :
    handle_{h}
{ }

auto _private::OsHandle::operator*() const noexcept -> sys::SocketHandle {
    return handle_;
}

TcpSocket::TcpSocket() :
    handle_{sys::INVALID_SOCKET_VALUE}
{
    auto h = sys::create_tcp_socket();
    TRY_EXCEPT(h);
    handle_ = h.value();
}

TcpSocket::TcpSocket(_private::OsHandle h) noexcept :
    handle_{*h}
{ 
    sys::set_cloexec(handle_);
}

TcpSocket::~TcpSocket() {
    sys::close_socket(handle_);
}

TcpSocket::TcpSocket(TcpSocket&& other) noexcept :
    handle_{other.handle_}
{
    other.handle_ = sys::INVALID_SOCKET_VALUE;
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
    return sys::bind_socket(handle_, addr, port);
}

auto TcpSocket::connect(ip::IPv4 addr,
                        uint16_t port) noexcept -> SocketResult<void>
{    
    return sys::connect_socket(handle_, addr, port);
}

auto TcpSocket::listen() noexcept -> SocketResult<void> {
    return sys::listen_on_socket(handle_);
}

auto TcpSocket::set_nonblocking(bool flag) noexcept -> SocketResult<void> {
    return sys::set_nonblocking(handle_, flag);
}

auto TcpSocket::set_reuseaddr(bool flag) noexcept -> SocketResult<void> {
    return sys::set_reuseaddr(handle_, flag);
}

auto TcpSocket::accept() noexcept -> SocketResult<TcpSocket> {
    auto incoming = sys::accept_socket(handle_);
    TRY(incoming);
    return result::ok(_private::OsHandle { incoming.value() });
}

auto TcpSocket::read_(char* buffer, size_t len) noexcept
    -> SocketResult<size_t>
{
    return sys::read_socket(handle_, buffer, len);
}

auto TcpSocket::write_(char const* buffer, size_t len) noexcept
            -> SocketResult<size_t>
{
    return sys::write_socket(handle_, buffer, len);
}

