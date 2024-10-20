#include "tokenizer.h"
#include <cctype>
#include <utility>

tokenizer::tokenizer(std::string input) : input(std::move(input)), position(0), current_line(1) {}

std::vector<token> tokenizer::tokenize() {
    std::vector<token> tokens;
    while (position < input.size()) {
        skip_whitespace_and_newlines();
        if (position >= input.size()) break;  // End of input

        char current = input[position];
        if (isalpha(current) || current == '_') {
            tokens.push_back(tokenize_identifier_or_keyword());
        }
        else if (isdigit(current)) {
            tokens.push_back(tokenize_number());
        }
        else {
            tokens.push_back(tokenize_symbol_or_operator());
        }
    }
    tokens.push_back(token{token_type::end_of_file, "", current_line});
    return tokens;
}

void tokenizer::skip_whitespace_and_newlines() {
    while (position < input.size() && isspace(input[position])) {
        if (input[position] == '\n') current_line++;  // Increment line number on newline
        advance();
    }
}

void tokenizer::advance() { position++; }

char tokenizer::current_char() const { return input[position]; }

bool tokenizer::has_more_chars() const { return position < input.size(); }

token tokenizer::tokenize_identifier_or_keyword() {
    size_t start = position;
    while (has_more_chars() && (isalnum(current_char()) || current_char() == '_')) {
        advance();
    }

    std::string lexeme = input.substr(start, position - start);

    // Check for keywords
    if (lexeme == "function") return token{token_type::function_kw, lexeme, current_line};
    if (lexeme == "if") return token{token_type::if_kw, lexeme, current_line};
    if (lexeme == "else") return token{token_type::else_kw, lexeme, current_line};
    if (lexeme == "while") return token{token_type::while_kw, lexeme, current_line};
    if (lexeme == "return") return token{token_type::return_kw, lexeme, current_line};
    if (lexeme == "int") return token{token_type::int_kw, lexeme, current_line};
    if (lexeme == "float") return token{token_type::float_kw, lexeme, current_line};
    if (lexeme == "bool") return token{token_type::bool_kw, lexeme, current_line};
    if (lexeme == "string") return token{token_type::string_kw, lexeme, current_line};
    // Add any other keywords or types as needed

    // Otherwise, it's an identifier
    return token{token_type::identifier, lexeme, current_line};
}

token tokenizer::tokenize_number() {
    size_t start = position;
    while (has_more_chars() && isdigit(current_char())) {
        advance();
    }
    // Optionally handle decimal numbers (floats)
    if (has_more_chars() && current_char() == '.') {
        advance();
        while (has_more_chars() && isdigit(current_char())) {
            advance();
        }
    }
    return token{token_type::number, input.substr(start, position - start), current_line};
}

token tokenizer::tokenize_symbol_or_operator() {
    char current = current_char();

    // Handle multi-character operators
    if (current == '&' || current == '|' || current == '=' || current == '!' ||
        current == '<' || current == '>') {

        char next = '\0';
        if (position + 1 < input.size()) {
            next = input[position + 1];
        }

        // Check for '&&' and '||'
        if (current == '&' && next == '&') {
            advance(); // Consume '&'
            advance(); // Consume second '&'
            return token{token_type::and_op, "&&", current_line};
        }
        else if (current == '|' && next == '|') {
            advance(); // Consume '|'
            advance(); // Consume second '|'
            return token{token_type::or_op, "||", current_line};
        }
            // Check for '==' and '!='
        else if (current == '=' && next == '=') {
            advance(); // Consume '='
            advance(); // Consume second '='
            return token{token_type::equal_equal, "==", current_line};
        }
        else if (current == '!' && next == '=') {
            advance(); // Consume '!'
            advance(); // Consume '='
            return token{token_type::not_equal, "!=", current_line};
        }
            // Check for '<=' and '>='
        else if (current == '<' && next == '=') {
            advance(); // Consume '<'
            advance(); // Consume '='
            return token{token_type::less_equal, "<=", current_line};
        }
        else if (current == '>' && next == '=') {
            advance(); // Consume '>'
            advance(); // Consume '='
            return token{token_type::greater_equal, ">=", current_line};
        }
            // Single-character operators: '<', '>', '=', '!'
        else {
            advance(); // Consume current character
            switch (current) {
                case '=': return token{token_type::assign, "=", current_line};
                case '!': return token{token_type::not_op, "!", current_line};
                case '<': return token{token_type::less, "<", current_line};
                case '>': return token{token_type::greater, ">", current_line};
                default: break; // Should not reach here
            }
        }
    }
    else {
        advance(); // Consume current character
        switch (current) {
            case '+': return token{token_type::plus, "+", current_line};
            case '-': return token{token_type::minus, "-", current_line};
            case '*': return token{token_type::star, "*", current_line};
            case '/': return token{token_type::slash, "/", current_line};
            case '(': return token{token_type::lparen, "(", current_line};
            case ')': return token{token_type::rparen, ")", current_line};
            case '{': return token{token_type::lbrace, "{", current_line};
            case '}': return token{token_type::rbrace, "}", current_line};
            case ',': return token{token_type::comma, ",", current_line};
            case ';': return token{token_type::semicolon, ";", current_line};
            default:
                // Handle unknown symbols
                return token{token_type::unknown, std::string(1, current), current_line};
        }
    }

    // If none of the above matched, return an unknown token
    return token{token_type::unknown, std::string(1, current), current_line};
}
