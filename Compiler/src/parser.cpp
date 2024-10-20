#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"

#include "parser.h"
#include "node_types/ast_node_types.h"
#include <stdexcept>

parser::parser(const std::vector<token>& tokens) : tokens(tokens), current_token_index(0) {}

token parser::current_token() const {
    if (current_token_index < tokens.size()) {
        return tokens[current_token_index];
    }
    return token{token_type::end_of_file, "", 0};
}

void parser::advance() {
    if (current_token_index < tokens.size()) {
        current_token_index++;
    }
}

bool parser::match(token_type type) {
    if (current_token().type == type) {
        advance();
        return true;
    }
    return false;
}

bool parser::is_type_keyword(token_type type) {
    return type == token_type::int_kw || type == token_type::float_kw ||
           type == token_type::bool_kw || type == token_type::string_kw ||
           type == token_type::custom_type_kw;
}

// Parse the entire program
std::shared_ptr<program_node> parser::parse_program() {
    auto program = std::make_shared<program_node>();

    while (current_token().type != token_type::end_of_file) {
        auto func = parse_function();
        if (func) {
            program->add_child(func);
        }
    }

    return program;
}

// Parse a function definition
std::shared_ptr<function_node> parser::parse_function() {
    if (!match(token_type::function_kw)) {
        error("Expected 'function' keyword.");
    }

    token identifier = current_token();
    if (!match(token_type::identifier)) {
        error("Expected function name.");
    }

    if (!match(token_type::lparen)) {
        error("Expected '(' after function name.");
    }

    auto parameters = parse_parameter_list();

    if (!match(token_type::rparen)) {
        error("Expected ')' after parameter list.");
    }

    if (!match(token_type::lbrace)) {
        error("Expected '{' at the beginning of function body.");
    }

    auto body = parse_scope();

    if (!match(token_type::rbrace)) {
        error("Expected '}' after function body.");
    }

    auto func_node = std::make_shared<function_node>(identifier.value, parameters, body);
    return func_node;
}

// Parse parameter list
std::vector<std::shared_ptr<parameter_node>> parser::parse_parameter_list() {
    std::vector<std::shared_ptr<parameter_node>> parameters;

    if (current_token().type != token_type::rparen) {
        do {
            auto param = parse_typed_identifier();
            if (param) {
                parameters.push_back(param);
            }
        } while (match(token_type::comma));
    }

    return parameters;
}

// Parse a typed identifier
std::shared_ptr<parameter_node> parser::parse_typed_identifier() {
    std::string type = parse_type();
    if (type.empty()) {
        error("Expected type in parameter.");
    }

    token id_token = current_token();
    if (!match(token_type::identifier)) {
        error("Expected identifier in parameter.");
    }

    return std::make_shared<parameter_node>(type, id_token.value);
}

// Parse a type
std::string parser::parse_type() {
    if (match(token_type::int_kw)) return "int";
    if (match(token_type::float_kw)) return "float";
    if (match(token_type::bool_kw)) return "bool";
    if (match(token_type::string_kw)) return "string";
    // Handle custom types
    if (match(token_type::custom_type_kw)) return current_token().value;
    return "";
}

// Parse a scope (block of statements)
std::shared_ptr<scope_node> parser::parse_scope() {
    auto scope = std::make_shared<scope_node>();

    while (current_token().type != token_type::rbrace && current_token().type != token_type::end_of_file) {
        auto stmt = parse_statement();
        if (stmt) {
            scope->add_statement(stmt);
        }
    }

    return scope;
}

// Parse different types of statements
std::shared_ptr<statement_node> parser::parse_statement() {
    if (match(token_type::return_kw)) {
        return parse_return_statement();
    } else if (is_type_keyword(current_token().type)) {
        return parse_declaration_statement();
    } else if (match(token_type::if_kw)) {
        return parse_if_statement();
    } else if (match(token_type::while_kw)) {
        return parse_while_statement();
    } else if (current_token().type == token_type::identifier) {
        return parse_assignment_statement();
    } else {
        error("Invalid statement.");
    }
    return nullptr; // Unreachable, but included to satisfy the compiler
}

// Parse a declaration statement
std::shared_ptr<statement_node> parser::parse_declaration_statement() {
    std::string type = parse_type();
    if (type.empty()) {
        error("Expected type in declaration.");
    }

    token id_token = current_token();
    if (!match(token_type::identifier)) {
        error("Expected identifier in declaration.");
    }

    if (!match(token_type::semicolon)) {
        error("Expected ';' at the end of declaration.");
    }

    return std::make_shared<declaration_node>(type, id_token.value);
}

// Parse an assignment statement
std::shared_ptr<statement_node> parser::parse_assignment_statement() {
    token id_token = current_token();
    if (!match(token_type::identifier)) {
        error("Expected identifier in assignment.");
    }

    if (!match(token_type::assign)) {
        error("Expected '=' in assignment.");
    }

    auto expr = parse_expression();

    if (!match(token_type::semicolon)) {
        error("Expected ';' at the end of assignment.");
    }

    return std::make_shared<assignment_node>(id_token.value, expr);
}

// Parse a return statement
std::shared_ptr<statement_node> parser::parse_return_statement() {
    auto expr = parse_expression();

    if (!match(token_type::semicolon)) {
        error("Expected ';' after return statement.");
    }

    return std::make_shared<return_node>(expr);
}

