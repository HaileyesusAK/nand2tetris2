cmake_minimum_required(VERSION 3.10)

project(vmtranslator)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Wall -Wpedantic -Werror")

include_directories(include)

add_subdirectory(src)
add_subdirectory(test)
