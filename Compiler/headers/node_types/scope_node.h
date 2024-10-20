#pragma once

#include "statement_node.h"
#include <vector>
#include <memory>
#include <iostream>

class scope_node : public statement_node {
public:
    void add_statement(const std::shared_ptr<statement_node>& stmt) {
        statements.push_back(stmt);
    }

    void display() const override {
        std::cout << "{" << std::endl;
        for (const auto& stmt : statements) {
            stmt->display();
        }
        std::cout << "}" << std::endl;
    }

private:
    std::vector<std::shared_ptr<statement_node>> statements;
};
