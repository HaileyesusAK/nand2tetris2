set(BINARY ${CMAKE_PROJECT_NAME})

set(SOURCES assembler.cpp)

add_library(${BINARY} SHARED ${SOURCES})
