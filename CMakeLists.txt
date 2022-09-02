cmake_minimum_required(VERSION 3.3)
include(CheckCXXCompilerFlag)

if(NOT DEFINED CMAKE_CXX_STANDARD)
	set(CMAKE_CXX_STANDARD 17)
endif()
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
find_package( Threads )

check_cxx_compiler_flag(-Wall HAS_WALL)

include_directories(src)

if (NOT DEFINED TOOLKIT_NS)
	set(TOOLKIT_NS toolkit)
endif()

message(STATUS "toolkit namespace: ${TOOLKIT_NS}")

set(TOOLKIT_CONFIG_DIR ${CMAKE_CURRENT_BINARY_DIR}/toolkit/core)
file(MAKE_DIRECTORY ${TOOLKIT_CONFIG_DIR})
configure_file(src/toolkit/core/config.h.in ${TOOLKIT_CONFIG_DIR}/config.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(TOOLKIT_SRC
	src/toolkit/core/ThreadPool.cpp
	src/toolkit/core/TaskQueue.cpp
	src/toolkit/core/StringRef.cpp

	src/toolkit/audio/WaveFile.cpp

	src/toolkit/log/Logger.cpp
	src/toolkit/log/ConsoleLoggingSink.cpp

	src/toolkit/io/DirectoryReader.cpp
	src/toolkit/io/Event.cpp
	src/toolkit/io/File.cpp
	src/toolkit/io/MemoryMapping.cpp
	src/toolkit/io/Poll.cpp
	src/toolkit/io/Process.cpp
	src/toolkit/io/SerialPort.cpp
	src/toolkit/io/SystemException.cpp
	src/toolkit/io/TemporaryFile.cpp

	src/toolkit/net/BaseSocket.cpp

	src/toolkit/net/bsd/Socket.cpp

	src/toolkit/net/ipv4/Address.cpp
	src/toolkit/net/ipv4/Endpoint.cpp

	src/toolkit/net/unix/Endpoint.cpp

	src/toolkit/text/String.cpp
	src/toolkit/text/Formatters.cpp
	src/toolkit/text/StringOutputStream.cpp

	src/toolkit/cli/OptionParser.cpp

	src/toolkit/serialization/bson/InputStream.cpp
	src/toolkit/serialization/bson/ObjectInputStream.cpp
)

if (TOOLKIT_GL_VARIANT)
	add_definitions(-DGL_GLEXT_PROTOTYPES)
	add_definitions(-DTOOLKIT_GL_VARIANT=${TOOLKIT_GL_VARIANT})
	list(APPEND TOOLKIT_SRC
		src/toolkit/raster/gl/Buffer.cpp
		src/toolkit/raster/gl/CubeMap.cpp
		src/toolkit/raster/gl/Program.cpp
		src/toolkit/raster/gl/Shader.cpp
		src/toolkit/raster/gl/VertexArray.cpp
		src/toolkit/raster/gl/FrameBuffer.cpp
		src/toolkit/raster/gl/Texture.cpp
	)
endif()

set(TOOLKIT_LIB toolkit)
add_library(${TOOLKIT_LIB} STATIC ${TOOLKIT_SRC})
target_compile_definitions(${TOOLKIT_LIB} PUBLIC -DTOOLKIT_NS=${TOOLKIT_NS})
if (TOOLKIT_GL_VARIANT)
	target_compile_definitions(${TOOLKIT_LIB} PUBLIC -DGL_GLEXT_PROTOTYPES -DTOOLKIT_GL_VARIANT=${TOOLKIT_GL_VARIANT})
endif()

target_include_directories(${TOOLKIT_LIB} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(${TOOLKIT_LIB} ${CMAKE_THREAD_LIBS_INIT})
