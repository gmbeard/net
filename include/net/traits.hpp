#ifndef NET_TRAITS_HPP_INCLUDED
#define NET_TRAITS_HPP_INCLUDED

#include <type_traits>

namespace net { namespace traits {

    using std::declval;

    namespace _private {

        template<typename... T> 
        using VoidT = void;

        template<typename T>
        struct Identity {
            using Type = T;
        };

        template<
            typename Fallback,
            typename Void,
            template <typename> class Test,
            typename... TestArgs
            >
        struct Detect : Identity<Fallback> { 
            using ValueType = std::false_type;
        };

        template<
            typename Fallback,
            template <typename> class Test,
            typename... TestArgs
            >
        struct Detect<Fallback, 
                      VoidT<Test<TestArgs...>>,
                      Test,
                      TestArgs...> :
            Identity<Test<TestArgs...>>
        {
            using ValueType = std::true_type;
        };

        struct None final {
            None() = delete;
            None(None const&) = delete;
            ~None() = delete;
            auto operator=(None const&) -> None& = delete;
        };

        template<template <typename> class Test,
                 typename... Args>
        using DetectOr = 
            typename Detect<None, void, Test, Args...>::ValueType;
    }

    template<typename T>
    using ContainerValueType = 
        typename std::remove_reference<
            decltype(*declval<T>().begin())
        >::type;

    template<typename T>
    using ContainerIteratorCategory =
        typename std::iterator_traits<
            decltype(declval<T>().begin())
        >::iterator_category;

    template<typename T>
    using HasRandomAccessIterator =
        typename std::enable_if<
            std::is_convertible<
                ContainerIteratorCategory<T>,
                std::random_access_iterator_tag
            >::value
        >::type;

    template<typename T>
    constexpr bool IsContiguousByteContainer =
        _private::DetectOr<ContainerValueType, T>::value &&
        _private::DetectOr<ContainerIteratorCategory, T>::value &&
        _private::DetectOr<HasRandomAccessIterator, T>::value;
}}
#endif //NET_TRAITS_HPP_INCLUDED
