#ifndef NET_DETAIL_TESTS_HPP_INCLUDED
#define NET_DETAIL_TESTS_HPP_INCLUDED

#include <type_traits>
#include <iterator>

namespace net {

    namespace _tests {
        using std::declval;
        using std::enable_if;
        using std::is_same;
        using std::is_convertible;
        using std::iterator_traits;
        using std::true_type;
        using std::false_type;

        template<typename... T>
        using VoidT = void;

        template<typename T>
        using ByteType = 
            decltype(uint8_t { *declval<T>().begin() });

        template<typename T, typename = void>
        struct NonNarrowConversionToByte : false_type
        { };

        template<typename T>
        struct NonNarrowConversionToByte<
            T,
            VoidT<ByteType<T>>
        > : true_type
        { };

        template<typename T, typename = void>
        struct RandomAccessIterator : false_type { };

        template<typename T>
        struct RandomAccessIterator<
            T,
            VoidT<typename enable_if<
                is_convertible<
                    typename iterator_traits<decltype(declval<T>().begin())>::iterator_category,
                    std::random_access_iterator_tag
                >::value
            >::type>
        > : true_type
        { };
    }
}
#endif //NET_DETAIL_TESTS_HPP_INCLUDED
