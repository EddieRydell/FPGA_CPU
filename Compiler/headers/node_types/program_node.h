#pragma once

#include "ast_node.h"
#include <vector>
#include <memory>
#include <iostream>

class program_node : public ast_node {
public:
    void add_child(std::shared_ptr<ast_node> child) {
        children.push_back(child);
    }

    void display() const override {
        std::cout << "Program" << std::endl;
        for (const auto& child : children) {
            child->display();
        }
    }

private:
    std::vector<std::shared_ptr<ast_node>> children;
};

