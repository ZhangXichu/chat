cmake_minimum_required(VERSION 3.20)

project(chatroom VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}")

include(${CMAKE_BINARY_DIR}/Release/generators/conan_toolchain.cmake)

find_package(uwebsockets REQUIRED)

if(CMAKE_COMPILER_IS_GNUCXX)
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(server server/server.cpp)
add_executable(client client/client.cpp)

message(STATUS "CMAKE_SOURCE_DIR: ${CMAKE_BINARY_DIR}")
message(STATUS "CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
target_include_directories(server PRIVATE ${CMAKE_SOURCE_DIR})
target_include_directories(server PRIVATE ${CMAKE_SOURCE_DIR}/server)
target_include_directories(client PRIVATE ${CMAKE_SOURCE_DIR})
target_include_directories(client PRIVATE ${CMAKE_SOURCE_DIR}/client)

message(STATUS "uwebsockets include directories: ${uwebsockets_INCLUDE_DIRS}")

target_link_libraries(server PRIVATE uwebsockets::uwebsockets)
target_include_directories(server PRIVATE ${uwebsockets_INCLUDE_DIRS})
