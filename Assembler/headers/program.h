#pragma once

#include "instruction.h"

#include <vector>
#include <cstdint>

class program {
public:
    program() = default;
    void add_instruction(const std::string&, int line_number);

    void generate_machine_code();
    void print_machine_code();
    std::vector<uint8_t> get_machine_code() {return machine_code;}

private:
    std::vector<instruction> instructions;
    std::vector<uint8_t> machine_code;
};
