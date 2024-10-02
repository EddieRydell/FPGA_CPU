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

# Link with required Windows libraries
target_link_libraries(Assembler
        kernel32
        user32
        gdi32
        winspool
        shell32
        ole32
        oleaut32
        uuid
        comdlg32
        advapi32
        setupapi
)
