#include "tokenizer.h"
#include <iostream>

void print_tokens(const std::vector<token>& tokens) {
    std::unordered_map<token_type, std::string> token_type_to_string = {
            {token_type::function, "FUNCTION"}, {token_type::if_kw, "IF"}, {token_type::while_kw, "WHILE"},
            {token_type::return_kw, "RETURN"}, {token_type::int_kw, "INT"}, {token_type::float_kw, "FLOAT"},
            {token_type::bool_kw, "BOOL"}, {token_type::string_kw, "STRING"}, {token_type::identifier, "IDENTIFIER"},
            {token_type::number, "NUMBER"}, {token_type::plus, "PLUS"}, {token_type::minus, "MINUS"},
            {token_type::star, "STAR"}, {token_type::slash, "SLASH"}, {token_type::assign, "ASSIGN"},
            {token_type::comma, "COMMA"}, {token_type::semicolon, "SEMICOLON"}, {token_type::lparen, "LPAREN"},
            {token_type::rparen, "RPAREN"}, {token_type::lbrace, "LBRACE"}, {token_type::rbrace, "RBRACE"},
            {token_type::end_of_file, "END_OF_FILE"}, {token_type::unknown, "UNKNOWN"}
    };

    for (const auto& token : tokens) {
        std::cout << "Line " << token.line_number << ": <" << token_type_to_string[token.type] << ", " << token.value << ">\n";
    }
}

int main() {
    std::string program = R"(
        function add(int a, int b) {
            int sum;
            sum = a + b;
            return sum;
        }
    )";

    tokenizer lx(program);
    std::vector<token> tokens = lx.tokenize();
    print_tokens(tokens);

    return 0;
}
