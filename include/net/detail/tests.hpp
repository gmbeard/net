#ifndef NET_DETAIL_TESTS_HPP_INCLUDED
#define NET_DETAIL_TESTS_HPP_INCLUDED

#include <type_traits>
#include <iterator>

namespace net {

    namespace _tests {
        template<typename... T>
        using VoidT = void;

        struct NoneSuch {
            NoneSuch() = delete;
            NoneSuch(NoneSuch const&) = delete;
            ~NoneSuch() = delete;
            NoneSuch& operator=(NoneSuch const&) = delete;
        };

        template<typename T, 
                 typename VoidType,
                 template <class...> class Test,
                 typename... TestArgs>
        struct Detector {
            using value = std::false_type;
            using type = T;
        };

        template<typename T,
                 template <class...> class Test,
                 typename... Args>
        struct Detector<T, VoidT<Test<Args...>>, Test, Args...> {
            using value = std::true_type;
            using type = Test<Args...>;
        };

        template<template <class...> class Test, typename... Args>
        using DetectedValue = 
            typename Detector<NoneSuch, void, Test, Args...>::value;

        template<template <class...> class Test, typename... Args>
        using DetectedType =
            typename Detector<NoneSuch, void, Test, Args...>::type;

        template<template <class...> class Test, typename... Args>
        constexpr bool IS_DETECTED =
            DetectedValue<Test, Args...>::value;

        template<typename Required, template <class...> class Test, typename... Args>
        using DetectedExact =
            std::is_same<Required, DetectedType<Test, Args...>>;

        template<typename Required, template <class...> class Test, typename... Args>
        constexpr bool IS_DETECTED_EXACT =
            DetectedExact<Required, Test, Args...>::value;

        template<typename To, template <class...> class Test, typename... Args>
        using DetectedConvertible = 
            std::is_convertible<DetectedType<Test, Args...>, To>;

        template<typename To, template <class...> class Test, typename... Args>
        constexpr bool IS_DETECTED_CONVERTIBLE =
            DetectedConvertible<To, Test, Args>::value;

        template<typename Default, template <class...> class Test, typename... Args>
        using DetectedOr = Detector<Default, void, Test, Args...>;

        template<typename Default, template <class...> class Test, typename... Args>
        using DetectedOrType = typename DetectedOr<Default, Test, Args...>::type;

        template<typename... Condition>
        struct RequireAll;

        template<typename First, typename... Others>
        struct RequireAll<First, Others...> {
            static constexpr bool value = 
                First::value && RequireAll<Others...>::value;
        };

        template<>
        struct RequireAll<> {
            static constexpr bool value = true;
        };

        template<typename... Conditions>
        struct RequireAny;

        template<typename First, typename... Others>
        struct RequireAny<First, Others...> {
            static constexpr bool value = 
                First::value || RequireAny<Others...>::value;
        };

        template<>
        struct RequireAny<> {
            static constexpr bool value = false;
        };
    }
}
#endif //NET_DETAIL_TESTS_HPP_INCLUDED
