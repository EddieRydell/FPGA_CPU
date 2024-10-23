cmake_minimum_required(VERSION 3.28)
project(Compiler)

set(CMAKE_CXX_STANDARD 17)

# Find LLVM package
set(LLVM_DIR "C:/Users/eddie/LLVM/clang+llvm-19.1.2-x86_64-pc-windows-msvc/lib/cmake/llvm")
find_package(LLVM REQUIRED CONFIG)
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})
llvm_map_components_to_libnames(LLVM_LIBS support core irreader)

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
        headers/node_types/function_call_node.h
        src/IR_generator.cpp
        headers/IR_generator.h
)

target_link_libraries(Compiler ${LLVM_LIBS})
