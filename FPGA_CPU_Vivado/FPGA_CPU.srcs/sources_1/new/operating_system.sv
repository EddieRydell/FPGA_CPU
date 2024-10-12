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
    
    // Data Memory Module
    logic data_write_enable;
    logic data_read_enable;
    logic [31:0] data_memory_to_write;
    logic [31:0] data_memory_accessed;
    logic [31:0] data_memory_address;
    data_memory #(.MEMORY_DEPTH(1024)) data_memory (
        .data_accessed(data_memory_accessed),
        .data_to_write(data_memory_to_write),
        .access_address(data_memory_address),
        .read_enable(data_read_enable),
        .write_enable(data_write_enable),
        .clk(clk)
    );
    
    // Instruction Memory Module
    logic instruction_write_enable;
    logic [47:0] instruction_to_write;
    logic [31:0] instruction_write_address;
    logic [47:0] instruction_from_memory;
    logic [31:0] instruction_read_address;
    instruction_memory #(.MEMORY_DEPTH(256)) instruction_memory (
        .program_counter(instruction_read_address),
        .write_address(instruction_write_address),
        .write_data(instruction_to_write),
        .write_enable(instruction_write_enable),
        .clk(clk),
        .instruction(instruction_from_memory)
    );
    
    // FPU Module
    logic [31:0] FPU_result;
    logic FPU_valid_in;
    logic FPU_valid_out;
    logic [3:0] FPU_flags;
    logic [31:0] FPU_op1;
    logic [31:0] FPU_op2;
    logic [3:0] FPU_op_code;
    logic [3:0] FPU_function_code;
    logic FPU_enable;
    logic FPU_ready;
    FPU FPU (
        .result(FPU_result),
        .valid_in(FPU_valid_in),
        .valid_out(FPU_valid_out),
        .flags(FPU_flags),
        .op1(FPU_op1),
        .op2(FPU_op2),
        .op_code(FPU_op_code),
        .function_code(FPU_function_code),
        .enable(FPU_enable),
        .ready(FPU_ready),
        .clk(clk),
        .reset(reset)
    );
    
    logic cpu_run;
    logic syscall_enable;
    logic [3:0] syscall_function_code;
    logic [31:0] syscall_data;
    
    logic [3:0] CPU_status;
    logic CPU_status_write_enable;
    CPU CPU (
        .clk(clk),
        .reset(reset),
        
        .memory_data_accessed(data_memory_accessed),
        .memory_data_to_write(data_memory_to_write),
        .memory_access_address(data_memory_address),
        .memory_read_enable(data_read_enable),
        .memory_write_enable(data_write_enable),
        
        .instruction_data(instruction_from_memory),
        .program_counter(instruction_read_address),
        
        .FPU_result(FPU_result),
        .FPU_valid_in(FPU_valid_in),
        .FPU_valid_out(FPU_valid_out),
        .FPU_flags(FPU_flags),
        .FPU_op1(FPU_op1),
        .FPU_op2(FPU_op2),
        .FPU_op_code(FPU_op_code),
        .FPU_function_code(FPU_function_code),
        .FPU_enable(FPU_enable),
        .FPU_ready(FPU_ready),
        
        .syscall_result(),
        .syscall_valid_in(),
        .syscall_valid_out(),
        .syscall_function_code(),
        .syscall_data(),
        .syscall_enable(),
        .syscall_ready(),
        
        .OS_status_in(CPU_status),
        .OS_status_write_enable(CPU_status_write_enable),
        .current_status()
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
    
    OS_state_t system_state;
    
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
                led <= instruction_buffer[15:0];
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
            PROGRAM_WAITING: begin
                led <= '1;
                if (btnu) begin
                    // led <= '0;
                    system_state <= PROGRAM_RUNNING;
                end
            end
            PROGRAM_RUNNING: begin
                cpu_run <= 1;
                if (syscall_enable) begin
                    case (syscall_function_code)
                        `FUN_LED_WRITE:
                            led <= syscall_data[15:0];
                        `FUN_HALT: begin
                            cpu_run <= 0;
                            system_state <= PROGRAM_HALTED;
                        end
                    endcase
                end
            end
            PROGRAM_HALTED:
                led <= 32'hAAAAAAAA;
        endcase
    end
    end
    
endmodule
