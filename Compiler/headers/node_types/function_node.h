#pragma once
#include "ast_node.h"
#include "statement_node.h"
#include <string>
#include <iostream>
#include <utility>

class function_node : public ast_node {
public:
    explicit function_node(std::string  name) : name(std::move(name)) {}

    std::string name;

    void add_statement(std::shared_ptr<statement_node> statement) {
        add_child(statement);  // Use the base class's add_child() method
    }

    void display() const override {
        std::cout << "Function: " << name << std::endl;
        for (const auto& child : children) {
            child->display();
        }
    }
};
