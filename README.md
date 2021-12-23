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
* *project/test* -- source code files of unit tests
* *project/test/data* -- input files for unit tests
* *project/test/data/expected* -- expected output files for unit tests

For example, the following is the directory structure of implementation of the vmtranslator:

```
        .
        ├── CMakeLists.txt
        ├── include
        │   └── vmtranslator.h
        ├── src
        │   ├── CMakeLists.txt
        │   └── vmtranslator.cpp
        └── test
            ├── CMakeLists.txt
            ├── data
            │   ├── expected
            │   │   ├── eq.asm
            │   │   ├── eq.cmp
            │   │   ├── eq.out
            │   │   ├── eq.tst
            │   ├── Mult.asm
            │   ├── Pong.asm
            │   └── Pong.hack
            ├── main.cpp
            └── vmtranslator_test.cpp

```

# Testing

## C++

The programs have been tested on a 64-bit Ubuntu-18.04 LTS.

In order to run the tests, make sure you have the following tools in place:

* g++ version 9.3.0
* [googletest-v1.11.0](https://github.com/google/googletest/releases/tag/release-1.11.0)
* cmake v3.10 and above
* [Nand2tetris Software Suite v2.6](https://www.nand2tetris.org/software)

With those packages in place, follow these steps to build each project:

* go to the project directory

* create a new directory where cmake builds the package

* finally launch cmake as so:

    * ***cmake -DGMOCK_HOME=[GOOGLETEST_DIR]/googlemock -DGTEST_HOME=[GOOGLETEST_DIR]/googlemock -GGOOGLETEST_LIB=[GOOGLETEST_BUILD_DIR]/lib -DNT_SUITE=[NT_SUITE_DIR] ..***

        * where,

            * GOOGLETEST_DIR: googletest source directory

            * GOOGLETEST_BUILD_DIR: directory where googletest is built

            * NT_SUITE_DIR: directory that contains Nand2tetris Software Suite v2.6

* among others, the previous step creates the package's shared library under ***src** and the test program under ***test***
