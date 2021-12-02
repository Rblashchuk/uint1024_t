cmake_minimum_required(VERSION 3.20)
project(uint1024_t)

set(CMAKE_C_STANDARD 99)

add_subdirectory(lib/googletest)
include_directories(lib/googletest/googletest/include)
include_directories(lib/googletest/googlemock/include)
add_executable(uint1024_t uint1024_t.c tests/tests.cpp)

target_link_libraries(uint1024_t gtest gtest_main)