#pragma once

#include "tokenizer.h"
#include "node_types/ast_node_types.h"
#include <memory>
#include <vector>
#include <string>

class parser {
public:
    parser(const std::vector<token>& tokens);

    std::shared_ptr<program_node> parse_program(); // Parse the entire program

private:
    std::vector<token> tokens;
    size_t current_token_index;

    // Token utilities
    token current_token() const;
    void advance();
    bool match(token_type type);
    bool is_type_keyword(token_type type);

    // Parsing functions
    std::shared_ptr<function_node> parse_function();
    std::vector<std::shared_ptr<parameter_node>> parse_parameter_list();
    std::shared_ptr<parameter_node> parse_typed_identifier();
    std::string parse_type();

    std::shared_ptr<scope_node> parse_scope();

    std::shared_ptr<statement_node> parse_statement();
    std::shared_ptr<statement_node> parse_declaration_statement();
    std::shared_ptr<statement_node> parse_assignment_statement();
    std::shared_ptr<statement_node> parse_return_statement();
    std::shared_ptr<statement_node> parse_if_statement();
    std::shared_ptr<statement_node> parse_while_statement();

    // Expression parsing with operator precedence
    std::shared_ptr<expression_node> parse_expression();
    std::shared_ptr<expression_node> parse_assignment_expression();
    std::shared_ptr<expression_node> parse_logical_or_expression();
    std::shared_ptr<expression_node> parse_logical_and_expression();
    std::shared_ptr<expression_node> parse_equality_expression();
    std::shared_ptr<expression_node> parse_relational_expression();
    std::shared_ptr<expression_node> parse_additive_expression();
    std::shared_ptr<expression_node> parse_multiplicative_expression();
    std::shared_ptr<expression_node> parse_unary_expression();
    std::shared_ptr<expression_node> parse_primary_expression();
};


