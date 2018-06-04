find_package(Ip REQUIRED)
find_package(Result REQUIRED)

set(net_sources
    tcp_socket.cpp
    tcp_stream.cpp
    tcp_listener.cpp
)

if(NOT WIN32)
    list(APPEND 
        net_sources
            sys/posix.cpp
    )
else()
    list(APPEND
        net_sources
            sys/windows.cpp
    )
endif()

add_library(
    net
    STATIC
        ${net_sources}
)

target_link_libraries(
    net
    PUBLIC
        Ip::headers
        Ip::ip
        Result::result
        Net::includes
)

if(WIN32)
    target_link_libraries(
        net
        PUBLIC
            Ws2_32
    )
endif()

add_library(
    Net::net
    ALIAS 
    net
)

install(
    TARGETS
        net
    EXPORT NetExports
    ARCHIVE DESTINATION lib
)
