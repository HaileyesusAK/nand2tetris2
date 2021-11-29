project(assembler_tester)
cmake_minimum_required(VERSION 3.0)

include_directories($ENV{GMOCK_HOME}/include $ENV{GTEST_HOME}/include ../assembler/include)
link_directories($ENV{GOOGLETEST}/build/lib)
add_definitions(-std=c++17)
set(CMAKE_CXX_FLAGS "${CMAXE_CXX_FLAGS} -Wall")


ADD_DEFINITIONS(-DTEST_DIR="..")

set(sources 
   src/main.cpp
   ../assembler/src/assembler.cpp
   src/assembler_test.cpp)
add_executable(tester ${sources})
target_link_libraries(tester stdc++fs)
target_link_libraries(tester gmock)
target_link_libraries(tester gtest)
target_link_libraries(tester pthread)
set_target_properties(tester PROPERTIES COMPILE_FLAGS "--coverage")
set_target_properties(tester PROPERTIES LINK_FLAGS "--coverage")
