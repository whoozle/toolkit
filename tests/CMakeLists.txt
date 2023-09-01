cmake_minimum_required(VERSION 3.5)
project(toolkit-tests)
add_subdirectory(.. ${CMAKE_CURRENT_BINARY_DIR}/toolkit)

include_directories(lest/include)
add_executable(toolkit-unit-tests test-runner.cpp)
target_link_libraries(toolkit-unit-tests toolkit)

add_executable(toolkit-poll-stress-test server.cpp)
target_link_libraries(toolkit-poll-stress-test toolkit)
