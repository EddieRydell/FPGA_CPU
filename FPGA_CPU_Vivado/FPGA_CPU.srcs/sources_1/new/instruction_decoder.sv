`timescale 1ns / 1ps
module instruction_decoder (
    input logic [47:0] instruction,
    output logic [3:0] op_code,
    output logic [3:0] function_code,
    output logic [3:0] reg1,
    output logic [3:0] reg2,
    output logic [31:0] immediate
    );

    always_comb begin
        op_code = instruction[47:44];
        function_code = instruction[43:40];
        reg1 = instruction[39:36];
        reg2 = instruction[35:32];
        immediate = instruction[31:0];
    end
    
    
endmodule
