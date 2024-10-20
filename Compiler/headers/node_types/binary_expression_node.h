#pragma once

#include "expression_node.h"
#include <string>
#include <memory>
#include <iostream>
#include <utility>

class binary_expression_node : public expression_node {
public:
    binary_expression_node(std::string op,
                           std::shared_ptr<expression_node> left,
                           std::shared_ptr<expression_node> right)
            : op(std::move(op)), left(std::move(left)), right(std::move(right)) {}

    void display() const override {
        std::cout << "(";
        left->display();
        std::cout << " " << op << " ";
        right->display();
        std::cout << ")";
    }

private:
    std::string op;
    std::shared_ptr<expression_node> left;
    std::shared_ptr<expression_node> right;
};
