#include "net/tcp_socket.hpp"

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

using namespace net;

constexpr int INVALID_SOCKET_VALUE = -1;

#define CHECK_FCNTL_RESULT(f) \
do { \
    if (0 > f) { \
        return ::result::err(::std::error_code { \
            errno, \
            ::std::system_category() \
        }); \
    } \
} while (0)

#define THROWS_FCNTL_RESULT(f) \
do { \
    if (0 > (f)) { \
        throw ::std::system_error { \
            ::std::error_code { \
                errno, \
                ::std::system_category() \
            } \
        }; \
    } \
} while (0)

#define THROWS_HANDLE_RESULT(e) THROWS_FCNTL_RESULT(e)

#define CHECK_SOCKET_RESULT(e) \
do { \
    if (e) { \
        return ::result::err(::std::error_code { \
            errno, \
            ::std::system_category() \
        }); \
    } \
} while (0)

#define CHECK_HANDLE_RESULT(e) CHECK_FCNTL_RESULT(e)
#define CHECK_IO_RESULT(e) CHECK_FCNTL_RESULT(e)

auto set_cloexec(int handle) {
    auto f = ::fcntl(handle, F_GETFD);
    THROWS_FCNTL_RESULT(f);
    f |= FD_CLOEXEC;
    THROWS_FCNTL_RESULT(::fcntl(handle, F_SETFD, f));
}

_private::OsHandle::OsHandle(int h) noexcept :
    handle_{h}
{ }

auto _private::OsHandle::operator*() const noexcept -> int {
    return handle_;
}

TcpSocket::TcpSocket() :
    handle_{::socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)}
{
    THROWS_HANDLE_RESULT(handle_);
    set_cloexec(handle_);
}

TcpSocket::TcpSocket(_private::OsHandle h) noexcept :
    handle_{*h}
{ 
    set_cloexec(handle_);
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

    CHECK_SOCKET_RESULT(e);
    return result::ok();
}

auto TcpSocket::connect(ip::IPv4 addr,
                        uint16_t port) noexcept -> SocketResult<void>
{    
    sockaddr_in a = {};
    a.sin_addr.s_addr = htonl(static_cast<uint32_t>(addr));
    a.sin_port = htons(port);

    auto e = ::connect(handle_, 
                       reinterpret_cast<sockaddr*>(&a),
                       sizeof(a));

    CHECK_SOCKET_RESULT(e);
    return result::ok();
}

auto TcpSocket::listen() noexcept -> SocketResult<void> {
    auto e = ::listen(handle_, SOMAXCONN);
    CHECK_SOCKET_RESULT(e);
    return result::ok();
}

auto TcpSocket::set_nonblocking(bool flag) noexcept -> SocketResult<void> {
    auto f = ::fcntl(handle_, F_GETFL);
    CHECK_FCNTL_RESULT(f);

    if (flag) {
        f |= O_NONBLOCK;
    }
    else {
        f &= ~O_NONBLOCK;
    }

    f = ::fcntl(handle_, F_SETFL, f);
    CHECK_FCNTL_RESULT(f);

    return result::ok();
}

auto TcpSocket::set_reuseaddr(bool flag) noexcept -> SocketResult<void> {
    int val = flag ? 1 : 0;
    auto e = ::setsockopt(handle_,
                          SOL_SOCKET,
                          SO_REUSEADDR,
                          &val,
                          sizeof(val));
    CHECK_SOCKET_RESULT(e);
    return result::ok();
}

auto TcpSocket::accept() noexcept -> SocketResult<TcpSocket> {
    auto incoming = ::accept(handle_, nullptr, nullptr);
    CHECK_HANDLE_RESULT(incoming);
    return result::ok(_private::OsHandle { incoming });
}

auto TcpSocket::read_(uint8_t* buffer, size_t len) noexcept
    -> SocketResult<size_t>
{
    auto n = ::read(handle_, buffer, len);
    CHECK_IO_RESULT(n);
    return result::ok(static_cast<size_t>(n));
}

auto TcpSocket::write_(uint8_t const* buffer, size_t len) noexcept
            -> SocketResult<size_t>
{
    auto n = ::write(handle_, buffer, len);
    CHECK_IO_RESULT(n);
    return result::ok(static_cast<size_t>(n));
}

