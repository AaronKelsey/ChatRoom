cmake_minimum_required(VERSION "3.19.0")

project(ChatRoom VERSION 1.0.0 DESCRIPTION "TCP ChatRoom")

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(shared/socket)
add_subdirectory(server)
add_subdirectory(client)
