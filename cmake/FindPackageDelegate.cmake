if(NOT FIND_PACKAGE_DELEGATED)
    macro(find_package)
        if(NOT ${ARGV0} IN_LIST LOCAL_TARGETS)
            _find_package(${ARGV})
        endif()
    endmacro(find_package)
else()
    message(STATUS "find_package already delegated. Ignoring.")
endif()

set(FIND_PACKAGE_DELEGATED
    ON
)
