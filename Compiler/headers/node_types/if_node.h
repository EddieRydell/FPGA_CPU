#pragma once

#include "statement_node.h"
#include "expression_node.h"
#include "scope_node.h"
#include <memory>
#include <iostream>
#include <utility>

class if_node : public statement_node {
public:
    if_node(std::shared_ptr<expression_node> condition, std::shared_ptr<scope_node> body)
            : condition(std::move(condition)), body(std::move(body)) {}

    void display() const override {
        std::cout << "if (";
        condition->display();
        std::cout << ") ";
        body->display();
    }

private:
    std::shared_ptr<expression_node> condition;
    std::shared_ptr<scope_node> body;
};
