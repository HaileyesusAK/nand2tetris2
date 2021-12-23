set(BINARY ${CMAKE_PROJECT_NAME})

set(SOURCES assembler.cpp instruction.cpp symbol_table.cpp)

add_library(${BINARY} SHARED ${SOURCES})

add_executable(${BINARY}_run ${SOURCES} main.cpp)

set_target_properties(${BINARY}_run PROPERTIES COMPILE_FLAGS "--coverage") 

set_target_properties(${BINARY}_run PROPERTIES LINK_FLAGS "--coverage")
