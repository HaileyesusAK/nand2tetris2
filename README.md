# Introduction

C++ and Python implementation of the project works of Coursera's 

[Build a Modern Computer from First Principles: Nand to Tetris Part II](https://www.coursera.org/learn/nand2tetris2) course.

This part of the course has 6 projects in total; however, for the sake of completeness, I have decided 

to include project 6 (Assembler) from part I of the course.


# Directory Structure 

The directories C++ and Python contain implementations in the respective languages.

## C++

Each subdirectory under C++ is structured as follows:

* *project/src*     -- source code files of the implementation
* *project/include* -- header files of the implementation
* *project/test/src* -- source code files of unit tests
* *project/test/data* -- input files for unit tests
* *project/test/data/expected* -- expected output files for unit tests

For example, the following is the directory structure of implementation of the assembler:

```
            .
            └── assembler
                ├── include
                │   └── assembler.h
                ├── src
                │   └── assembler.cpp
                └── test
                    ├── CMakeLists.txt
                    ├── data
                    │   ├── expected
                    │   │   ├── Mult.hack
                    │   │   └── Pong.hack
                    │   ├── Mult.asm
                    │   └── Pong.asm
                    └── src
                        ├── assembler_test.cpp
                        └── main.cpp
```

# Testing

## C++

The programs have been tested on a 64-bit Ubuntu-18.04 LTS.

In order to run the tests, make sure you have the following tools in place:

* g++ version 8.4.0
* [googletest-v1.11.0](https://github.com/google/googletest/releases/tag/release-1.11.0)
* cmake v3.0 and above

Moreover, make sure the following environment variables are set:

* GOOGLETEST: set to the root directory of googletest
* GMOCK_HOME: set to ${GOOGLETEST}/googlemock
* GTEST_HOME: set to ${GOOGLETEST}/googletest

Finally, it is assumed that googletest is built under the directory ${GOOGLETEST}/build 
