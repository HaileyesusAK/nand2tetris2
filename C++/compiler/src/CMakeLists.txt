set(BINARY ${CMAKE_PROJECT_NAME})

set(SOURCES token.cpp)
            
add_library(${BINARY} SHARED ${SOURCES})
