// irgenerator.cpp

#include "IR_generator.h"
#include "node_types/ast_node_types.h" // Ensure this path is correct
#include <llvm/IR/Verifier.h>
#include <llvm/Support/raw_ostream.h>
#include <iostream>

// Constructor
IR_generator::IR_generator()
        : context(), builder(context), module(std::make_unique<llvm::Module>("main_module", context)) {
    // Initialize the module and IRBuilder if needed
}

// Generate IR for the entire program
llvm::Module* IR_generator::generate_program(std::shared_ptr<program_node> program) {
    // Iterate over each function in the program and generate IR
    for (const auto& func_node : program->get_children()) {
        generate_function(func_node);
    }

    // Verify the module for correctness
    if (llvm::verifyModule(*module, &llvm::errs())) {
        std::cerr << "Error constructing function!\n";
        return nullptr;
    }

    return module.release();
}

// Generate IR for a function
llvm::Function* IR_generator::generate_function(std::shared_ptr<function_node> func_node) {
    // Get the function name, parameters, and return type using getters
    const std::string& func_name = func_node->get_name();
    const auto& parameters = func_node->get_parameters(); // Vector of type-identifier pairs
    const std::string& return_type_str = func_node->get_return_type();

    // Convert parameter types to LLVM types
    std::vector<llvm::Type*> param_types;
    for (const auto& param : parameters) {
        llvm::Type* llvm_type = get_llvm_type(param.first);
        if (!llvm_type) {
            std::cerr << "Unknown parameter type: " << param.first << " in function: " << func_name << "\n";
            return nullptr;
        }
        param_types.push_back(llvm_type);
    }

    // Convert return type to LLVM type
    llvm::Type* ret_type = get_llvm_type(return_type_str);
    if (!ret_type) {
        std::cerr << "Unknown return type: " << return_type_str << " in function: " << func_name << "\n";
        return nullptr;
    }

    // Create the LLVM function type
    llvm::FunctionType* func_type = llvm::FunctionType::get(ret_type, param_types, false);

    // Create the LLVM function
    llvm::Function* function = llvm::Function::Create(
            func_type, llvm::Function::ExternalLinkage, func_name, module.get());

    // Assign names to the function arguments
    unsigned idx = 0;
    for (auto& arg : function->args()) {
        if (idx < parameters.size()) {
            arg.setName(parameters[idx].second); // Set the argument name
            ++idx;
        } else {
            arg.setName("arg" + std::to_string(idx++)); // Default name if not enough parameters
        }
    }

    // Create a basic block and set the builder's insertion point
    llvm::BasicBlock* entry = llvm::BasicBlock::Create(context, "entry", function);
    builder.SetInsertPoint(entry);

    // Allocate space for each argument and store their values
    idx = 0;
    for (auto& arg : function->args()) {
        llvm::AllocaInst* alloc = create_entry_block_alloca(function, arg.getName().str(), arg.getType());
        builder.CreateStore(&arg, alloc);
        named_values[arg.getName().str()] = alloc; // Add to symbol table
        ++idx;
    }

    // Generate IR for each statement in the function body
    for (const auto& stmt : func_node->get_body()) {
        llvm::Value* ret = generate_statement(stmt);
        if (!ret) {
            std::cerr << "Error generating statement in function: " << func_name << "\n";
            return nullptr;
        }
    }

    // Add a default return if not present
    if (ret_type->isVoidTy()) {
        builder.CreateRetVoid();
    } else {
        // Assuming 0 is the default return value for non-void types
        builder.CreateRet(llvm::ConstantInt::get(ret_type, 0));
    }

    // Verify the function
    if (llvm::verifyFunction(*function, &llvm::errs())) {
        std::cerr << "Error constructing function: " << func_name << "\n";
        return nullptr;
    }

    return function;
}


// Generate IR for a statement
llvm::Value* IR_generator::generate_statement(std::shared_ptr<statement_node> stmt) {
    switch (stmt->type) {
        case statement_type::DECLARATION:
            return generate_declaration(std::static_pointer_cast<declaration_node>(stmt));
        case statement_type::ASSIGNMENT:
            return generate_assignment(std::static_pointer_cast<assignment_node>(stmt));
        case statement_type::IF:
            return generate_if_statement(std::static_pointer_cast<if_node>(stmt));
        case statement_type::WHILE:
            return generate_while_statement(std::static_pointer_cast<while_node>(stmt));
            // Add more cases as needed
        default:
            std::cerr << "Unknown statement type!\n";
            return nullptr;
    }
}

// Generate IR for an expression
llvm::Value* IR_generator::generate_expression(std::shared_ptr<expression_node> expr) {
    // Implement expression generation based on expression type
    // Example for binary expressions:
    if (expr->type == expression_type::BINARY) {
        auto bin_expr = std::static_pointer_cast<binary_expression_node>(expr);
        llvm::Value* lhs = generate_expression(bin_expr->lhs);
        llvm::Value* rhs = generate_expression(bin_expr->rhs);
        if (!lhs || !rhs) {
            std::cerr << "Error generating binary expression operands!\n";
            return nullptr;
        }

        switch (bin_expr->op) {
            case binary_op::ADD:
                return builder.CreateAdd(lhs, rhs, "addtmp");
            case binary_op::SUB:
                return builder.CreateSub(lhs, rhs, "subtmp");
            case binary_op::MUL:
                return builder.CreateMul(lhs, rhs, "multmp");
            case binary_op::DIV:
                return builder.CreateSDiv(lhs, rhs, "divtmp");
                // Handle other binary operations
            default:
                std::cerr << "Unknown binary operator!\n";
                return nullptr;
        }
    }
        // Handle other expression types
    else if (expr->type == expression_type::INTEGER_LITERAL) {
        auto int_expr = std::static_pointer_cast<integer_literal_node>(expr);
        return llvm::ConstantInt::get(context, llvm::APInt(32, int_expr->value));
    }
        // Add more expression types as needed
    else {
        std::cerr << "Unknown expression type!\n";
        return nullptr;
    }
}

