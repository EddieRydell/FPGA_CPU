# Operating System, Pipelined CPU, and Floating-Point Unit (FPU) FPGA Design

## Overview

This project implements a **top-level operating system (OS)**, a **pipelined CPU**, and a **Floating-Point Unit (FPU)**. The system is designed to perform both integer and floating-point operations with support for floating-point arithmetic based on the **IEEE 754** standard. The **OS** controls the **CPU** and its interaction with the **FPU**, allowing for controlled execution of programs through the operating system.

## System Architecture

The system consists of the following major components:

1. **Top-Level OS Module**  
   The OS module manages the overall state of the CPU and its communication with external devices (such as UART for receiving instructions). The OS handles system calls and transitions the CPU between different states.

2. **Pipelined CPU**  
   The CPU is a multi-stage, pipelined processor capable of executing a variety of integer and floating-point operations. It connects to the FPU for floating-point calculations and includes a stack for managing function calls, returns, and other stack-based operations. It also has a module for hazard detection and stalling to mitigate pipeline hazards (still in progress).

3. **FPU Module (not yet fully implemented)**  
   The FPU performs floating-point arithmetic operations using a multi-cycle state machine. The FPU can handle floating-point addition, subtraction, multiplication, and division. Additional operations such as **FTOI (float-to-integer)** and **ITOF (integer-to-float)** are supported.

**Additional Floating Point Operations**: Trigonometric functions (e.g., sine, cosine), square root, exponentiation, logarithms.

## Instruction Set

Each instruction is 48 bits long, consisting of an **op code** (4 bits), a **function code** (4 bits), two **arguments** (4 bits each), and an **immediate value** (32 bits), as shown below:

**| 4-bit opcode | 4-bit funcode | 4-bit arg1 | 4-bit arg2 | 32-bit immediate |**

The CPU architecture is 32-bit and supports reading and writing from main memory by interfacing with the operating system. It is mostly a load-store architecture, but I also allow for mov instructions to access memory in addition to the load and store instructions. Currently, there are 12 different op codes corresponding to different CPU operations. They are defined in a SystemVerilog header (opcodes.vh) as:
<pre><code>
`define OP_NOP      4'h0
`define OP_HLT      4'h1
`define OP_BINARY   4'h2
`define OP_UNARY    4'h3
`define OP_MOV      4'h4
`define OP_JMP      4'h5
`define OP_CALL     4'h6
`define OP_RET      4'h7
`define OP_PUSH     4'h8
`define OP_POP      4'h9
`define OP_SYSCALL  4'ha
`define OP_BINARYF  4'hb
`define OP_UNARYF   4'hc
`define OP_LOAD     4'hd
`define OP_STORE    4'he
</code></pre>
Some op codes also have corresponding function codes, such as OP_BINARY, which takes different binary operations and applies them to the registers specified in the arguments. These are definned in opcodes.vh as:
<pre><code>
// function codes for binary operators
`define FUN_ADD     4'h0
`define FUN_SUB     4'h1
`define FUN_MULT    4'h2
`define FUN_DIV     4'h3
`define FUN_AND     4'h4
`define FUN_OR      4'h5
`define FUN_XOR     4'h6
`define FUN_CMP     4'h7
</code></pre>
The rest of the function codes corresponding to different op codes are also found in opcodes.vh. This header also includes a list of each possible state of the CPU, as well as different flags that can be set by the ALU. 

## Assembler
Because I created a custom instruction set for this project, I also had to create a custom **assembler**. In the Assembler file of this repository, there is a **C++** program made to assemble programs and then transmit them over **UART** to the FPGA. The format for my custom assembly language is demonstrated in the following code snippet to add two values and display their result on the FPGA led array:
<pre><code>
mov $65535 %r0      ; registers are prefixed with '%'
mov $65535 %r1      ; immediate values are prefixed with '$'
add %r1 %r0
syscall led_write   ; different syscalls are hardcoded into my assembler
nop
hlt
</code></pre>
This assembler is still not complete. I plan to flesh it out more soon, and possibly even write my own compiler in the future.

## Building the Project
Unfortunately, because this project requires specific hardware, it is almost impossible for someone to build it themself. 

If you want to try, you will need to modify the constraints.xdc file found in FPGA_CPU_Vivado/FPGA_CPU.srcs/constrs_1/new to fit the FPGA you have. It is currently written for a Xilinx Basys 3 board (part number xc7a35ticpg236-1L). The pins you need to define in your constraints file are the ones declared as inputs/outputs of operating_system.sv (the top level module).

## Future
This project is still a work in progress. I will continue to update this repository regularly as I finish implementing the FPU and hazard detection system.

