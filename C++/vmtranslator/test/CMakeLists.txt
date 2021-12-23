set(BINARY ${CMAKE_PROJECT_NAME}_test)

set(SOURCES vmtranslator_test.cpp
            main.cpp)

include_directories(${GMOCK_HOME}/include ${GTEST_HOME}/include)

link_directories(${GOOGLETEST_LIB})

add_executable(${BINARY} ${SOURCES})

add_test(NAME ${BINARY} COMMAND ${BINARY})

ADD_DEFINITIONS(-DNT_SUITE="${NT_SUITE}")
ADD_DEFINITIONS(-DTEST_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(${BINARY} ${CMAKE_PROJECT_NAME})
target_link_libraries(${BINARY} gmock)
target_link_libraries(${BINARY} gtest)
target_link_libraries(${BINARY} pthread)
