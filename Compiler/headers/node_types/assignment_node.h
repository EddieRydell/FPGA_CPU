#pragma once

#include "statement_node.h"
#include "expression_node.h"
#include <string>
#include <memory>
#include <iostream>
#include <utility>

class assignment_node : public statement_node {
public:
    assignment_node(std::string  variable_name, std::shared_ptr<expression_node> expr)
            : variable_name(std::move(variable_name)), expression(std::move(expr)) {}

    void display() const override {
        std::cout << variable_name << " = ";
        expression->display();
        std::cout << ";" << std::endl;
    }

private:
    std::string variable_name;
    std::shared_ptr<expression_node> expression;
};

