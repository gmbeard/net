#ifndef NET_TCP_SOCKET_HPP_INCLUDED
#define NET_TCP_SOCKET_HPP_INCLUDED

#include "net/sys/tcp_socket.hpp"
#include "net/detail/tests.hpp"
#include "ip/ipv4.hpp"
#include "result/result.hpp"
#include <system_error>
#include <type_traits>
#include <iterator>

namespace net {

    template<typename T>
    using SocketResult = sys::SocketResult<T>;

    template<typename T>
    constexpr bool ContiguousBytes = 
        _tests::NonNarrowConversionToByte<T>::value &&
        _tests::RandomAccessIterator<T>::value;

    namespace _private {
        struct OsHandle {
            explicit OsHandle(sys::SocketHandle) noexcept;
            auto operator*() const noexcept -> sys::SocketHandle;
        private:
            sys::SocketHandle handle_;
        };
    }

    struct TcpListener;

    struct TcpSocket {

        friend struct TcpListener;

        TcpSocket();
        TcpSocket(_private::OsHandle) noexcept;
        TcpSocket(TcpSocket&&) noexcept;
        TcpSocket(TcpSocket const&) = delete;
        ~TcpSocket();
        auto operator=(TcpSocket&&) noexcept -> TcpSocket&;
        auto operator=(TcpSocket const&) -> TcpSocket& = delete;
        auto bind(ip::IPv4 addr, uint16_t port) noexcept -> SocketResult<void>;
        auto connect(ip::IPv4 addr, uint16_t port) noexcept
            -> SocketResult<void>;
        auto listen() noexcept -> SocketResult<void>;
        auto set_nonblocking(bool) noexcept -> SocketResult<void>;
        auto set_reuseaddr(bool) noexcept -> SocketResult<void>;
        auto accept() noexcept -> SocketResult<TcpSocket>;

        template<
            typename Buffer,
            typename std::enable_if<
                ContiguousBytes<Buffer>>::type* = nullptr>
        auto read(Buffer& b) noexcept -> SocketResult<size_t> {
            return read_(&*b.begin(), b.size());
        }

        template<
            typename Buffer,
            typename std::enable_if<
                ContiguousBytes<Buffer>>::type* = nullptr>
        auto write(Buffer const& b) noexcept -> SocketResult<size_t> {
            return write_(&*b.begin(), b.size());
        }

    private:
        auto read_(uint8_t*, size_t) noexcept -> SocketResult<size_t>;
        auto write_(uint8_t const*, size_t) noexcept -> SocketResult<size_t>;

        sys::SocketHandle handle_;
    };
}
#endif //NET_TCP_SOCKET_HPP_INCLUDED

