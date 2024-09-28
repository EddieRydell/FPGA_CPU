`timescale 1ns / 1ps

module data_memory #(parameter MEMORY_DEPTH = 1024) (
    input logic[31:0] address,
    output logic[31:0] data
    );
    logic[31:0] data_memory[0:MEMORY_DEPTH - 1];
    
    assign data = data_memory[address];
    
endmodule
