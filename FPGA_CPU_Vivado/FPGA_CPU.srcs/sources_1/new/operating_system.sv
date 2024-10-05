`timescale 1ns / 1ps

module operating_system(
    input logic clk,
    input logic reset,
    input logic rx,
    output logic tx,
    output logic LED_1
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
    
    logic tx_send;
    logic [47:0] tx_in;
    logic tx_busy;
    serial_transmitter serial_transmitter (
        .clk(clk),
        .reset(reset),
        .send(tx_send),
        .data_in(tx_in),
        .tx(tx),
        .busy(tx_busy)
    );
    
    logic [47:0] PING_STRING = {8'h2E, 8'h50, 8'h49, 8'h4E, 8'h47, 8'h2E}; // ".PING."
    logic [47:0] ACK_STRING = {8'h41, 8'h43, 8'h4B, 8'h41, 8'h43, 8'h4B};  // "ACKACK"
    
    typedef enum logic[2:0] {
        RESET = 0,
        INIT_HANDSHAKE_RECEIVING,
        INIT_HANDSHAKE_WAITING,
        INIT_HANDSHAKE_START_TRANSMITTING,
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
            LED_1 <= 1;
        end
        else begin
        system_state <= next_state;
        case (system_state)
            RESET: begin
                next_state <= INIT_HANDSHAKE_WAITING;
                LED_1 <= 0;
            end
            
            INIT_HANDSHAKE_WAITING: begin
                if (!rx) next_state <= INIT_HANDSHAKE_RECEIVING;
            end
            
            INIT_HANDSHAKE_RECEIVING:
                if (rx_data_ready)
                    if (rx_out == PING_STRING)
                        next_state <= INIT_HANDSHAKE_START_TRANSMITTING;
                    else next_state = INIT_HANDSHAKE_WAITING;
                    
            INIT_HANDSHAKE_START_TRANSMITTING: begin
                tx_in <= ACK_STRING;
                tx_send <= 1;
            end
            
            INIT_HANDSHAKE_TRANSMITTING: begin
                tx_send <= 0;
                if (!tx_busy)
                    next_state <= INIT_RECEIVING_PROGRAM;
            end
            
            INIT_RECEIVING_PROGRAM:;
            
            RUNNING:;
            
            HALTED:;
            
        endcase
        end
    end
    
endmodule
