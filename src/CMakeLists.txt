add_library(
    net
    STATIC
        tcp_socket.cpp
)

target_link_libraries(
    net
    PUBLIC
        Ip::headers
        Ip::ip
        Result::result
        Net::includes
)

add_library(
    Net::net
    ALIAS 
    net
)