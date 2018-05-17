cmake_minimum_required(VERSION 3.7)

set(CMAKE_MODULE_PATH 
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake
)

include(FindPackageDelegate)

project(Net CXX)

set(LOCAL_TARGETS
    Ip
    Result
)

add_subdirectory(submodules/ip)
add_subdirectory(submodules/result)
add_subdirectory(include)
add_subdirectory(src)

set(NET_ENABLE_TESTING
    OFF
    CACHE
    BOOL
    "Enable unit tests for ${PROJECT_NAME}"
)

if(NET_ENABLE_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()
