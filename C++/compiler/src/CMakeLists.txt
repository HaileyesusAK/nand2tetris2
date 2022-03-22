set(BINARY ${CMAKE_PROJECT_NAME})

set(SOURCES integer_term.cpp
            keyword_term.cpp
            parser.cpp
            string_term.cpp
            syntax_tree.cpp
            term.cpp
            token.cpp
            tokenizer.cpp)

add_library(${BINARY} SHARED ${SOURCES})
