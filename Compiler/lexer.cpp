#include "lexer.h"
#include <cctype>
#include <iostream>
#include <utility>

lexer::lexer(std::string  input) : input(std::move(input)), position(0), current_line(1) {}

std::vector<token> lexer::tokenize() {
    std::vector<token> tokens;
    while (position < input.size()) {
        skip_whitespace_and_newlines();
        if (position >= input.size()) break;  // End of input

        char current = input[position];
        if (isalpha(current) || current == '_') {
            tokens.push_back(lex_identifier_or_keyword());
        } else if (isdigit(current)) {
            tokens.push_back(lex_number());
        } else {
            tokens.push_back(lex_symbol());
        }
    }
    tokens.push_back(token{token_type::end_of_file, "", current_line});
    return tokens;
}

void lexer::skip_whitespace_and_newlines() {
    while (position < input.size() && (isspace(input[position]) || input[position] == '\n')) {
        if (input[position] == '\n') current_line++;  // Increment line_number number on newline
        advance();
    }
}

void lexer::advance() { position++; }

char lexer::current_char() const { return input[position]; }

bool lexer::has_more_chars() const { return position < input.size(); }

token lexer::lex_identifier_or_keyword() {
    size_t start = position;
    while (has_more_chars() && (isalnum(current_char()) || current_char() == '_')) {
        advance();
    }

    std::string lexeme = input.substr(start, position - start);

    // Check for keywords
    if (lexeme == "function") return token{token_type::function, lexeme, current_line};
    if (lexeme == "if") return token{token_type::if_kw, lexeme, current_line};
    if (lexeme == "while") return token{token_type::while_kw, lexeme, current_line};
    if (lexeme == "return") return token{token_type::return_kw, lexeme, current_line};
    if (lexeme == "int") return token{token_type::int_kw, lexeme, current_line};
    if (lexeme == "float") return token{token_type::float_kw, lexeme, current_line};
    if (lexeme == "bool") return token{token_type::bool_kw, lexeme, current_line};
    if (lexeme == "string") return token{token_type::string_kw, lexeme, current_line};

    // Otherwise, it's an identifier
    return token{token_type::identifier, lexeme, current_line};
}

token lexer::lex_number() {
    size_t start = position;
    while (has_more_chars() && isdigit(current_char())) {
        advance();
    }
    return token{token_type::number, input.substr(start, position - start), current_line};
}

token lexer::lex_symbol() {
    char current = current_char();
    advance();

    switch (current) {
        case '+': return token{token_type::plus, "+", current_line};
        case '-': return token{token_type::minus, "-", current_line};
        case '*': return token{token_type::star, "*", current_line};
        case '/': return token{token_type::slash, "/", current_line};
        case '=': return token{token_type::assign, "=", current_line};
        case ',': return token{token_type::comma, ",", current_line};
        case ';': return token{token_type::semicolon, ";", current_line};
        case '(': return token{token_type::lparen, "(", current_line};
        case ')': return token{token_type::rparen, ")", current_line};
        case '{': return token{token_type::lbrace, "{", current_line};
        case '}': return token{token_type::rbrace, "}", current_line};
        default: return token{token_type::unknown, std::string(1, current), current_line};
    }
}
