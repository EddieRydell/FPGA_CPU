#pragma once

#include <string>
#include <vector>

enum class token_type {
    // Keywords
    function_kw,
    return_kw,
    if_kw,
    else_kw,
    while_kw,
    int_kw,
    float_kw,
    bool_kw,
    string_kw,
    custom_type_kw, // For custom types

    // Operators and symbols
    plus,           // '+'
    minus,          // '-'
    star,           // '*'
    slash,          // '/'
    assign,         // '='
    equal_equal,    // '=='
    not_equal,      // '!='
    less,           // '<'
    greater,        // '>'
    less_equal,     // '<='
    greater_equal,  // '>='
    and_op,         // '&&'
    or_op,          // '||'
    not_op,         // '!'
    comma,          // ','
    semicolon,      // ';'
    lparen,         // '('
    rparen,         // ')'
    lbrace,         // '{'
    rbrace,         // '}'

    // Literals
    identifier,
    number,

    // Special
    unknown,
    end_of_file
};

struct token {
    token_type type;
    std::string value;
    size_t line;
};

class tokenizer {
public:
    explicit tokenizer(std::string input);

    std::vector<token> tokenize();

private:
    std::string input;
    size_t position;
    size_t current_line;

    void skip_whitespace_and_newlines();
    void advance();
    char current_char() const;
    bool has_more_chars() const;

    token tokenize_identifier_or_keyword();
    token tokenize_number();
    token tokenize_symbol_or_operator();
};


