cmake_minimum_required(VERSION "3.19.0")

project("client")

INCLUDE_DIRECTORIES(../shared/socket)

add_executable("${PROJECT_NAME}" "client.cpp")

target_link_libraries("${PROJECT_NAME}" SimpleSocket)
