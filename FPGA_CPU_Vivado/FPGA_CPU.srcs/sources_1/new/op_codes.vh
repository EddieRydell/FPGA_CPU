// Header file containing predefined constants for all op codes, states, and function codes for the CPU

typedef enum logic [3:0] {
    CPU_IDLE,
    CPU_RUNNING,
    CPU_HALTED,
    CPU_SLEEPING,
    CPU_WAITING,
    CPU_RESET,
    CPU_INTERRUPT_HANDLING,
    CPU_FAULT_HANDLING,
    CPU_DEBUG_BREAKPOINT,
    CPU_BOOTING,
    CPU_THROTTLED,
    CPU_CONTEXT_SWITCHING
} CPU_state_t;

// op codes
`define OP_NOP      4'h0
`define OP_HLT      4'h1
`define OP_BINARY   4'h2
`define OP_UNARY    4'h3
`define OP_MOV      4'h4
`define OP_JMP      4'h5
`define OP_CALL     4'h6
`define OP_RET      4'h7
`define OP_PUSH     4'h8
`define OP_POP      4'h9
`define OP_SYSCALL  4'ha
`define OP_BINARYF  4'hb
`define OP_UNARYF   4'hc
`define OP_LOAD     4'hd
`define OP_STORE    4'he

// function codes for binary operators
`define FUN_ADD     4'h0
`define FUN_SUB     4'h1
`define FUN_MULT    4'h2
`define FUN_DIV     4'h3
`define FUN_AND     4'h4
`define FUN_OR      4'h5
`define FUN_XOR     4'h6
`define FUN_CMP     4'h7

// function codes for unary operators
`define FUN_INC     4'h0
`define FUN_DEC     4'h1
`define FUN_NOT     4'h2

// function codes for floating point binary operators
`define FUN_ADDF    4'h0
`define FUN_SUBF    4'h1
`define FUN_MULTF   4'h2
`define FUN_DIVF    4'h3
`define FUN_CMPF    4'h4

// function codes for floating point unary operators
`define FUN_FTOI    4'h0
`define FUN_ITOF    4'h1
`define FUN_SINF    4'h2
`define FUN_COSF    4'h3
`define FUN_TANF    4'h4
`define FUN_EXPF    4'h5
`define FUN_LOG2F   4'h6
`define FUN_SQRTF   4'h7

// function codes for mov instructions
`define FUN_IR      4'h0
`define FUN_RR      4'h1
`define FUN_RM      4'h2
`define FUN_MR      4'h3

// function codes for jmp instructions
`define FUN_JU      4'h0
`define FUN_JE      4'h1
`define FUN_JNE     4'h2
`define FUN_JG      4'h3
`define FUN_JL      4'h4
`define FUN_JC      4'h5
`define FUN_JNC     4'h6
`define FUN_JO      4'h7
`define FUN_JS      4'h8
`define FUN_JNS     4'h9

// function codes for different syscalls
`define SYSCALL_LED_WRITE   4'h0
`define SYSCALL_HALT        4'h1

// flag codes
`define FLAG_ZERO       4'b0001
`define FLAG_CARRY      4'b0010
`define FLAG_SIGN       4'b0100
`define FLAG_OVERFLOW   4'b1000
