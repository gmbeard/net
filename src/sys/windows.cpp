#include "net/sys/tcp_socket.hpp"
#include <system_error>
#include <mutex>
#include <cstdlib>

#define CHECK_FCNTL_RESULT(f) \
do { \
    if (0 > f) { \
        return ::result::err(::std::error_code { \
            ::WSAGetLastError(), \
            ::std::system_category() \
        }); \
    } \
} while (0)

#define THROWS_FCNTL_RESULT(f) \
do { \
    if (0 > (f)) { \
        throw ::std::system_error { \
            ::std::error_code { \
                ::WSAGetLastError(), \
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
            ::WSAGetLastError(), \
            ::std::system_category() \
        }); \
    } \
} while (0)

#define CHECK_HANDLE_RESULT(e) CHECK_FCNTL_RESULT(e)
#define CHECK_IO_RESULT(e) CHECK_FCNTL_RESULT(e)

std::once_flag winsock_init;

auto teardown_winsock() -> void {
    ::WSACleanup();
}

auto net::sys::set_cloexec(SocketHandle) noexcept 
    -> SocketResult<void>
{    
    // TODO(GB):
    //  How to set CLOEXEC in Windows?...
    return result::ok();
}

auto net::sys::create_tcp_socket() noexcept 
    -> SocketResult<SocketHandle>
{
    std::call_once(
        winsock_init,
        [] {
            WSADATA wsa_data;;
            WORD version = MAKEWORD(2, 2);
            auto err = ::WSAStartup(version, &wsa_data);
            if (err != 0) {
                auto ec = std::error_code(err, std::system_category());
                throw std::system_error(ec);
            }

            std::atexit(teardown_winsock);
        }
    );

    auto h = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    CHECK_HANDLE_RESULT(h);
    return result::ok(h);
}

auto net::sys::close_socket(SocketHandle h) noexcept -> void {
    ::closesocket(h);
}

auto net::sys::bind_socket(SocketHandle h, 
                           ip::IPv4 addr, 
                           uint16_t port) noexcept 
    -> SocketResult<void>
{    
    sockaddr_in a = {};
    a.sin_family = AF_INET;
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
    a.sin_family = AF_INET;
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
    u_long enabled = flag ? 1 : 0;
    auto f = ::ioctlsocket(h, FIONBIO, &enabled);
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
                          reinterpret_cast<char const*>(&val),
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
    DWORD bytes_read = 0;
    WSABUF b[] = { 
        { len, buffer }
    };

    auto e = ::WSARecv(h,
                       &b[0],
                       1,
                       &bytes_read,
                       0,
                       nullptr,
                       nullptr);
    CHECK_SOCKET_RESULT(e);

    return result::ok(static_cast<size_t>(bytes_read));
}

auto net::sys::write_socket(SocketHandle h, 
                            char const* buffer, 
                            size_t len) noexcept
            -> SocketResult<size_t>
{    
    DWORD bytes_written = 0;
    WSABUF b[] = { 
        { len, const_cast<char*>(buffer) }
    };

    auto e =  ::WSASend(h,
                        &b[0],
                        1,
                        &bytes_written,
                        0,
                        nullptr,
                        nullptr);
    CHECK_SOCKET_RESULT(e);

    return result::ok(static_cast<size_t>(bytes_written));;
}
