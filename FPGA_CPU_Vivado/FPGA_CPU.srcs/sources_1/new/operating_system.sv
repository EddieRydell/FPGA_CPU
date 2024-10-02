`timescale 1ns / 1ps

module operating_system(
    input logic clk,
    input logic reset
    );
    CPU CPU1 (
        .clk(clk),
        .reset(reset)
    );
    
    
    always_ff @(posedge clk) begin
    
    end
    
endmodule
