find_package(Catch2 REQUIRED)

add_executable(
    tests
        main.cpp
        tcp_socket_tests.cpp
)

target_link_libraries(
    tests
    PRIVATE
        Net::net
        Catch2::Catch
)

add_test(
    NAME
        netTests
    COMMAND
        tests -s
)
