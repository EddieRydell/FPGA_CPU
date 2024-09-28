#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "ERROR: No file selected\n Usage: ./assembler <assembly_file.asm>\n";
        return 1;
    }
    std::string file_name = argv[1];
    std::ifstream file;
    file.open(file_name, )
    return 0;
}

