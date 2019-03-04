cmake_minimum_required(VERSION 3.3)
add_subdirectory(.. ${CMAKE_CURRENT_BINARY_DIR}/toolkit)

#include_directories(lest/include)
#add_executable(toolkit-unit-tests tests.cpp)

add_executable(toolkit-poll-stress-test server.cpp)
target_link_libraries(toolkit-poll-stress-test toolkit)
