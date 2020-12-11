cmake_minimum_required(VERSION "3.19.0")

project("server")

INCLUDE_DIRECTORIES(../shared/socket)

add_executable("${PROJECT_NAME}" "server.cpp")

target_link_libraries("${PROJECT_NAME}" SimpleSocket)
