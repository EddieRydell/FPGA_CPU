# OS, Pipelined CPU, and Floating-Point Unit (FPU) Design

## Overview

This project implements a **top-level operating system (OS)**, a **pipelined CPU**, and a **Floating-Point Unit (FPU)**. The system is designed to perform both integer and floating-point operations with support for floating-point arithmetic based on the **IEEE 754** standard. The **OS** controls the **CPU** and its interaction with the **FPU**, allowing for controlled execution of programs through the operating system.

## System Architecture

The system consists of the following major components:

1. **Top-Level OS Module**  
   The OS module manages the overall state of the CPU and its communication with external devices (such as UART for receiving instructions). The OS handles system calls and transitions the CPU between different states.

2. **Pipelined CPU**  
   The CPU is a multi-stage, pipelined processor capable of executing a variety of integer and floating-point operations. It connects to the FPU for floating-point calculations and includes a stack for managing function calls, returns, and other stack-based operations. It also has a module for hazard detection and stalling to mitigate pipeline hazards.

3. **FPU Module**  
   The FPU performs floating-point arithmetic operations using a multi-cycle state machine. The FPU can handle floating-point addition, subtraction, multiplication, and division. Additional operations such as **FTOI (float-to-integer)** and **ITOF (integer-to-float)** are supported.

**Additional Floating Point Operations**: Trigonometric functions (e.g., sine, cosine), square root, exponentiation, logarithms.

## Instruction Set

Each instruction is 48 bits long, consisting of an **op code** (4 bits), **function code** (4 bits), two **arguments** (4 bits each), and an **immediate value** (32 bits). The CPU architecture is 32-bit and supports reading and writing from main memory by interfacing with the operating system. Below is a diagram of the structure of each instruction:


Currently, there are 12 different op codes corresponding to different CPU operations. They are:
<pre><code>
OP_NOP      4'h0
OP_HLT      4'h1
OP_BINARY   4'h2
OP_UNARY    4'h3
OP_MOV      4'h4
OP_JMP      4'h5
OP_CALL     4'h6
OP_RET      4'h7
OP_PUSH     4'h8
OP_POP      4'h9
OP_SYSCALL  4'ha
OP_BINARYF  4'hb
OP_UNARYF   4'hc
</code></pre>
