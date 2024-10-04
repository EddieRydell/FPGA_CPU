`timescale 1ns / 1ps

module serial_transmitter #(
    parameter CLOCK_FREQ = 100000000,
    parameter BAUD_RATE = 115200
)(
    input logic clk,          // Clock signal
    input logic reset,          // Reset signal
    input logic send,         // Signal to start transmission
    input logic [47:0] data_in,   // Data byte to transmit
    output logic tx,           // UART TX (data to the PC)
    output logic busy          // Busy signal during transmission
);

    localparam BAUD_TICK = CLOCK_FREQ / BAUD_RATE;

    logic [15:0] baud_counter = 0;
    logic [3:0] bit_counter = 0;
    logic [7:0] shift_reg = 0;
    
    always_ff @(posedge clk) begin
        if (reset) begin
            baud_counter <= 0;
            bit_counter <= 0;
            tx <= 1;
            busy <= 0;
        end 
        else if (send && !busy) begin
            busy <= 1;
            shift_reg <= data_in;
            bit_counter <= 0;
        end 
        else if (busy) begin
            if (baud_counter == 0) begin
                baud_counter <= BAUD_TICK - 1;
                
                if (bit_counter == 0) begin
                    tx <= 0; // Start bit
                end 
                else if (bit_counter <= 8) begin
                    tx <= shift_reg[0];
                    shift_reg <= shift_reg >> 1;
                end
                else if (bit_counter == 9) begin
                    tx <= 1; // Stop bit
                end
                else begin
                    busy <= 0; // Transmission complete
                end
                
                bit_counter <= bit_counter + 1;
            end
            else begin
                baud_counter <= baud_counter - 1;
            end
        end
    end
endmodule
