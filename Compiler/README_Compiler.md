# Custom Compiler

My compiler is made to implement a programming language that I have decided to call ***C--*** (C minus minus). This is due to the fact that this language lacks many of the features of **C**, but I plan to continue adding more functionality to the language as time goes on (and even possibly experiment with new features like custom memory models for pointers/integers).

## Compilation

The compilation process generally follows these steps:
### 1. Tokenization (Lexing)

The lexer (lexer.cpp) converts the input source code into a series of tokens, which are fed into the parser. It handles basic syntax like variable declarations, function calls, operators, and control flow constructs (e.g., if, while).

### 2. Parsing and Abstract Syntax Tree (AST) generation

The parser (parser.cpp) processes the token stream and constructs an Abstract Syntax Tree (AST), which represents the structure of the input program. The documentation for the grammar of this language can be found [here](./grammar.md).

### 3. Conversion to an Intermediate Representation (IR)

Once the AST is generated, the IR generator (ir_generator.cpp) converts it into an Intermediate Representation. The IR is a linear, low-level representation of the program, which simplifies code generation and optimization.

### 4. Optimization (eventually)

Still a work in progress.

### 5. Assembly code generation

The codegen module (codegen.cpp) translates the IR into assembly code for the custom instruction set. This assembly code can then be passed to the assembler for binary conversion and execution on the FPGA.
