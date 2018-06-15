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

    namespace cncpts {
        using namespace _tests;

        template<typename T>
        using IteratorCategory = 
            typename std::iterator_traits<T>::iterator_category;

        template<typename T>
        using IteratorValueType = 
            typename std::iterator_traits<T>::value_type;

        template<typename From, typename To>
        using NoNarrowConversionTo = 
            decltype(To { std::declval<From>() });

        template<typename T>
        constexpr bool ContiguousByteIterator =
            RequireAll<
                DetectedConvertible<
                    std::random_access_iterator_tag,
                    IteratorCategory,
                    T
                >,
                DetectedExact<
                    char,
                    IteratorValueType,
                    T
                >
            >::value;
    }

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
        auto bind(ip::IPv4 addr, uint16_t port) noexcept 
            -> SocketResult<void>;
        auto connect(ip::IPv4 addr, uint16_t port) noexcept
            -> SocketResult<void>;
        auto listen() noexcept -> SocketResult<void>;
        auto set_nonblocking(bool) noexcept -> SocketResult<void>;
        auto set_reuseaddr(bool) noexcept -> SocketResult<void>;
        auto accept() noexcept -> SocketResult<TcpSocket>;

        template<
            typename OutputIterator,
            typename std::enable_if<
                cncpts::ContiguousByteIterator<OutputIterator>>::type* = 
                    nullptr>
        auto read(OutputIterator first, OutputIterator last) noexcept 
            -> SocketResult<size_t> 
        {
            return read_(std::addressof(*first), 
                         std::distance(first, last));
        }

        template<
            typename InputIterator,
            typename std::enable_if<
                cncpts::ContiguousByteIterator<InputIterator>>::type* = 
                    nullptr>
        auto write(InputIterator first, InputIterator last) noexcept 
            -> SocketResult<size_t> 
        {
            return write_(std::addressof(*first), 
                          std::distance(first, last));
        }

    private:
        auto read_(char*, size_t) noexcept -> SocketResult<size_t>;
        auto write_(char const*, size_t) noexcept -> SocketResult<size_t>;

        sys::SocketHandle handle_;
    };
}
#endif //NET_TCP_SOCKET_HPP_INCLUDED

