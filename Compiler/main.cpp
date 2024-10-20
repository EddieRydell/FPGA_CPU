#include <iostream>
#include <string>
#include <vector>
#include <memory>

// Include your tokenizer and parser headers
#include "tokenizer.h"
#include "parser.h"
#include "node_types/ast_node_types.h"

int main() {
    // Define a sample input program as a string
    std::string input_program = R"(
        function factorial(int n) {
            if (n <= 1) {
                return 1;
            }
            return n * factorial(n - 1);
        }
        function main() {
            int result;
            result = factorial(5);
            return result;
        }
    )";

    // Create a tokenizer and tokenize the input program
    tokenizer tk(input_program);
    std::vector<token> tokens = tk.tokenize();

    // Optionally, print the tokens
    // ...

    // Create a parser with the tokens
    parser p(tokens);

    try {
        // Parse the program to build the AST
        std::shared_ptr<program_node> ast = p.parse_program();

        // Display the AST
        std::cout << "Abstract Syntax Tree:" << std::endl;
        ast->display();
    } catch (const parser_exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
