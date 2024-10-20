#pragma once

#include "statement_node.h"
#include "expression_node.h"
#include <memory>
#include <iostream>
#include <utility>

class return_node : public statement_node {
public:
    explicit return_node(std::shared_ptr<expression_node> expr)
            : expression(std::move(expr)) {}

    void display() const override {
        std::cout << "return ";
        expression->display();
        std::cout << ";" << std::endl;
    }

private:
    std::shared_ptr<expression_node> expression;
};
