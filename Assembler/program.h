#pragma once

#include "instruction.h"

#include <vector>
#include <cstdint>

class program {
public:
    program() = default;
    void add_instruction(const std::string&, int line_number);

    std::vector<uint8_t> generate_machine_code();
    void print_machine_code();

private:
    std::vector<instruction> instructions;
};
