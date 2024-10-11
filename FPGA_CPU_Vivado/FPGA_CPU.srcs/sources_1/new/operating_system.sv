`timescale 1ns / 1ps

module operating_system(
    input logic clk,
    input logic reset,
    input logic btnu,
    input logic rx,
    output logic tx,
    output logic[15:0] led
    );
    
    logic [5:0] instruction_byte_counter;
    logic [47:0] instruction_buffer;
    
    logic [31:0] instruction_write_address;
    logic [47:0] instruction_write_data;
    logic instruction_write_enable;
    logic cpu_run;
    CPU CPU1 (
        .clk(clk),
        .reset(reset),
        .instruction_write_address(instruction_write_address),
        .instruction_write_data(instruction_write_data),
        .instruction_write_enable(instruction_write_enable),
        .run(cpu_run),
        .led(led)
    );
    
    logic [7:0] rx_out;
    logic rx_data_ready;
    serial_receiver serial_receiver (
        .clk(clk),
        .reset(reset),
        .rx(rx),
        .data_out(rx_out),
        .data_ready(rx_data_ready)
    );
    
    logic tx_send;
    logic [7:0] tx_in;
    logic tx_busy;
    serial_transmitter serial_transmitter (
        .clk(clk),
        .reset(reset),
        .send(tx_send),
        .data_in(tx_in),
        .tx(tx),
        .busy(tx_busy)
    );
    
    typedef enum logic[2:0] {
        RESET = 0,
        INIT_HANDSHAKE_RECEIVING,
        INIT_HANDSHAKE_WAITING,
        INIT_HANDSHAKE_START_TRANSMITTING,
        INIT_HANDSHAKE_TRANSMITTING,
        INIT_RECEIVING_PROGRAM,
        PROGRAM_WAITING,
        PROGRAM_RUNNING,
        PROGRAM_HALTED
    } state;
    state system_state;
    state next_state;
    
    always_ff @(posedge clk) begin
    if (reset) begin
        system_state <= RESET;
        led <= '1;
        tx_send <= 0;
        tx_in <= 0;
        cpu_run <= 0;
    end
    else begin
        case (system_state)
            RESET: begin
                system_state <= INIT_HANDSHAKE_WAITING;
                led <= '0;
            end
            
            INIT_HANDSHAKE_WAITING: begin
                if (!rx) system_state <= INIT_HANDSHAKE_RECEIVING;
            end
            
            INIT_HANDSHAKE_RECEIVING:
                if (rx_data_ready)
                    if (rx_out == 8'h50) // 'P'
                        system_state <= INIT_HANDSHAKE_START_TRANSMITTING;
                    else system_state <= INIT_HANDSHAKE_WAITING;
                    
            INIT_HANDSHAKE_START_TRANSMITTING: begin
                tx_in <= 8'h41; // 'A'
                tx_send <= 1;
                system_state <= INIT_HANDSHAKE_TRANSMITTING;
            end
            
            INIT_HANDSHAKE_TRANSMITTING: begin
                tx_send <= 0;
                if (!tx_busy)
                    system_state <= INIT_RECEIVING_PROGRAM;
            end
            
            INIT_RECEIVING_PROGRAM: begin
                if (rx_data_ready) begin
                    instruction_buffer <= {instruction_buffer[39:0], rx_out};  // Shift in byte
                    instruction_byte_counter <= instruction_byte_counter + 1;
                    if (instruction_byte_counter == 5) begin
                        instruction_write_data <= instruction_buffer;  // Load buffer into write_data
                        instruction_write_enable <= 1;                // Enable memory write
                        instruction_write_address <= instruction_write_address + 1;  // Increment write address
                        instruction_byte_counter <= 0;
                    end 
                    else begin
                        if (instruction_buffer == '1) system_state <= PROGRAM_WAITING;
                        instruction_write_enable <= 0;  // Disable memory write until instruction is complete
                    end
                end
            end
            PROGRAM_WAITING:
                if (btnu) system_state <= PROGRAM_RUNNING;
            PROGRAM_RUNNING:
                cpu_run <= 1;
            PROGRAM_HALTED:; 
            
        endcase
    end
    end
    
endmodule
