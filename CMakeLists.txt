cmake_minimum_required(VERSION 3.7)

list(APPEND
    CMAKE_MODULE_PATH 
    ${CMAKE_CURRENT_LIST_DIR}/cmake
)

list(APPEND
    CMAKE_MODULE_PATH
    ${CMAKE_CURRENT_LIST_DIR}/cmake-modules
)

list(APPEND
    CMAKE_PREFIX_PATH
    ${CMAKE_CURRENT_LIST_DIR}/deps
)

include(FindPackageDelegate)
include(InstallExternals)

project(Net CXX)

#set(LOCAL_TARGETS
#    Ip
#    Result
#)

#add_subdirectory(submodules/ip)
#add_subdirectory(submodules/result)
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
