#pragma once

#include <unordered_map>

// Op codes
#define OP_NOP      0x0
#define OP_HLT      0x1
#define OP_BINARY   0x2
#define OP_UNARY    0x3
#define OP_MOV      0x4
#define OP_JMP      0x5
#define OP_CALL     0x6
#define OP_RET      0x7
#define OP_PUSH     0x8
#define OP_POP      0x9
#define OP_LOAD     0xa
#define OP_STORE    0xb
#define OP_SYSCALL  0xc

// Function codes for binary operators
#define FUN_ADD     0x0
#define FUN_SUB     0x1
#define FUN_MUL    0x2
#define FUN_DIV     0x3
#define FUN_AND     0x4
#define FUN_OR      0x5
#define FUN_XOR     0x6
#define FUN_CMP     0x7

// Function codes for unary operators
#define FUN_INC     0x0
#define FUN_DEC     0x1
#define FUN_NOT     0x2

// Function codes for mov instructions
#define FUN_IR      0x0
#define FUN_RR      0x1
#define FUN_RM      0x2
#define FUN_MR      0x3

// Function codes for jmp instructions
#define FUN_JU      0x0
#define FUN_JE      0x1
#define FUN_JNE     0x2
#define FUN_JG      0x3
#define FUN_JL      0x4
#define FUN_JC      0x5
#define FUN_JNC     0x6
#define FUN_JO      0x7
#define FUN_JS      0x8
#define FUN_JNS     0x9

// Mapping mnemonics to op_codes and function codes
const std::unordered_map<std::string, int> op_codes = {
        {"nop", OP_NOP},
        {"hlt", OP_HLT},
        {"binary", OP_BINARY},
        {"unary", OP_UNARY},
        {"mov", OP_MOV},
        {"jmp", OP_JMP},
        {"call", OP_CALL},
        {"ret", OP_RET},
        {"push", OP_PUSH},
        {"pop", OP_POP},
        {"load", OP_LOAD},
        {"store", OP_STORE},
        {"syscall", OP_SYSCALL}
};

// Mapping mnemonics to function codes (for binary instructions)
const std::unordered_map<std::string, int> binary_operations = {
        {"add", FUN_ADD},
        {"sub", FUN_SUB},
        {"mul", FUN_MUL},
        {"div", FUN_DIV},
        {"and", FUN_AND},
        {"or", FUN_OR},
        {"xor", FUN_XOR},
        {"cmp", FUN_CMP}
};

const std::unordered_map<std::string, int> unary_operations = {
        {"inc", FUN_INC},
        {"dec", FUN_DEC},
        {"not", FUN_NOT}
};

std::unordered_map<std::string, int> mov_operations = {
        {"mov_ir", FUN_IR},  // Immediate to Register
        {"mov_rr", FUN_RR},  // Register to Register
        {"mov_rm", FUN_RM},  // Register to Memory
        {"mov_mr", FUN_MR}   // Memory to Register
};

std::unordered_map<std::string, int> jump_operations = {
        {"jmp", FUN_JU},     // Unconditional jump
        {"je", FUN_JE},      // Jump if equal
        {"jne", FUN_JNE},    // Jump if not equal
        {"jg", FUN_JG},      // Jump if greater
        {"jl", FUN_JL},      // Jump if less
        {"jc", FUN_JC},      // Jump if carry
        {"jnc", FUN_JNC},    // Jump if no carry
        {"jo", FUN_JO},      // Jump if overflow
        {"js", FUN_JS},      // Jump if sign
        {"jns", FUN_JNS}     // Jump if no sign
};
