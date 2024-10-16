`timescale 1ns / 1ps

module CPU (
    input logic clk,
    input logic reset,
    
    // I/O wires for reading/writing data memory
    input logic [31:0] memory_data_accessed,
    output logic [31:0] memory_data_to_write,
    output logic [31:0] memory_access_address,
    output logic memory_read_enable,
    output logic memory_write_enable,
    
    // I/O wires for reading instruction memory
    input logic [47:0] instruction_data,
    output logic [31:0] instruction_address,
    
    // I/O wires for FPU
    input logic [31:0] FPU_result,
    input logic FPU_valid_in,               // set when FPU is ready to receive an instruction from CPU
    input logic FPU_valid_out,              // set when FPU is done with an operation
    input logic [3:0] FPU_flags,            // flags set by the FPU based on the result of the operation
    output logic [31:0] FPU_op1,
    output logic [31:0] FPU_op2,
    output logic [3:0] FPU_op_code,
    output logic [3:0] FPU_function_code,
    output logic FPU_enable,                // set when CPU is sending operands to FPU
    output logic FPU_ready,                 // set when CPU is ready to receive result from FPU
    
    // I/O wires for syscalls
    input logic [31:0] syscall_result,
    input logic syscall_valid_in,           // set when OS has received a syscall
    input logic syscall_valid_out,          // set when OS is done executing a syscall
    output logic [31:0] syscall_data,
    output logic syscall_enable,            // set when CPU wants to execute a syscall
    output logic syscall_ready,             // set when CPU has received syscall result
    
    // I/O wires for determining the status of the CPU
    input logic [3:0] OS_status_in,         // sent by the OS to set the CPU's state
    input logic OS_status_write_enable,
    output logic [3:0] current_status,
    
    // Registers as outputs so they can be accessed during syscalls
    output logic [31:0] register[0:15]
    );
    
    // Registers, Stack, and Program Counter
    logic [31:0] stack[0:1023];
    logic [31:0] stack_pointer;
    logic [31:0] program_counter;
    assign instruction_address = program_counter;
    
    logic [3:0] ALU_flags;
    
    logic [47:0] fetched_instruction;
    
    logic[3:0] decode_op_code;
    logic[3:0] decode_function_code;
    logic[3:0] decode_reg1;
    logic[3:0] decode_reg2;
    logic[31:0] decode_immediate;
    
    logic [3:0] execute_op_code;
    logic [3:0] execute_function_code;
    logic [3:0] execute_reg1;
    logic [3:0] execute_reg2;
    logic [31:0] execute_immediate;
    logic [31:0] execute_result;
    
    logic [3:0] memory_op_code;
    logic [3:0] memory_function_code;
    logic [3:0] memory_reg1;
    logic [3:0] memory_reg2;
    logic [31:0] memory_immediate;
    logic [31:0] memory_result;
    logic [31:0] memory_read;
    
    instruction_decoder decoder (
        .instruction(fetched_instruction), 
        .op_code(decode_op_code), 
        .function_code(decode_function_code),
        .reg1(decode_reg1),
        .reg2(decode_reg2),
        .immediate(decode_immediate)
        );
    
    ALU ALU(
        .op_code(execute_op_code),
        .function_code(execute_function_code),
        .operand1(register[execute_reg1]),
        .operand2(register[execute_reg2]),
        .result(execute_result),
        .flags(ALU_flags)
        );
    
    enum logic [2:0] {
        SYSCALL_IDLE,
        SYSCALL_ENTRY,
        SYSCALL_WAITING,
        SYSCALL_EXECUTING,
        SYSCALL_EXIT
    } syscall_handling_state;

    enum logic [2:0] {
        FPU_IDLE,
        FPU_ENTRY,
        FPU_WAITING,
        FPU_EXECUTING,
        FPU_EXIT
    } FPU_handling_state;
    
        
    always_ff @(posedge clk) begin
        if (OS_status_write_enable)
            current_status <= OS_status_in;
        if (reset) begin
            register <= {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
            program_counter <= 0;
            current_status <= CPU_IDLE;
            syscall_handling_state <= SYSCALL_IDLE;
            FPU_handling_state <= FPU_IDLE;
            stack_pointer <= 1023;
        end 
        else if (current_status == CPU_RUNNING) begin
            fetched_instruction <= instruction_data;
            program_counter <= program_counter + 1;
            
            execute_op_code <= decode_op_code;
            execute_function_code <= decode_function_code;
            execute_reg1 <= decode_reg1;
            execute_reg2 <= decode_reg2;
            execute_immediate <= decode_immediate;
            
            if (execute_op_code == `OP_MOV && execute_function_code == `FUN_MR)
                memory_read_enable <= 1;
            
            memory_op_code <= execute_op_code;
            memory_function_code <= execute_function_code;
            memory_reg1 <= execute_reg1;
            memory_reg2 <= execute_reg2;
            memory_immediate <= execute_immediate;
            memory_result <= execute_result;
            memory_read <= memory_data_accessed;
        end
        if (current_status == CPU_RUNNING || current_status == CPU_WAITING) begin
            case(memory_op_code)
                `OP_NOP:;
                `OP_HLT:
                    current_status <= CPU_HALTED;
                `OP_BINARY:
                    register[memory_reg2] <= memory_result;
                `OP_UNARY:
                    register[memory_reg1] <= memory_result;
                `OP_MOV:
                    case(memory_function_code)
                        `FUN_IR: register[memory_reg1] <= memory_immediate;
                        `FUN_RR: register[memory_reg2] <= register[memory_reg1];
                        `FUN_RM: memory_data_to_write = register[memory_reg1];
                        `FUN_MR: register[memory_reg1] <= memory_read;
                    endcase
                `OP_JMP:
                    case (memory_function_code)
                        `FUN_JU:
                            program_counter <= memory_immediate;
                        `FUN_JE:
                            if (ALU_flags & `FLAG_ZERO)
                                program_counter <= memory_immediate;
                        `FUN_JNE:
                            if (!(ALU_flags & `FLAG_ZERO))
                                program_counter <= memory_immediate;
                        `FUN_JG:
                            if (!(ALU_flags & `FLAG_ZERO) && !(ALU_flags & `FLAG_SIGN))
                                program_counter <= memory_immediate;
                        `FUN_JL:
                            if ((ALU_flags & `FLAG_SIGN) != (ALU_flags & `FLAG_OVERFLOW))
                                program_counter <= memory_immediate;
                    endcase
                `OP_CALL: begin
                    stack[stack_pointer - 1] <= program_counter;
                    stack_pointer <= stack_pointer - 1;
                    program_counter <= memory_immediate;
                end
                `OP_RET: begin
                    program_counter <= stack[stack_pointer];
                    stack_pointer <= stack_pointer + 1;
                end
                `OP_PUSH: begin
                    stack[stack_pointer - 1] <= register[memory_reg1];
                    stack_pointer <= stack_pointer - 1;
                end
                `OP_POP: begin
                    register[memory_reg1] <= stack[stack_pointer];
                    stack_pointer <= stack_pointer + 1;
                end
                `OP_SYSCALL: begin
                    case(syscall_handling_state)
                        SYSCALL_IDLE: begin
                            memory_op_code <= memory_op_code;
                            syscall_ready <= 0;
                            syscall_enable <= 0;
                            syscall_handling_state <= SYSCALL_ENTRY;
                            current_status <= CPU_WAITING;
                        end
                        
                        SYSCALL_ENTRY: begin
                            syscall_enable <= 1;
                            syscall_data <= memory_immediate;
                            syscall_handling_state <= SYSCALL_WAITING;
                        end
                        
                        SYSCALL_WAITING:
                            if (syscall_valid_in) begin
                                syscall_handling_state <= SYSCALL_EXECUTING;
                                syscall_enable <= 0;
                            end
                                
                        SYSCALL_EXECUTING:
                            if (syscall_valid_out)
                                syscall_handling_state <= SYSCALL_EXIT;
                                
                        SYSCALL_EXIT: begin
                            syscall_handling_state <= SYSCALL_IDLE;
                            current_status <= CPU_RUNNING;
                            memory_op_code <= `OP_NOP;
                            syscall_ready <= 1;
                            register[0] <= syscall_result;
                        end
                        
                        default:
                            syscall_handling_state <= SYSCALL_IDLE;
                    endcase
                end
                `OP_BINARYF, `OP_UNARYF: begin
                    case(FPU_handling_state)
                        FPU_IDLE: begin
                            FPU_ready <= 0;
                            FPU_enable <= 0;
                            FPU_handling_state <= FPU_ENTRY;
                        end
                        
                        FPU_ENTRY: begin
                            FPU_enable <= 1;
                            FPU_op1 <= register[memory_reg1];
                            FPU_op2 <= register[memory_reg2];
                            FPU_op_code <= memory_op_code;
                            FPU_function_code <= memory_function_code;
                            current_status <= CPU_WAITING;
                            FPU_handling_state <= FPU_WAITING;
                        end
                        
                        FPU_WAITING:
                            if (FPU_valid_in) begin
                                FPU_handling_state <= FPU_EXECUTING;
                                FPU_enable <= 0;
                            end
                                
                        FPU_EXECUTING:
                            if (FPU_valid_out)
                                FPU_handling_state <= FPU_EXIT;
                                
                        FPU_EXIT: begin
                            FPU_handling_state <= FPU_IDLE;
                            current_status <= CPU_RUNNING;
                            FPU_ready <= 1;
                            register[memory_reg1] <= FPU_result;
                        end
                        
                        default:
                            FPU_handling_state <= FPU_IDLE;
                    endcase
                end
            endcase
        end
    end
    
endmodule
