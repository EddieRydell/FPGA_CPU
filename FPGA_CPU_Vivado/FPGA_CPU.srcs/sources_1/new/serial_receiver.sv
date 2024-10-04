`timescale 1ns / 1ps

module serial_receiver #(
    parameter CLOCK_FREQ = 50000000,   // 50 MHz clock
    parameter BAUD_RATE = 115200
)(
    input logic clk,                   // System clock
    input logic reset,                 // Reset signal
    input logic rx,                    // Serial data input (UART RX)
    output logic [47:0] data_out,      // 48-bit parallel output
    output logic data_ready            // Data ready flag
    );

    // Baud rate counter and calculation
    localparam integer BAUD_TICK = CLOCK_FREQ / BAUD_RATE;

    logic [15:0] baud_counter;         // Counter for baud rate timing
    logic [5:0] bit_counter;           // Counts up to 48 bits (6 bytes)
    logic [47:0] shift_reg;            // Shift register for receiving data
    logic rx_prev;                     // Used for start bit detection
    logic receiving;                   // Indicates reception is in progress

    // Synchronous logic for receiving UART data
    always_ff @(posedge clk) begin
        if (reset) begin
            baud_counter <= 0;
            bit_counter <= 0;
            data_ready <= 0;
            shift_reg <= 48'b0;        // Clear shift register
            receiving <= 0;
            rx_prev <= 1;              // Assume idle state (rx high)
        end 
        else begin
            // Detect falling edge of RX (start bit detection)
            if (!receiving && !rx && rx_prev) begin
                receiving <= 1;        // Start receiving when start bit is detected
                baud_counter <= BAUD_TICK / 2;  // Align to the middle of the start bit
                bit_counter <= 0;
                data_ready <= 0;
            end

            // Receiving process
            if (receiving) begin
                if (baud_counter == 0) begin
                    baud_counter <= BAUD_TICK - 1;  // Reset baud counter

                    if (bit_counter < 48) begin
                        // Shift the received bit into the shift register
                        shift_reg <= {rx, shift_reg[47:1]};
                        bit_counter <= bit_counter + 1;
                    end 
                    else begin
                        // 48 bits received, stop receiving and output data
                        data_out <= shift_reg;
                        data_ready <= 1;
                        receiving <= 0;
                    end
                end 
                else begin
                    baud_counter <= baud_counter - 1;
                end
            end

            rx_prev <= rx;
        end
    end
endmodule
