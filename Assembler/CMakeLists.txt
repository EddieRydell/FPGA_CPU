cmake_minimum_required(VERSION 3.28)
project(Assembler)

set(CMAKE_CXX_STANDARD 14)

add_executable(Assembler
        main.cpp
        op_codes.h
        instruction.cpp
        instruction.h
        program.cpp
        program.h)
