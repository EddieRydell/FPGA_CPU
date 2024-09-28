#include <iostream>
#include <fstream>
#include <string>

#include "program.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "ERROR: No file selected\n Usage: ./assembler <assembly_file.asm>\n";
        return 1;
    }
    std::string file_name = argv[1];
    std::ifstream file;
    file.open(file_name);

    program my_program;

    std::string curr_line;
    while (std::getline(file, curr_line)) {
        my_program.add_instruction(curr_line);
    }

    return 0;
}

