`timescale 1ns / 1ps

module FPU(
    output logic [31:0] result,
    output logic valid_in,              // set when FPU is ready to receive an instruction from CPU
    output logic valid_out,             // set when FPU is done with an operation
    output logic [3:0] flags,           // flags set by the FPU based on the result of the operation
    input logic [31:0] op1,             // First operand (IEEE 754 format)
    input logic [31:0] op2,             // Second operand (IEEE 754 format)
    input logic [3:0] op_code,          // Operation code (e.g., ADD, SUB, MUL, DIV)
    input logic [3:0] function_code,
    input logic enable,
    input logic ready,
    input logic clk,
    input logic reset
);

    // Extract fields from op1 and op2 (IEEE 754 Single Precision)
    logic op1_sign, op2_sign;
    logic [7:0] op1_exponent, op2_exponent;
    logic [23:0] op1_mantissa, op2_mantissa;

    // Extract the sign, exponent, and mantissa
    assign op1_sign = op1[31];
    assign op1_exponent = op1[30:23];
    assign op1_mantissa = {1'b1, op1[22:0]};  // Implicit leading 1 in IEEE 754
    assign op2_sign = op2[31];
    assign op2_exponent = op2[30:23];
    assign op2_mantissa = {1'b1, op2[22:0]};  // Implicit leading 1

    // Internal signals
    logic [31:0] intermediate_result;
    logic [3:0] internal_flags;              // Overflow, underflow, zero, NaN, etc.
    logic operation_done;                    // Marks when operation is done

    // State machine for managing FPU operations
    typedef enum logic [1:0] {
        IDLE,
        EXECUTE,
        DONE
    } FPU_state_t;

    FPU_state_t state;  // Current state

    logic [31:0] result_reg;

    // Main FPU state machine
    always_ff @(posedge clk or posedge reset) begin
        if (reset) begin
            state <= IDLE;
            valid_out <= 1'b0;
            result_reg <= 32'b0;
        end 
        else begin
            /*case (state)
                *//*IDLE: begin
                    if (enable) begin
                        valid_in <= 1'b1;
                        state <= EXECUTE;
                    end
                end
                EXECUTE: begin
                    // Call specific operations based on op_code and function_code
                    case (op_code)
                        `OP_BINARYF: begin
                            case (function_code)
                                `FUN_ADDF: add(op1_sign, op1_exponent, op1_mantissa, op2_sign, op2_exponent, op2_mantissa);
                                `FUN_SUBF: subtract(op1_sign, op1_exponent, op1_mantissa, op2_sign, op2_exponent, op2_mantissa);
                                `FUN_MULTF: multiply(op1_sign, op1_exponent, op1_mantissa, op2_sign, op2_exponent, op2_mantissa);
                                `FUN_DIVF: divide(op1_sign, op1_exponent, op1_mantissa, op2_sign, op2_exponent, op2_mantissa);
                                default: result_reg <= 32'b0;
                            endcase
                        end
                        `OP_UNARYF: begin
                            case (function_code)
                                `FUN_FTOI: ftoi(op1);
                                `FUN_ITOF: itof(op1);
                                `FUN_SQRTF: sqrt(op1);
                                // You can add other unary floating-point functions here
                                default: result_reg <= 32'b0;
                            endcase
                        end
                        default: result_reg <= 32'b0;
                    endcase
                    state <= DONE;
                end
                DONE: begin
                    if (ready) begin
                        result <= result_reg;
                        valid_out <= 1'b1;
                        state <= IDLE;
                    end
                end*//*
            endcase*/
        end
    end

endmodule

