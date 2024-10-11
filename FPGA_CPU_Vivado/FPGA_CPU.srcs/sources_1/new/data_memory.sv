`timescale 1ns / 1ps

module data_memory #(parameter MEMORY_DEPTH = 1024) (
    output logic [31:0] data_accessed,
    input logic [31:0] data_to_write,
    input logic [31:0] access_address,
    input logic read_enable,
    input logic write_enable,
    input logic clk
    );
    logic[31:0] data_memory[0:MEMORY_DEPTH - 1];
    
    always_ff @(posedge clk) begin
        if (read_enable)
            data_accessed <= data_memory[access_address];
        if (write_enable)
            data_memory[access_address] <= data_to_write;
    end
    
endmodule
