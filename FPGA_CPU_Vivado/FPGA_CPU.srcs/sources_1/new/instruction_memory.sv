module instruction_memory #(parameter MEMORY_DEPTH = 256) (
    input logic[31:0] program_counter,
    input logic[31:0] write_address,
    input logic[47:0] write_data,
    input logic write_enable,
    output logic[47:0] instruction
    );
    
    logic [47:0] instruction_memory[0:MEMORY_DEPTH -1];
    
    // Reading from instruction memory (for normal operation)
    assign instruction = instruction_memory[program_counter];
    
    // Writing to instruction memory (when receiving program via UART)
    always_ff @(posedge clk) begin
        if (write_enable)
            instruction_memory[write_address] <= write_data;
    end
endmodule