// Parse an if statement
std::shared_ptr<statement_node> parser::parse_if_statement() {
    if (!match(token_type::lparen)) {
        error("Expected '(' after 'if'.");
    }

    auto condition = parse_expression();

    if (!match(token_type::rparen)) {
        error("Expected ')' after condition.");
    }

    if (!match(token_type::lbrace)) {
        error("Expected '{' after 'if' condition.");
    }

    auto body = parse_scope();

    if (!match(token_type::rbrace)) {
        error("Expected '}' after 'if' body.");
    }

    return std::make_shared<if_node>(condition, body);
}

// Parse a while statement
std::shared_ptr<statement_node> parser::parse_while_statement() {
    if (!match(token_type::lparen)) {
        error("Expected '(' after 'while'.");
    }

    auto condition = parse_expression();

    if (!match(token_type::rparen)) {
        error("Expected ')' after condition.");
    }

    if (!match(token_type::lbrace)) {
        error("Expected '{' after 'while' condition.");
    }

    auto body = parse_scope();

    if (!match(token_type::rbrace)) {
        error("Expected '}' after 'while' body.");
    }

    return std::make_shared<while_node>(condition, body);
}

// Parse an expression
std::shared_ptr<expression_node> parser::parse_expression() {
    return parse_logical_or_expression();
}

// Parse logical OR expressions
std::shared_ptr<expression_node> parser::parse_logical_or_expression() {
    auto left = parse_logical_and_expression();

    while (match(token_type::or_op)) {
        std::string op = "||";
        auto right = parse_logical_and_expression();
        left = std::make_shared<binary_expression_node>(op, left, right);
    }

    return left;
}

// Parse logical AND expressions
std::shared_ptr<expression_node> parser::parse_logical_and_expression() {
    auto left = parse_equality_expression();

    while (match(token_type::and_op)) {
        std::string op = "&&";
        auto right = parse_equality_expression();
        left = std::make_shared<binary_expression_node>(op, left, right);
    }

    return left;
}

// Parse equality expressions
std::shared_ptr<expression_node> parser::parse_equality_expression() {
    auto left = parse_relational_expression();

    while (current_token().type == token_type::equal_equal ||
           current_token().type == token_type::not_equal) {
        std::string op = current_token().value;
        advance();
        auto right = parse_relational_expression();
        left = std::make_shared<binary_expression_node>(op, left, right);
    }

    return left;
}

// Parse relational expressions
std::shared_ptr<expression_node> parser::parse_relational_expression() {
    auto left = parse_additive_expression();

    while (current_token().type == token_type::less ||
           current_token().type == token_type::greater ||
           current_token().type == token_type::less_equal ||
           current_token().type == token_type::greater_equal) {
        std::string op = current_token().value;
        advance();
        auto right = parse_additive_expression();
        left = std::make_shared<binary_expression_node>(op, left, right);
    }

    return left;
}

// Parse additive expressions
std::shared_ptr<expression_node> parser::parse_additive_expression() {
    auto left = parse_multiplicative_expression();

    while (current_token().type == token_type::plus ||
           current_token().type == token_type::minus) {
        std::string op = current_token().value;
        advance();
        auto right = parse_multiplicative_expression();
        left = std::make_shared<binary_expression_node>(op, left, right);
    }

    return left;
}

// Parse multiplicative expressions
std::shared_ptr<expression_node> parser::parse_multiplicative_expression() {
    auto left = parse_unary_expression();

    while (current_token().type == token_type::star ||
           current_token().type == token_type::slash) {
        std::string op = current_token().value;
        advance();
        auto right = parse_unary_expression();
        left = std::make_shared<binary_expression_node>(op, left, right);
    }

    return left;
}

// Parse unary expressions
std::shared_ptr<expression_node> parser::parse_unary_expression() {
    if (current_token().type == token_type::minus ||
        current_token().type == token_type::not_op) {
        std::string op = current_token().value;
        advance();
        auto operand = parse_unary_expression();
        return std::make_shared<unary_expression_node>(op, operand);
    }
    return parse_primary_expression();
}

std::shared_ptr<expression_node> parser::parse_primary_expression() {
    if (match(token_type::number)) {
        return std::make_shared<number_node>(tokens[current_token_index - 1].value);
    } else if (match(token_type::identifier)) {
        std::string name = tokens[current_token_index - 1].value;

        if (match(token_type::lparen)) {
            // Parse function call arguments
            auto arguments = parse_argument_list();

            if (!match(token_type::rparen)) {
                error("Expected ')' after function call.");
            }

            // Create a function_call_node
            return std::make_shared<function_call_node>(name, arguments);
        }

        return std::make_shared<variable_node>(name);
    } else if (match(token_type::lparen)) {
        auto expr = parse_expression();
        if (!match(token_type::rparen)) {
            error("Expected ')' after expression.");
        }
        return expr;
    }

    error("Invalid primary expression.");
    return nullptr; // Unreachable, but included to satisfy the compiler
}



std::vector<std::shared_ptr<expression_node>> parser::parse_argument_list() {
    std::vector<std::shared_ptr<expression_node>> arguments;

    if (current_token().type != token_type::rparen) {
        do {
            auto arg = parse_expression();
            if (arg) {
                arguments.push_back(arg);
            } else {
                error("Invalid argument in function call.");
            }
        } while (match(token_type::comma));
    }

    return arguments;
}

// Error handling function
void parser::error(const std::string& message) {
    token tok = current_token();
    std::string error_message = "Parsing error at line " + std::to_string(tok.line) + ": " + message;
    throw parser_exception(error_message);
}

#pragma clang diagnostic pop
