set(BINARY ${CMAKE_PROJECT_NAME})

set(SOURCES token.cpp
            tokenizer.cpp)
            
add_library(${BINARY} SHARED ${SOURCES})
