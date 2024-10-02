cmake_minimum_required(VERSION 3.28)
project(Assembler)

set(CMAKE_CXX_STANDARD 14)

add_executable(Assembler
        main.cpp
        headers/op_codes.h
        src/instruction.cpp
        headers/instruction.h
        src/program.cpp
        headers/program.h
        src/uart_transmitter.cpp
        headers/uart_transmitter.h)
