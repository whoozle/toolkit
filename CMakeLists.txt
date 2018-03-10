cmake_minimum_required(VERSION 2.6)
include(CheckCXXCompilerFlag)

set (CMAKE_CXX_STANDARD 11)
find_package( Threads )

check_cxx_compiler_flag(-Wall HAS_WALL)
if (HAS_WALL)
	add_definitions(-Wall)
endif()

include_directories(src)

if (NOT DEFINED TOOLKIT_NS)
	set(TOOLKIT_NS toolkit)
endif()

message(STATUS "toolkit namespace: ${TOOLKIT_NS}")

set(TOOLKIT_CONFIG_DIR ${CMAKE_CURRENT_BINARY_DIR}/toolkit/core)
file(MAKE_DIRECTORY ${TOOLKIT_CONFIG_DIR})
configure_file(src/toolkit/core/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/toolkit/core/config.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(TOOLKIT_SRC
	src/toolkit/log/Logger.cpp
	src/toolkit/log/ConsoleLoggingSink.cpp

	src/toolkit/io/SystemException.cpp
	src/toolkit/io/File.cpp
	src/toolkit/io/Poll.cpp
	src/toolkit/io/TemporaryFile.cpp

	src/toolkit/net/bsd/Socket.cpp

	src/toolkit/net/ipv4/Address.cpp
	src/toolkit/net/ipv4/Endpoint.cpp
	src/toolkit/net/ipv4/TCPServerSocket.cpp
	src/toolkit/net/ipv4/TCPSocket.cpp
)

set(TOOLKIT_LIB toolkit)
add_library(${TOOLKIT_LIB} STATIC ${TOOLKIT_SRC})

set(TOOLKIT_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src;${CMAKE_CURRENT_BINARY_DIR}" PARENT_SCOPE)
set(TOOLKIT_LIBRARIES ${TOOLKIT_LIB} ${CMAKE_THREAD_LIBS_INIT} PARENT_SCOPE)
