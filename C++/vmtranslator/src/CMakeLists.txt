set(BINARY ${CMAKE_PROJECT_NAME})

set(SOURCES vmtranslator.cpp)

add_library(${BINARY} SHARED ${SOURCES})
