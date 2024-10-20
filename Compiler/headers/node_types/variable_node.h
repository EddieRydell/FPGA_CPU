#pragma once

#include "expression_node.h"
#include <string>
#include <iostream>
#include <utility>

class variable_node : public expression_node {
public:
    explicit variable_node(std::string  name) : name(std::move(name)) {}

    void display() const override {
        std::cout << name;
    }

private:
    std::string name;
};

