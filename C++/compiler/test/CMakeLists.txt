set(BINARY ${CMAKE_PROJECT_NAME}_test)

set(SOURCES code_generator_test.cpp
            tokenizer_test.cpp
            parser_test.cpp
            statement_factory_test.cpp
            symbol_table_test.cpp
            term_factory_test.cpp
            utils.cpp
            main.cpp)

include_directories(${GMOCK_HOME}/include ${GTEST_HOME}/include)

link_directories(${GOOGLETEST_LIB})

add_definitions(-DTEST_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(${BINARY} ${SOURCES})

add_test(NAME ${BINARY} COMMAND ${BINARY})

target_link_libraries(${BINARY} ${CMAKE_PROJECT_NAME})
target_link_libraries(${BINARY} gmock)
target_link_libraries(${BINARY} gtest)
target_link_libraries(${BINARY} pthread)
target_link_libraries(${BINARY} stdc++fs)
