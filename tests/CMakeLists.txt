cmake_minimum_required(VERSION 2.6)
add_subdirectory(.. ${CMAKE_CURRENT_BINARY_DIR}/toolkit)

include_directories(${TOOLKIT_INCLUDE_DIRS})
link_libraries(${TOOLKIT_LIBRARIES})

#include_directories(lest/include)
#add_executable(toolkit-unit-tests tests.cpp)

add_executable(toolkit-poll-stress-test server.cpp)
