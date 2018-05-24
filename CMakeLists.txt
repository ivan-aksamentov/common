cmake_minimum_required(VERSION 3.5)

project(common)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

set(public_include_dirs
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set(public_headers
    include/common/debug.h
    include/common/ieee754_comparison.h
    include/common/macro_overload.h
    include/common/random.h
)

set(headers
    ${public_headers}
)

add_library(${PROJECT_NAME} INTERFACE)
add_custom_target(_${PROJECT_NAME} SOURCES ${headers})

set(${PROJECT_NAME}_INCLUDE_DIRS
    ${public_include_dirs}
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)
