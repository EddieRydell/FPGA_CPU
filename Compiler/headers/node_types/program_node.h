#pragma once

#include "ast_node.h"
#include "ast_node_types.h"

#include <vector>
#include <memory>
#include <iostream>

class program_node : public ast_node {
public:
    // Adds a child AST node to the program
    void add_child(const std::shared_ptr<ast_node>& child) {
        children.push_back(child);
    }

    // Displays the program node and its children
    void display() const override {
        std::cout << "Program" << std::endl;
        for (const auto& child : children) {
            child->display();
        }
    }

    // Getter for the children
    const std::vector<std::shared_ptr<ast_node>>& get_children() const {
        return children;
    }

private:
    std::vector<std::shared_ptr<ast_node>> children;
};

