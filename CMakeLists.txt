cmake_minimum_required(VERSION 3.20)

project(chatroom VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(CMAKE_COMPILER_IS_GNUCXX)
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(server server/server.cpp)
add_executable(client client/client.cpp)

message(STATUS "CMAKE_SOURCE_DIR: ${CMAKE_BINARY_DIR}")
message(STATUS "CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
target_include_directories(server PRIVATE ${CMAKE_SOURCE_DIR}/server)
target_include_directories(client PRIVATE ${CMAKE_SOURCE_DIR}/client)
