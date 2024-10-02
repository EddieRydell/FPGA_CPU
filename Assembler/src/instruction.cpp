#include "../headers/instruction.h"

#include "../headers/op_codes.h"

#include <sstream>
#include <iostream>

#define PREFIX_IMMEDIATE '$'
#define PREFIX_MEMORY '*'
#define PREFIX_INSTRUCTION '#'
#define PREFIX_REGISTER '%'

instruction::instruction(const std::string& text) {
    this->text = text;
    std::vector<std::string> tokens = split(text);
    if (tokens.empty()) {
        return;
    }
    for (auto& token : tokens) {
        for (auto& c : token) {
            c = (char)tolower(c);
        }
    }
    // First check if the instruction is a binary/unary operator, jmp instruction, or mov instruction
    // These instructions require special function codes in addition to the op code
    if (binary_operations.find(tokens[0]) != binary_operations.end()) {
        op_code = op_codes.at("binary");
        fun_code = binary_operations.at(tokens[0]);
        arg1 = string_to_arg(tokens[1]).second;
        arg2 = string_to_arg(tokens[2]).second;
    }
    else if (unary_operations.find(tokens[0]) != unary_operations.end()) {
        op_code = op_codes.at("unary");
        fun_code = unary_operations.at(tokens[0]);
        arg1 = string_to_arg(tokens[1]).second;
    }
    else if (jump_operations.find(tokens[0]) != jump_operations.end()) {
        op_code = op_codes.at("jmp");
        fun_code = jump_operations.at(tokens[0]);
        arg1 = string_to_arg(tokens[1]).second;
    }
    else if (tokens[0] == "mov") {
        op_code = op_codes.at("mov");
        auto arg1_converted = string_to_arg(tokens[1]);
        auto arg2_converted = string_to_arg(tokens[2]);
        arg1 = arg1_converted.second;
        arg2 = arg2_converted.second;
        char arg1_prefix = arg1_converted.first;
        char arg2_prefix = arg2_converted.first;

        if (arg1_prefix == PREFIX_IMMEDIATE && arg2_prefix == PREFIX_REGISTER) {
            fun_code = mov_operations.at("mov_ir");
        }
        else if (arg1_prefix == PREFIX_REGISTER && arg2_prefix == PREFIX_REGISTER) {
            fun_code = mov_operations.at("mov_rr");
        }
        else if (arg1_prefix == PREFIX_REGISTER && arg2_prefix == PREFIX_MEMORY) {
            fun_code = mov_operations.at("mov_rm");
        }
        else if (arg1_prefix == PREFIX_MEMORY && arg2_prefix == PREFIX_REGISTER) {
            fun_code = mov_operations.at("mov_mr");
        }
        else {
            throw std::runtime_error("Invalid arguments to mov instruction");
        }
    }
    else if (op_codes.find(tokens[0]) != op_codes.end()){
        op_code = op_codes.at(tokens[0]);
        fun_code = 0;
        if (op_code == op_codes.at("push")) {
            arg1 = string_to_arg(tokens[1]).second;
        }
    }
    else {
        throw std::runtime_error("Unknown instruction: " + tokens[0]);
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

std::pair<const char, const uint32_t> instruction::string_to_arg(const std::string& arg) {
    char c = arg[0];
    // $ prefixes immediate values (actual numbers), so the number is returned
    // * prefixes an address in memory, so the address is returned (not the value at that address)
    // # prefixes an address in instruction memory, so that address is returned
    // % prefixes a register, so the register number is returned
    switch (c) {
        case PREFIX_IMMEDIATE:
        case PREFIX_MEMORY:
        case PREFIX_INSTRUCTION:
            return {c, std::stoi(arg.substr(1))};
        case PREFIX_REGISTER:
            return {c, std::stoi(arg.substr(2))};
        default:
            throw std::runtime_error("Failed to parse argument in instruction");
    }
}
