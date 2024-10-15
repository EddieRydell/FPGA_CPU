`timescale 1ns / 1ps

`include "op_codes.vh"

module ALU (
    input logic [3:0] op_code,
    input logic [3:0] function_code,
    input logic [31:0] operand1,
    input logic [31:0] operand2,
    output logic [31:0] result,
    output logic [3:0] flags
    );
    
    always_comb begin
        result = 0; // default value
        flags = 0;
        case(op_code)
            `OP_BINARY: begin
                case(function_code)
                    `FUN_ADD: result = operand1 + operand2;
                    `FUN_SUB: result = operand1 - operand2;
                    `FUN_MULT: result = operand1 * operand2;
                    `FUN_DIV: result = operand1 / operand2; // TODO: not ready for division yet
                    `FUN_AND: result = operand1 & operand2;
                    `FUN_OR: result = operand1 | operand2;
                    `FUN_XOR: result = operand1 ^ operand2;
                endcase
            end
            `OP_UNARY: begin
                case(function_code)
                    `FUN_INC: result = operand1 + 1;
                    `FUN_DEC: result = operand1 - 1;
                endcase
            end
        endcase
    end

endmodule