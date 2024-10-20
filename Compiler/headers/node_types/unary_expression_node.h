#pragma once

#include "expression_node.h"
#include <string>
#include <memory>
#include <iostream>
#include <utility>

class unary_expression_node : public expression_node {
public:
    unary_expression_node(std::string op, std::shared_ptr<expression_node> operand)
            : op(std::move(op)), operand(std::move(operand)) {}

    void display() const override {
        std::cout << op;
        operand->display();
    }

private:
    std::string op;
    std::shared_ptr<expression_node> operand;
};
