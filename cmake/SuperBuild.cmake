include(ExternalProject)

find_package(Ip QUIET)
if(NOT Ip_FOUND)
    ExternalProject_Add(
        IpProject
        GIT_REPOSITORY https://github.com/gmbeard/ip.git
        GIT_SHALLOW ON
        BUILD_ALWAYS ON
        INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
        CMAKE_ARGS
            -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
            -DSKIP_SUPERBUILD=OFF
    )
else()
    add_custom_target(IpProject)
endif()

find_package(Result QUIET)
if(NOT Result_FOUND)
    ExternalProject_Add(
        ResultProject
        GIT_REPOSITORY https://github.com/gmbeard/result.git
        GIT_SHALLOW ON
        BUILD_ALWAYS ON
        INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
        CMAKE_ARGS
            -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
            -DINSTALL_DEPS_TO=<INSTALL_DIR>
    )
else()
    add_custom_target(ResultProject)
endif()

find_package(Catch2 QUIET)
if(NOT Catch2_FOUND)
    ExternalProject_Add(
        Catch2Project
        GIT_REPOSITORY https://github.com/CatchOrg/Catch2.git
        GIT_SHALLOW ON
        BUILD_ALWAYS ON
        INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
        CMAKE_ARGS
            -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
    )
else()
    add_custom_target(Catch2Project)
endif()

ExternalProject_Add(
    NetProject
    DEPENDS 
        IpProject 
        ResultProject 
        Catch2Project
    SOURCE_DIR ${PROJECT_SOURCE_DIR}
    BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}
    INSTALL_COMMAND ""
    INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
    CMAKE_ARGS
        -DCMAKE_PREFIX_PATH=<INSTALL_DIR>
        -DSKIP_SUPERBUILD=ON
)
set(SKIP_SUPERBUILD ON)
