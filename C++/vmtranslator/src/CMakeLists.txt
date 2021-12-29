set(BINARY ${CMAKE_PROJECT_NAME})

set(SOURCES vmtranslator.cpp
            command.cpp
            binary_alu_cmd.cpp
            add.cpp
            sub.cpp
            and.cpp
            or.cpp
            relational_cmd.cpp
            eq.cpp
            lt.cpp
            gt.cpp
            not.cpp
            neg.cpp
            push.cpp
            pop.cpp
            push_static.cpp
            pop_static.cpp
            factory.cpp)
            
add_library(${BINARY} SHARED ${SOURCES})
