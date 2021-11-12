project(assembler_tester)
cmake_minimum_required(VERSION 3.0)

include_directories($ENV{GMOCK_HOME}/include $ENV{GTEST_HOME}/include ../include)
link_directories($ENV{GOOGLETEST}/build/lib)
add_definitions(-std=c++17)
set(CMAKE_CXX_FLAGS "${CMAXE_CXX_FLAGS} -Wall")


ADD_DEFINITIONS(-DTEST_DIR="..")

set(sources 
   src/main.cpp
   src/assembler_test.cpp)
add_executable(test ${sources})
target_link_libraries(test stdc++fs)
target_link_libraries(test gmock)
target_link_libraries(test gtest)
target_link_libraries(test pthread)


