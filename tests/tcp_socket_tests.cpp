#include "net/tcp_socket.hpp"
#include "catch.hpp"
#include <string>
#include <vector>
#include <forward_list>

using namespace std::literals;

TEST_CASE("TCP Sockets", "[net]") {

    SECTION("TcpSocket should bind") {
        auto s = net::TcpSocket{};
        auto e = s.bind(ip::IPv4::from_string("127.0.0.1"), 
                        5050);

        REQUIRE_NOTHROW([&] {
            if (!e.is_ok()) {
                throw std::system_error{e.error()};
            }
        }());
    }

    SECTION("TcpSocket should listen") {

        auto s = net::TcpSocket{};
        auto e = s.bind(ip::IPv4::from_string("127.0.0.1"), 
                        5050);

        REQUIRE(e.is_ok());
        e = s.listen();

        REQUIRE_NOTHROW([&] {
            if (!e.is_ok()) {
                throw std::system_error{e.error()};
            }
        }());
    }

    SECTION("TcpSocket should set non-blocking") {
        
        auto s = net::TcpSocket{};
        auto e = s.set_nonblocking(true);

        REQUIRE_NOTHROW([&] {
            if (!e.is_ok()) {
                throw std::system_error{e.error()};
            }
        }());
    }
}

// TODO(GB):
//  Make this test automated
TEST_CASE("Connection tests", "[.net]") {

    SECTION("TcpSocket should accept a socket") {

        auto s = net::TcpSocket{};
        auto e = s.bind(ip::IPv4::from_string("127.0.0.1"), 
                        5050);

        REQUIRE(e.is_ok());
        e = s.listen();
        REQUIRE(e.is_ok());

        auto incoming = s.accept();

        REQUIRE_NOTHROW([&] {
            if (!incoming.is_ok()) {
                throw std::system_error{e.error()};
            }
        }());
    }
}

// TODO(GB):
//  Make this test automated
TEST_CASE("Socket IO", "[.net]") {

    SECTION("Should obey ContiguousBytes") {
        using Valid = std::vector<uint8_t>;
        using Invalid = std::vector<int32_t>;

        REQUIRE(net::ContiguousBytes<Valid>);
        REQUIRE(!net::ContiguousBytes<Invalid>);

        Valid v(128);

        auto s = net::TcpSocket{};
        REQUIRE(s.read(v).is_ok());
        REQUIRE(s.write(v).is_ok());
    }
}
