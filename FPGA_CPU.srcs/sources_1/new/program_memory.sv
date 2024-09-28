`timescale 1ns / 1ps

module program_memory #(parameter MEMORY_DEPTH = 256) (
    input logic[31:0] program_counter,
    output logic[47:0] instruction
    );
    
    logic [47:0] instruction_memory[0:MEMORY_DEPTH -1];
    
    assign instruction = instruction_memory[program_counter];
    
    initial begin
        instruction_memory[0] = 48'h400000000002;
        instruction_memory[1] = 48'h401000000005;
        instruction_memory[2] = 48'h000000000000;
        instruction_memory[3] = 48'h000000000000;
        instruction_memory[4] = 48'h200100000000;
        
    end

endmodule
