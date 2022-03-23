set(BINARY ${CMAKE_PROJECT_NAME})

set(SOURCES array_term.cpp
            do_statement.cpp
            expression.cpp
            expression_list.cpp
            identifier_term.cpp
            integer_term.cpp
            jack_fragment.cpp
            keyword_term.cpp
            let_statement.cpp
            method_call_term.cpp
            parenthesized_term.cpp
            parser.cpp
            return_statement.cpp
            string_term.cpp
            subroutine_call_term.cpp
            syntax_tree.cpp
            term.cpp
            term_factory.cpp
            token.cpp
            tokenizer.cpp
            unaryop_term.cpp)

add_library(${BINARY} SHARED ${SOURCES})
