module instruction_memory #(parameter MEMORY_DEPTH = 256) (
    input logic[31:0] program_counter,
    input logic[31:0] write_address,
    input logic[47:0] write_data,
    input logic write_enable,
    input logic clk,
    input logic reset,
    output logic[47:0] instruction
    );
    
    logic [47:0] instruction_memory[0:MEMORY_DEPTH -1];
    
    always_ff @(posedge clk) begin
        if (reset) begin
            instruction_memory[0] <= 48'h40000000ffff;
            instruction_memory[1] <= 48'h40100000ffff;
            instruction_memory[2] <= 48'h000000000000;
            instruction_memory[3] <= 48'h000000000000;
            instruction_memory[4] <= 48'h000000000000;
            instruction_memory[5] <= 48'h201000000000;
            instruction_memory[6] <= 48'h000000000000;
            instruction_memory[7] <= 48'h000000000000;
            instruction_memory[8] <= 48'h000000000000;
            instruction_memory[9] <= 48'ha00000000000;
            instruction_memory[10] <= 48'h000000000000;
            instruction_memory[11] <= 48'h000000000000;
            instruction_memory[12] <= 48'h000000000000;
            instruction_memory[13] <= 48'h000000000000;
            instruction_memory[14] <= 48'h100000000000;
        end
        else if (write_enable)
            instruction_memory[write_address] <= write_data;
        else
            instruction <= instruction_memory[program_counter];
    end
    
    initial begin
        
        
    end
endmodule
