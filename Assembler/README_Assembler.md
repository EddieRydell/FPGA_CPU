## Instruction Set

For this assembler, each instruction is 48 bits long, consisting of an **op code** (4 bits), a **function code** (4 bits), two **arguments** (4 bits each), and an **immediate value** (32 bits), as shown below:

**| 4-bit opcode | 4-bit funcode | 4-bit arg1 | 4-bit arg2 | 32-bit immediate |**

The CPU architecture is 32-bit and supports reading and writing from main memory by interfacing with the operating system. It is mostly a load-store architecture, but I also allow for mov instructions to access memory in addition to the load and store instructions. Currently, there are 12 different op codes corresponding to different CPU operations. They are defined in a SystemVerilog header `opcodes.vh` as:
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
Some op codes also have corresponding function codes, such as OP_BINARY, which takes different binary operations and applies them to the registers specified in the arguments. These are definned in `opcodes.vh` as:
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
The rest of the function codes corresponding to different op codes are also found in `opcodes.vh`. This header also includes a list of each possible state of the CPU, as well as different flags that can be set by the ALU. 

## Example Program

This is a small example program to demonstrate the basic syntax of my custom assembly language:

<pre><code>
mov $65535 %r0      ; registers are prefixed with '%'
mov $65535 %r1      ; immediate values are prefixed with '$'
add %r1 %r0
syscall led_write   ; different syscalls are hardcoded into my assembler
nop
hlt
</code></pre>
This assembler is still not complete. I plan to flesh it out more soon, and possibly even write my own compiler in the future.

