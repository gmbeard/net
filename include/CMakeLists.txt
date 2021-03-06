add_library(
    includes
    INTERFACE
)

target_include_directories(
    includes
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
        $<TARGET_PROPERTY:Ip::headers,INTERFACE_INCLUDE_DIRECTORIES>
        $<TARGET_PROPERTY:Result::result,INTERFACE_INCLUDE_DIRECTORIES>
)

target_compile_features(
    includes
    INTERFACE
        cxx_decltype_auto
        $<TARGET_PROPERTY:Ip::headers,INTERFACE_COMPILE_FEATURES>
        $<TARGET_PROPERTY:Result::result,INTERFACE_COMPILE_FEATURES>
)

target_compile_options(
    includes
    INTERFACE
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Werror -Wextra>
        $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
        # TODO(GB): Result contains a throw in a (possibly)
        #   noexcept function. Sort this MSVC compiler warning!
        #   Issue #: https://github.com/gmbeard/result/issues/1
        $<$<CXX_COMPILER_ID:MSVC>:/wd4297>
        $<TARGET_PROPERTY:Ip::headers,INTERFACE_COMPILE_OPTIONS>
        $<TARGET_PROPERTY:Result::result,INTERFACE_COMPILE_OPTIONS>
)

install(
    DIRECTORY net
    DESTINATION include
)

install(
    TARGETS 
        includes
    EXPORT NetExports
)

add_library(
    Net::includes
    ALIAS includes
)
