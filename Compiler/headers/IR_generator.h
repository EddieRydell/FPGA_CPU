#pragma once

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <memory>
#include "node_types/ast_node_types.h"

class IR_generator {
public:
    IR_generator();

    // Generate IR for the entire program
    llvm::Module* generate_program(std::shared_ptr<program_node> program);

    // Generate IR for a function
    llvm::Function* generate_function(std::shared_ptr<function_node> func_node);

    // Generate IR for statements
    llvm::Value* generate_statement(std::shared_ptr<statement_node> stmt);

    // Generate IR for expressions
    llvm::Value* generate_expression(std::shared_ptr<expression_node> expr);

private:
    llvm::LLVMContext context;
    std::unique_ptr<llvm::Module> module;
    llvm::IRBuilder<> builder;

    // Helper methods to get LLVM types and handle code generation details
    llvm::Type* get_llvm_type(const std::string& type);
    llvm::Value* generate_declaration(std::shared_ptr<declaration_node> decl);
    llvm::Value* generate_assignment(std::shared_ptr<assignment_node> assign);
    llvm::Value* generate_if_statement(std::shared_ptr<if_node> if_stmt);
    llvm::Value* generate_while_statement(std::shared_ptr<while_node> while_stmt);
};

