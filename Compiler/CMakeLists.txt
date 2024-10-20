cmake_minimum_required(VERSION 3.28)
project(Compiler)

set(CMAKE_CXX_STANDARD 17)

# Add include directories for headers
include_directories(headers)

# Add executable and specify the source files
add_executable(Compiler
        main.cpp
        src/tokenizer.cpp
        src/parser.cpp
        headers/parser.h
        headers/node_types/ast_node.h
        headers/node_types/function_node.h
        headers/node_types/statement_node.h
        headers/node_types/assignment_node.h
        headers/node_types/expression_node.h
        headers/node_types/binary_expression_node.h
        headers/node_types/number_node.h
        headers/node_types/ast_node_types.h
        headers/node_types/program_node.h
        headers/node_types/return_node.h
        headers/node_types/variable_node.h
        headers/node_types/parameter_node.h
        headers/node_types/scope_node.h
        headers/node_types/declaration_node.h
        headers/node_types/if_node.h
        headers/node_types/while_node.h
        headers/node_types/unary_expression_node.h
)
