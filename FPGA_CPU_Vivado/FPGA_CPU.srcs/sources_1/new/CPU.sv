`timescale 1ns / 1ps

module CPU (
    input logic clk,
    input logic reset
    );
    
    // Registers and Program Counter
    logic[31:0] program_counter;
    logic[31:0] register[0:15];
    logic[3:0] flags;
    
    // FETCH
    logic[47:0] fetch_instruction;
    program_memory #(
        .MEMORY_DEPTH(256)
        ) 
        program_memory (
            .program_counter(program_counter),
            .instruction(fetch_instruction)
        );
    
    // DECODE
    logic[47:0] decode_instruction;
    logic[3:0] decode_op_code;
    logic[3:0] decode_function_code;
    logic[3:0] decode_reg1;
    logic[3:0] decode_reg2;
    logic[31:0] decode_immediate;
    instruction_decoder decoder (
        .instruction(decode_instruction), 
        .op_code(decode_op_code), 
        .function_code(decode_function_code),
        .reg1(decode_reg1),
        .reg2(decode_reg2),
        .immediate(decode_immediate)
        );
    
    // EXECUTE
    logic[3:0] execute_op_code;
    logic[3:0] execute_function_code;
    logic[3:0] execute_reg1;
    logic[3:0] execute_reg2;
    logic[31:0] execute_immediate;
    logic[31:0] execute_result;
    ALU ALU(
        .op_code(execute_op_code),
        .function_code(execute_function_code),
        .operand1(register[execute_reg1]),
        .operand2(register[execute_reg2]),
        .result(execute_result)
        );
    
    // MEMORY ACCESS
    logic[3:0] memory_op_code;
    logic[3:0] memory_function_code;
    logic[3:0] memory_reg1;
    logic[3:0] memory_reg2;
    logic[31:0] memory_immediate;
    logic[31:0] memory_result;
    logic[31:0] memory_accessed;
    data_memory #(
        .MEMORY_DEPTH(1024)
        ) 
        data_memory (
            .address(memory_immediate),
            .data(memory_accessed)
        );
        
    // WRITEBACK
    logic[3:0] writeback_op_code;
    logic[3:0] writeback_function_code;
    logic[31:0] writeback_reg1;
    logic[31:0] writeback_reg2;
    logic[31:0] writeback_immediate;
    logic[31:0] writeback_result;
    logic[31:0] writeback_accessed;
        
    always_ff @(posedge clk) begin
        if (reset) begin
            program_counter <= 0;
        end else begin
        // FETCH
        decode_instruction <= fetch_instruction;
        
        // DECODE
        execute_op_code <= decode_op_code;
        execute_function_code <= decode_function_code;
        execute_reg1 <= decode_reg1;
        execute_reg2 <= decode_reg2;
        execute_immediate <= decode_immediate;
        program_counter <= program_counter + 1;
        
        // EXECUTE
        memory_op_code <= execute_op_code;
        memory_function_code <= execute_function_code;
        memory_reg1 <= execute_reg1;
        memory_reg2 <= execute_reg2;
        memory_immediate <= execute_immediate;
        memory_result <= execute_result;
        
        // MEMORY ACCESS
        writeback_op_code <= memory_op_code;
        writeback_function_code <= memory_function_code;
        writeback_reg1 <= memory_reg1;
        writeback_reg2 <= memory_reg2;
        writeback_immediate <= memory_immediate;
        writeback_result <= memory_result;
        writeback_accessed <= memory_accessed;
        // WRITEBACK
        case(writeback_op_code)
            `OP_NOP:;
            `OP_HLT:;
            `OP_BINARY:
                register[writeback_reg2] <= writeback_result;
            `OP_UNARY:
                register[writeback_reg1] <= writeback_result;
            `OP_MOV:
                case(writeback_function_code)
                    `FUN_IR: register[writeback_reg1] <= writeback_immediate;
                    `FUN_RR: register[writeback_reg2] <= register[writeback_reg1];
                    `FUN_RM:; // TODO
                    `FUN_MR: register[writeback_reg1] <= memory_accessed;
                endcase
            `OP_JMP:
                case (writeback_function_code)
                    `FUN_JU:
                        program_counter <= memory_immediate;
                    `FUN_JE:
                        if (flags & `FLAG_ZERO)
                            program_counter <= memory_immediate;
                    `FUN_JNE:
                        if (!(flags & `FLAG_ZERO))
                            program_counter <= memory_immediate;
                    `FUN_JG:
                        if (!(flags & `FLAG_ZERO) && !(flags & `FLAG_SIGN))
                            program_counter <= memory_immediate;
                    `FUN_JL:
                        if ((flags & `FLAG_SIGN) != (flags & `FLAG_OVERFLOW))
                            program_counter <= memory_immediate;
                endcase
            `OP_CALL:;
            `OP_RET:;
            `OP_PUSH:;
            `OP_POP:;
            `OP_LOAD:;
            `OP_STORE:;
            `OP_SYSCALL:;
        endcase
        end
    
    end

endmodule
