cmake_minimum_required(VERSION "3.19.0")

project(SimpleSocket VERSION 1.0.0 DESCRIPTION "Simple socket library to wrap Winsock2")

add_library(SimpleSocket SHARED
			socket.cpp
			socket.h)

set_target_properties(SimpleSocket PROPERTIES VERSION ${PROJECT_VERSION})

set_target_properties(SimpleSocket PROPERTIES PUBLIC_HEADER socket.h)

add_compile_definitions(SIMPLESOCKET_EXPORT)

include(GNUInstallDirs)

set(CMAKE_INSTALL_LIBDIR ../../build)
set(CMAKE_INSTALL_INCLUDEDIR ../../build)

install(TARGETS SimpleSocket
		LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
		PUBLIC_HEADER DESTINATION ${CMAKE_INCLUDE_INCLUDEDIR})

if(WIN32)
	target_link_libraries("${PROJECT_NAME}" wsock32 ws2_32)
endif()
