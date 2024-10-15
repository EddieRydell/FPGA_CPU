`timescale 1ns / 1ps

module operating_system(
    input logic clk,
    input logic reset,
    input logic btnu,
    input logic btnd,
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
        .reset(reset),
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
    
    logic [31:0] syscall_result;
    logic syscall_enable;
    logic syscall_ready;
    logic syscall_valid_in;
    logic syscall_valid_out;
    logic [31:0] syscall_data;
    logic [31:0] syscall_registers[0:15];
    
    logic [3:0] CPU_status_to_set;
    logic [3:0] CPU_current_status;
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
        .instruction_address(instruction_read_address),
        
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
        
        .syscall_result(syscall_result),
        .syscall_valid_in(syscall_valid_in),
        .syscall_valid_out(syscall_valid_out),
        .syscall_data(syscall_data),
        .syscall_enable(syscall_enable),
        .syscall_ready(syscall_ready),
        
        .OS_status_in(CPU_status_to_set),
        .OS_status_write_enable(CPU_status_write_enable),
        .current_status(CPU_current_status),
        
        .register(syscall_registers)
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
    
    enum logic [3:0] {
        BOOTUP,
        IDLE,
        RECEIVING_PROGRAM,
        EXECUTING_PROGRAM,
        FINISHED_PROGRAM,
        BEGINNING_SYSCALL,
        HANDLING_SYSCALL,
        DONE_HANDLING_SYSCALL
    } system_state;
    
    enum logic [2:0] {
        INIT,
        HANDSHAKE_RECEIVING,
        HANDSHAKE_TRANSMITTING,
        RECEIVING_BYTES
    } UART_state;
    
    logic [32:0] counter;
    always_ff @(posedge clk) begin
        if (reset) begin
            system_state <= BOOTUP;
            led <= '1;
            tx_send <= 0;
            tx_in <= 0;
            counter <= 0;
            UART_state <= INIT;
        end
        else begin
            case (system_state)
                BOOTUP: begin
                    system_state <= IDLE;
                    led <= 0;
                end
                
                IDLE: begin
                    counter <= counter + 1;
                    if (led == 0) led <= 1;
                    if (counter == 10000000) begin
                        led <= led << 1;
                        counter <= 0;
                    end
                    if (!rx) begin
                        // system_state <= RECEIVING_PROGRAM;
                    end
                    if (btnu) begin
                        CPU_status_write_enable <= 1;
                        CPU_status_to_set <= CPU_RUNNING;
                        system_state <= EXECUTING_PROGRAM;
                    end
                end
                
                RECEIVING_PROGRAM: begin
                    case (UART_state)
                        INIT: begin
                            UART_state <= HANDSHAKE_RECEIVING;
                        end
                        
                        HANDSHAKE_RECEIVING: begin
                            if (rx_data_ready) begin
                                if (rx_out == 8'h50) begin // 'P'
                                    tx_in <= 8'h41; // 'A'
                                    tx_send <= 1;
                                    UART_state <= HANDSHAKE_TRANSMITTING;
                                end
                                else begin
                                    UART_state <= INIT;
                                    system_state <= IDLE;
                                end
                            end
                        end
                        
                        HANDSHAKE_TRANSMITTING: begin
                            tx_send <= 0;
                            if (!tx_busy)
                                UART_state <= RECEIVING_BYTES;
                        end
                        
                        RECEIVING_BYTES: begin
                            
                            
                        end
                    endcase
                end
                
                EXECUTING_PROGRAM: begin
                    CPU_status_write_enable <= 0;
                    if (syscall_enable) begin
                        system_state <= BEGINNING_SYSCALL;
                    end
                    if (CPU_current_status == CPU_HALTED) begin
                        system_state <= FINISHED_PROGRAM;
                    end
                end
                
                FINISHED_PROGRAM: begin
                    if (btnd) begin
                        system_state <= IDLE;
                    end
                end
                
                BEGINNING_SYSCALL: begin
                    syscall_valid_in <= 1;
                    syscall_valid_out <= 0;
                    system_state <= HANDLING_SYSCALL;
                end
                
                HANDLING_SYSCALL: begin
                    case (syscall_data)
                        default: begin
                            syscall_valid_out <= 1;
                            syscall_result <= 0;
                            led <= syscall_registers[0][15:0];
                            system_state <= DONE_HANDLING_SYSCALL;
                        end
                    endcase
                end
                
                DONE_HANDLING_SYSCALL: begin
                    if (syscall_ready) begin
                        system_state <= EXECUTING_PROGRAM;
                    end
                end
            endcase
        end
    end
    
endmodule
