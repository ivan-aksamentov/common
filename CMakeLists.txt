cmake_minimum_required(VERSION 3.5)

project(common)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

add_library(${PROJECT_NAME} INTERFACE)

target_compile_definitions(${PROJECT_NAME}
    INTERFACE -D${PROJECT_NAME}=1
)

target_include_directories(${PROJECT_NAME}
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(${PROJECT_NAME} INTERFACE
    ""
)
