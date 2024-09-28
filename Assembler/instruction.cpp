#include "instruction.h"

#include "op_codes.h"

#include <sstream>
#include <iostream>

instruction::instruction(const std::string& text) {
    this->text = text;
    std::vector<std::string> tokens = split(text);
    for (auto& token : tokens) {
        for (auto& c : token) {
            c = (char)tolower(c);
        }
    }
    try {
        if (binary_operations.find(tokens[0]) != binary_operations.end()) {
            op_code = op_codes.at("binary");
            fun_code = binary_operations.at(tokens[0]);
            arg1 = string_to_arg(tokens[1]);
            arg2 = string_to_arg(tokens[2]);
        }
        else if (unary_operations.find(tokens[0]) != unary_operations.end()) {
            op_code = op_codes.at("unary");
            fun_code = unary_operations.at(tokens[0]);
            arg1 = string_to_arg(tokens[1]);
        }
        else if (jump_operations.find(tokens[0]) != jump_operations.end()) {
            op_code = op_codes.at("jmp");
            fun_code = jump_operations.at(tokens[0]);
        }
        else {
            op_code = op_codes.at(tokens[0]);
            fun_code = 0;
        }
    }
    catch (...) {
        std::cerr << "Failed to parse instructions. Line number: " << std::endl;
    }
}

std::vector<std::string> instruction::split(const std::string& str) {
    std::vector<std::string> tokens;
    std::string token;
    std::stringstream ss(str);

    while (std::getline(ss, token, ' ')) {
        if (!token.empty()) {
            tokens.push_back(token);
        }
    }
    return tokens;
}

uint32_t instruction::string_to_arg(std::string arg) {
    char c = arg[0];
    switch (c) {
        case '$':
            return std::stoi(arg.substr(1));
        case '%':
            return std::stoi(arg.substr(2));
        case '*':
            break;
        default:
            break;
    }
    throw std::exception();
}
