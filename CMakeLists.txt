cmake_minimum_required(VERSION 3.7)

list(APPEND
    CMAKE_MODULE_PATH 
    ${CMAKE_CURRENT_LIST_DIR}/cmake
)

project(Net CXX)

option(SKIP_SUPERBUILD
    "SuperBuild for ${PROJECT_NAME}"
    OFF
)

option(NET_ENABLE_TESTING
    "Enable unit tests for ${PROJECT_NAME}"
    OFF
)

if(NOT SKIP_SUPERBUILD)
    include(SuperBuild)
    return()
endif()

add_subdirectory(include)
add_subdirectory(src)

if(NET_ENABLE_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()

install(
    EXPORT NetExports
    NAMESPACE Net::
    FILE NetTargets.cmake
    DESTINATION lib/cmake/net
)

install(
    FILES
        cmake/NetConfig.cmake
    DESTINATION
        lib/cmake/net
)
