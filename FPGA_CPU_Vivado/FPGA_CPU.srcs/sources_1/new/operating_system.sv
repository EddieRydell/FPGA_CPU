`timescale 1ns / 1ps

module operating_system(
    input logic clk,
    input logic reset
    );
    CPU CPU1 (
        .clk(clk),
        .reset(reset)
    );
    
    logic rx;
    logic [47:0] rx_out;
    logic rx_data_ready;
    serial_receiver serial_receiver (
        .clk(clk),
        .reset(reset),
        .rx(rx),
        .data_out(rx_out),
        .data_ready(rx_data_ready)
    );
    
    serial_transmitter serial_transmitter (
    
    );
    
    
    always_ff @(posedge clk) begin
    
    end
    
endmodule
