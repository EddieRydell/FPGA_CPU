# Custom C-Based Compiler

## Overview

This project includes a **custom C-based compiler** for a programming language that I have decided to call ***C--*** (C minus minus).

### Features
- **Custom Instruction Set**: The compiler translates programs written in a C-like language into a custom instruction set designed for a custom CPU.
- **Floating-Point Support**: The compiler fully supports floating-point operations, following the IEEE 754 standard.
- **Custom Assembler**: The assembler translates the compiler's output into binary format and transmits it via **UART** to the FPGA.
- **Basic Optimizations**: The compiler performs basic optimizations during the translation process to improve performance.
- **Intermediate Representation (IR)**: The compiler utilizes an IR to simplify the generation of assembly code and facilitate optimizations.

---
