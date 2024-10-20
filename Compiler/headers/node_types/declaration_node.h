#pragma once

#include "statement_node.h"
#include <string>
#include <iostream>
#include <utility>

class declaration_node : public statement_node {
public:
    declaration_node(std::string  type, std::string  name)
            : type(std::move(type)), name(std::move(name)) {}

    void display() const override {
        std::cout << type << " " << name << ";" << std::endl;
    }

private:
    std::string type;
    std::string name;
};
