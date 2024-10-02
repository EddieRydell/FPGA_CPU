#include <iostream>
#include <fstream>
#include <string>

#include "headers/program.h"
#include "headers/uart_transmitter.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "ERROR: No file selected\n Usage: ./assembler <assembly_file.asm>\n";
        return 1;
    }
    std::string file_name = argv[1];
    std::ifstream file;
    file.open(file_name);
    if (!file.is_open()) {
        std::cerr << "ERROR: Failed to open file: " << file_name << std::endl;
        exit(-1);
    }

    program my_program;

    std::string curr_line;
    int line_number = 0;
    while (std::getline(file, curr_line)) {
        my_program.add_instruction(curr_line, line_number++);
    }
    my_program.print_machine_code();

    uart_transmitter tx;
    tx.initialize();

    return 0;
}

