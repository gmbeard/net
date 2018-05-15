#include "net/tcp_listener.hpp"

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

TcpListener::TcpListener(ip::IPv4 addr, uint16_t port) {
    TRY_EXCEPT(socket_.bind(addr, port));
    TRY_EXCEPT(socket_.listen());
}

auto TcpListener::accept() noexcept -> SocketResult<TcpStream> {
    auto r = socket_.accept();
    TRY(r);
    return result::ok(std::move(r.value()));
}
