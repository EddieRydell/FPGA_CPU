#pragma once

#include "ast_node.h"
#include <string>
#include <iostream>
#include <utility>

class parameter_node : public ast_node {
public:
    parameter_node(std::string  type, std::string  name)
            : type(std::move(type)), name(std::move(name)) {}

    void display() const override {
        std::cout << type << " " << name;
    }

private:
    std::string type;
    std::string name;
};
