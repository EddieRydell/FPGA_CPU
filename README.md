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
