#pragma once

#include <string>
#include <vector>
#include <unordered_map>

enum class token_type {
    function, if_kw, while_kw, return_kw, int_kw, float_kw, bool_kw, string_kw, identifier, number,
    plus, minus, star, slash, assign, comma, semicolon, lparen, rparen, lbrace, rbrace, end_of_file, unknown
};

struct token {
    token_type type;
    std::string value;
    int line_number;
};

class lexer {
public:
    explicit lexer(std::string  input);
    std::vector<token> tokenize();

private:
    std::string input;
    size_t position;
    int current_line;

    void advance();
    char current_char() const;
    bool has_more_chars() const;
    void skip_whitespace_and_newlines();

    token lex_identifier_or_keyword();
    token lex_number();
    token lex_symbol();
};

