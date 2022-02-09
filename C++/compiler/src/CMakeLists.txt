set(BINARY ${CMAKE_PROJECT_NAME})

set(SOURCES token.cpp
            tokenizer.cpp
            syntax_tree.cpp
            parser.cpp)
            
add_library(${BINARY} SHARED ${SOURCES})
target_link_libraries(${BINARY} stdc++fs)
