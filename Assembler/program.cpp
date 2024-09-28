#include "program.h"

void program::add_instruction(const instruction& i) {
    this->instructions.push_back(i);
}

std::vector<uint8_t>  program::generate_machine_code() {
    std::vector<uint8_t> machine_code;
    for (const auto& i : instructions) {
        machine_code.push_back((i.op_code << 4) | i.fun_code);
        machine_code.push_back((i.arg1 << 4) | i.arg2);
        for (const auto& byte : i.immediate) {
            machine_code.push_back(byte);
        }
    }
    return machine_code;
}
