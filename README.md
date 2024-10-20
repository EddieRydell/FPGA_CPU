# Compiler, Assembler, and FPGA Architecture Implementation

This project has 3 main components:

### 1. **Pipelined CPU and FPU implemented on an FPGA board**
This portion of the project implements a **top-level operating system (OS)**, a **pipelined CPU**, and a **Floating-Point Unit (FPU)**. The system is designed to perform both integer and floating-point operations with support for floating-point arithmetic based on the **IEEE 754** standard. The **OS** controls the **CPU** and its interaction with the **FPU**, allowing for controlled execution of programs through the operating system.

### 2. **Assembler for the CPU's custom instruction set**
Because I created a custom instruction set for this project, I also had to create a custom **assembler**. In the `Assembler` directory of this repository, there is a **C++** program made to assemble programs and then transmit them over **UART** to the FPGA. More details on the custom instruction set can be found [here](./Assembler/README_Assembler.md).

### 3. **Compiler for a custom C-based language**
This component involves creating a **compiler** for a custom C-based programming language designed to interface with the **CPU** and **FPU**. The compiler translates high-level C-like programs into an intermediate representation, and then ultimately into an assembly language that uses the instruction set used by the CPU. More details on the compiler and its features can be found [here](./Compiler/README_Compiler.md).

## System Architecture

The system consists of the following major components:

1. **Top-Level OS Module**  
   The OS module manages the overall state of the CPU and its communication with external devices (such as UART for receiving instructions). The OS handles system calls and transitions the CPU between different states.

2. **Pipelined CPU**  
   The CPU is a multi-stage, pipelined processor capable of executing a variety of integer and floating-point operations. It connects to the FPU for floating-point calculations and includes a stack for managing function calls, returns, and other stack-based operations. It also has a module for hazard detection and stalling to mitigate pipeline hazards (still in progress).

3. **FPU Module (not yet fully implemented)**  
   The FPU performs floating-point arithmetic operations using a multi-cycle state machine. The FPU can handle floating-point addition, subtraction, multiplication, and division. Additional operations such as **FTOI (float-to-integer)** and **ITOF (integer-to-float)** are supported.

**Additional Floating Point Operations**: Trigonometric functions (e.g., sine, cosine), square root, exponentiation, logarithms.

## Building the Project
Unfortunately, because this project requires specific hardware, it is almost impossible for someone to build it themself. 

If you want to try, you will need to modify the constraints.xdc file found in FPGA_CPU_Vivado/FPGA_CPU.srcs/constrs_1/new to fit the FPGA you have. It is currently written for a Xilinx Basys 3 board (part number xc7a35ticpg236-1L). The pins you need to define in your constraints file are the ones declared as inputs/outputs of operating_system.sv (the top level module).

## Future
This project is still a work in progress. I will continue to update this repository regularly as I finish implementing the FPU and hazard detection system.

