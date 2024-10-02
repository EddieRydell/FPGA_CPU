#include "../headers/program.h"

#include <iostream>
#include <sstream>
#include <iomanip>

void program::add_instruction(const std::string& instruction_string, int line_number) {
    instruction i;
    // Catch all parsing errors and handle them all from here
    // This should ensure that no error handling occurs inside the actual parsing logic
    try {
        i = instruction_string;
    }
    catch (std::exception& e) {
        std::stringstream ss;
        ss << "ERROR: Unable to parse line " << line_number << "\n" << "Message: " << e.what() << "\n";
        throw std::runtime_error(ss.str());
    }
    this->instructions.push_back(i);
}

std::vector<uint8_t> program::generate_machine_code() {
    std::cout << "instructions size: " << this->instructions.size() << std::endl;
    std::vector<uint8_t> machine_code(6);
    for (const auto& i : instructions) {
        machine_code.push_back((i.op_code << 4) | i.fun_code);
        machine_code.push_back((i.arg1 << 4) | i.arg2);
        for (const auto& byte : i.immediate) {
            machine_code.emplace_back(byte);
        }
    }
    return machine_code;
}

void program::print_machine_code() {
    const std::vector<uint8_t> bytes = generate_machine_code();
    for (int i = 0; i < bytes.size(); i++) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)bytes.at(i) << " ";

        // Break line after every 6 bytes
        if (i % 6 == 5) {
            std::cout << std::endl;
        }
    }
}
