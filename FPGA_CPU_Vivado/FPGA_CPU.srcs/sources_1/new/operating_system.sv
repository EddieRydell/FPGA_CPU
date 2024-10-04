`timescale 1ns / 1ps

module operating_system(
    input logic clk,
    input logic reset,
    input logic rx,
    output logic tx
    );
    
    CPU CPU1 (
        .clk(clk),
        .reset(reset)
    );
    
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
    
    typedef enum logic[2:0] {
        RESET = 0,
        INIT_HANDSHAKE_RECEIVING,
        INIT_HANDSHAKE_WAITING,
        INIT_HANDSHAKE_TRANSMITTING,
        INIT_RECEIVING_PROGRAM,
        RUNNING,
        HALTED
    } state;
    state system_state;
    state next_state;
    always_ff @(posedge clk) begin
        if (reset) begin 
            system_state <= RESET;
            next_state <= RESET;
        end
        else begin
        case (system_state)
            RESET: begin
                next_state = INIT_HANDSHAKE_WAITING;
                tx <= 0;
            end
            INIT_HANDSHAKE_WAITING: begin
                
            end
            INIT_HANDSHAKE_RECEIVING:;
            INIT_HANDSHAKE_TRANSMITTING:;
            INIT_RECEIVING_PROGRAM:;
            RUNNING:;
            HALTED:;
        endcase
        end
    end
    
endmodule
