#pragma once
#include "statement_node.h"
#include "expression_node.h"
#include <string>
#include <iostream>
#include <utility>

class assignment_node : public statement_node {
public:
    assignment_node(std::string  variable, std::shared_ptr<expression_node> expression)
            : variable(std::move(variable)), expression(std::move(expression)) {}

    std::string variable;
    std::shared_ptr<expression_node> expression;

    void display() const override {
        std::cout << "Assignment: " << variable << " = ";
        expression->display();
        std::cout << std::endl;
    }
};