// Helper method to get LLVM type from string
llvm::Type* IR_generator::get_llvm_type(const std::string& type_str) {
    if (type_str == "int") {
        return llvm::Type::getInt32Ty(context);
    }
    else if (type_str == "float") {
        return llvm::Type::getFloatTy(context);
    }
    else if (type_str == "double") {
        return llvm::Type::getDoubleTy(context);
    }
    else if (type_str == "void") {
        return llvm::Type::getVoidTy(context);
    }
        // Handle other types as needed
    else {
        std::cerr << "Unknown type: " << type_str << "\n";
        return nullptr;
    }
}

// Generate IR for a variable declaration
llvm::Value* IR_generator::generate_declaration(std::shared_ptr<declaration_node> decl) {
    llvm::Type* var_type = get_llvm_type(decl->var_type);
    if (!var_type) {
        std::cerr << "Invalid type in declaration: " << decl->var_type << "\n";
        return nullptr;
    }

    llvm::AllocaInst* alloc = builder.CreateAlloca(var_type, nullptr, decl->var_name);
    // Optionally, initialize the variable to zero or another default value
    builder.CreateStore(llvm::Constant::getNullValue(var_type), alloc);
    return alloc;
}

// Generate IR for an assignment statement
llvm::Value* IR_generator::generate_assignment(std::shared_ptr<assignment_node> assign) {
    // Find the variable (assume it's already allocated)
    llvm::Value* var = module->getNamedValue(assign->var_name);
    if (!var) {
        std::cerr << "Undefined variable: " << assign->var_name << "\n";
        return nullptr;
    }

    // Generate the value to assign
    llvm::Value* value = generate_expression(assign->value);
    if (!value) {
        std::cerr << "Error generating value for assignment to " << assign->var_name << "\n";
        return nullptr;
    }

    // Store the value in the variable
    builder.CreateStore(value, var);
    return value;
}

// Generate IR for an if statement
llvm::Value* IR_generator::generate_if_statement(std::shared_ptr<if_node> if_stmt) {
    // Generate condition expression
    llvm::Value* cond = generate_expression(if_stmt->condition);
    if (!cond) {
        std::cerr << "Error generating condition for if statement!\n";
        return nullptr;
    }

    // Convert condition to a boolean
    llvm::Value* cond_bool = builder.CreateICmpNE(cond, llvm::ConstantInt::get(cond->getType(), 0), "ifcond");

    // Create basic blocks for 'then', 'else', and 'merge'
    llvm::Function* function = builder.GetInsertBlock()->getParent();

    llvm::BasicBlock* then_bb = llvm::BasicBlock::Create(context, "then", function);
    llvm::BasicBlock* else_bb = llvm::BasicBlock::Create(context, "else");
    llvm::BasicBlock* merge_bb = llvm::BasicBlock::Create(context, "ifcont");

    // Create the conditional branch
    builder.CreateCondBr(cond_bool, then_bb, else_bb);

    // Emit 'then' block
    builder.SetInsertPoint(then_bb);
    llvm::Value* then_val = generate_statement(if_stmt->then_branch);
    builder.CreateBr(merge_bb);
    then_bb = builder.GetInsertBlock(); // Update then_bb in case it was changed

    // Emit 'else' block
    function->getBasicBlockList().push_back(else_bb);
    builder.SetInsertPoint(else_bb);
    llvm::Value* else_val = nullptr;
    if (if_stmt->else_branch) {
        else_val = generate_statement(if_stmt->else_branch);
    }
    builder.CreateBr(merge_bb);
    else_bb = builder.GetInsertBlock(); // Update else_bb in case it was changed

    // Emit 'merge' block
    function->getBasicBlockList().push_back(merge_bb);
    builder.SetInsertPoint(merge_bb);

    // Optionally, return a value or continue
    return nullptr; // Adjust based on your needs
}

// Generate IR for a while statement
llvm::Value* IR_generator::generate_while_statement(std::shared_ptr<while_node> while_stmt) {
    llvm::Function* function = builder.GetInsertBlock()->getParent();

    // Create basic blocks for loop condition, body, and after loop
    llvm::BasicBlock* cond_bb = llvm::BasicBlock::Create(context, "loopcond", function);
    llvm::BasicBlock* body_bb = llvm::BasicBlock::Create(context, "loopbody");
    llvm::BasicBlock* after_bb = llvm::BasicBlock::Create(context, "afterloop");

    // Branch to condition block
    builder.CreateBr(cond_bb);

    // Emit condition block
    builder.SetInsertPoint(cond_bb);
    llvm::Value* cond = generate_expression(while_stmt->condition);
    if (!cond) {
        std::cerr << "Error generating condition for while statement!\n";
        return nullptr;
    }

    // Convert condition to a boolean
    llvm::Value* cond_bool = builder.CreateICmpNE(cond, llvm::ConstantInt::get(cond->getType(), 0), "whilecond");

    // Create conditional branch
    builder.CreateCondBr(cond_bool, body_bb, after_bb);

    // Emit body block
    function->getBasicBlockList().push_back(body_bb);
    builder.SetInsertPoint(body_bb);
    llvm::Value* body_val = generate_statement(while_stmt->body);
    builder.CreateBr(cond_bb);
    body_bb = builder.GetInsertBlock(); // Update body_bb in case it was changed

    // Emit after loop block
    function->getBasicBlockList().push_back(after_bb);
    builder.SetInsertPoint(after_bb);

    return nullptr; // Adjust based on your needs
}
