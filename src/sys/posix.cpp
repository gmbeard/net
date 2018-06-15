#include "net/sys/tcp_socket.hpp"
#include <system_error>

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

auto net::sys::set_cloexec(SocketHandle handle) noexcept 
    -> SocketResult<void>
{    
    auto f = ::fcntl(handle, F_GETFD);
    CHECK_FCNTL_RESULT(f);
    f |= FD_CLOEXEC;
    CHECK_FCNTL_RESULT(::fcntl(handle, F_SETFD, f));
    return result::ok();
}

auto net::sys::create_tcp_socket() noexcept 
    -> SocketResult<SocketHandle>
{
    auto h = ::socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    CHECK_HANDLE_RESULT(h);
    return result::ok(h);
}

auto net::sys::close_socket(SocketHandle h) noexcept -> void {
    ::close(h);
}

auto net::sys::bind_socket(SocketHandle h, 
                           ip::IPv4 addr, 
                           uint16_t port) noexcept 
    -> SocketResult<void>
{    
    sockaddr_in a = {};
    a.sin_addr.s_addr = htonl(static_cast<uint32_t>(addr));
    a.sin_port = htons(port);

    auto e = ::bind(h, 
                    reinterpret_cast<sockaddr*>(&a),
                    sizeof(a));

    CHECK_SOCKET_RESULT(e);
    return result::ok();
}

auto net::sys::connect_socket(SocketHandle h, 
                              ip::IPv4 addr, 
                              uint16_t port) noexcept 
    -> SocketResult<void>
{    
    sockaddr_in a = {};
    a.sin_addr.s_addr = htonl(static_cast<uint32_t>(addr));
    a.sin_port = htons(port);

    auto e = ::connect(h, 
                       reinterpret_cast<sockaddr*>(&a),
                       sizeof(a));

    CHECK_SOCKET_RESULT(e);
    return result::ok();
}

auto net::sys::listen_on_socket(SocketHandle h) noexcept 
    -> SocketResult<void>
{    
    auto e = ::listen(h, SOMAXCONN);
    CHECK_SOCKET_RESULT(e);
    return result::ok();
}

auto net::sys::set_nonblocking(SocketHandle h, 
                               bool flag) noexcept 
    -> SocketResult<void>
{    
    auto f = ::fcntl(h, F_GETFL);
    CHECK_FCNTL_RESULT(f);

    if (flag) {
        f |= O_NONBLOCK;
    }
    else {
        f &= ~O_NONBLOCK;
    }

    f = ::fcntl(h, F_SETFL, f);
    CHECK_FCNTL_RESULT(f);

    return result::ok();
}

auto net::sys::set_reuseaddr(SocketHandle h, 
                             bool flag) noexcept 
    -> SocketResult<void>
{    
    int val = flag ? 1 : 0;
    auto e = ::setsockopt(h,
                          SOL_SOCKET,
                          SO_REUSEADDR,
                          &val,
                          sizeof(val));
    CHECK_SOCKET_RESULT(e);
    return result::ok();
}

auto net::sys::accept_socket(SocketHandle h) noexcept 
    -> SocketResult<SocketHandle>
{    
    auto incoming = ::accept(h, nullptr, nullptr);
    CHECK_HANDLE_RESULT(incoming);
    return result::ok(incoming);
}

auto net::sys::read_socket(SocketHandle h, 
                           char* buffer, 
                           size_t len) noexcept
    -> SocketResult<size_t>
{    
    auto n = ::read(h, buffer, len);
    CHECK_IO_RESULT(n);
    return result::ok(static_cast<size_t>(n));
}

auto net::sys::write_socket(SocketHandle h, 
                            char const* buffer, 
                            size_t len) noexcept
            -> SocketResult<size_t>
{    
    auto n = ::write(h, buffer, len);
    CHECK_IO_RESULT(n);
    return result::ok(static_cast<size_t>(n));
}
