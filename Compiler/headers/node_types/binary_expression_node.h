#pragma once
#include "expression_node.h"
#include <string>
#include <iostream>
#include <utility>

class binary_expression_node : public expression_node {
public:
    binary_expression_node(std::string  op, std::shared_ptr<expression_node> left, std::shared_ptr<expression_node> right)
            : op(std::move(op)), left(std::move(left)), right(std::move(right)) {}

    std::string op;
    std::shared_ptr<expression_node> left;
    std::shared_ptr<expression_node> right;

    void display() const override {
        left->display();
        std::cout << " " << op << " ";
        right->display();
    }
};
