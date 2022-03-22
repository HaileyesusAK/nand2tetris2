set(BINARY ${CMAKE_PROJECT_NAME})

set(SOURCES token.cpp
            tokenizer.cpp
            syntax_tree.cpp
            parser.cpp            
            integer_term.cpp)
            
add_library(${BINARY} SHARED ${SOURCES})
