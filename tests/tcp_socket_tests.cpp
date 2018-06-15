#include "net/tcp_socket.hpp"
#include "catch.hpp"
#include <string>
#include <vector>
#include <forward_list>
#include <atomic>
#include <thread>
#include <functional>

using namespace std::literals;

TEST_CASE("TCP Sockets", "[net]") {

    SECTION("TcpSocket should bind") {
        auto s = net::TcpSocket { };
        REQUIRE(s.set_reuseaddr(true).is_ok());
        auto e = s.bind(ip::IPv4::from_string("127.0.0.1"), 
                        5050);

        REQUIRE_NOTHROW([&] {
            if (!e.is_ok()) {
                throw std::system_error{e.error()};
            }
        }());
    }

    SECTION("TcpSocket should listen") {

        auto s = net::TcpSocket { };
        REQUIRE(s.set_reuseaddr(true).is_ok());
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

TEST_CASE("Connection tests", "[net]") {

    SECTION("TcpSocket should accept a socket") {

        std::atomic<bool> flag { false };
        net::SocketResult<void> r = result::ok();
        auto addr = ip::IPv4::from_string("127.0.0.1");
        uint16_t port = 5050;
        auto t = std::thread(
            [](net::SocketResult<void>& f,
                   std::atomic<bool>& flag,
                   ip::IPv4 addr,
                   uint16_t port)
            {
                auto s = net::TcpSocket { };
                auto reuse = s.set_reuseaddr(true);
                if (!reuse.is_ok()) {
                    f = result::err(reuse.error());
                    return;
                }

                auto e = s.bind(addr, port);

                if (!e.is_ok()) {
                    f = result::err(e.error());
                    return;
                }

                e = s.listen();
                if (!e.is_ok()) {
                    f = result::err(e.error());
                    return;
                }

                auto incoming = s.accept();
                if (!incoming.is_ok()) {
                    f = result::err(e.error());
                }

                flag = true;
            },
            std::ref(r),
            std::ref(flag),
            addr,
            port);

        net::TcpSocket s { };
        auto e = s.connect(addr, port);
        t.join();

        REQUIRE_NOTHROW([&] {
            if (!r.is_ok()) {
                throw std::system_error { r.error() };
            }
        }());

        REQUIRE_NOTHROW([&] {
            if (!e.is_ok()) {
                throw std::system_error { e.error() };
            }
        }());
        REQUIRE(flag);
    }
}

TEST_CASE("Socket IO", "[net]") {

    SECTION("Should obey ContiguousBytes") {
        using Valid = std::vector<char>;
        using Invalid = std::vector<int32_t>;
        using List = std::forward_list<uint8_t>;

        using ValidIterator = 
            decltype(std::declval<Valid>().begin());
        using InvalidIterator =
            decltype(std::declval<Invalid>().begin());
        using ListIterator = 
            decltype(std::declval<List>().begin());

        REQUIRE(net::cncpts::ContiguousByteIterator<ValidIterator>);
        REQUIRE(!net::cncpts::ContiguousByteIterator<InvalidIterator>);
        REQUIRE(!net::cncpts::ContiguousByteIterator<ListIterator>);

        auto s = net::TcpSocket { };
        auto valid = Valid { };
        using ReadResult = decltype(s.write(valid.begin(), valid.end()));
    }
}
