#include "net/tcp_socket.hpp"
#include "catch.hpp"
#include <string>

using namespace std::literals;

TEST_CASE("TCP Sockets", "[net]") {

    SECTION("TcpSocket should construct") {
        auto s = net::TcpSocket{};

        auto e = s.bind(ip::IPv4::from_string("127.0.0.1"), 
                        5050);

        REQUIRE_NOTHROW([&] {
            if (!e.is_ok()) {
                throw std::system_error{e.error()};
            }
        }());
    }
}
