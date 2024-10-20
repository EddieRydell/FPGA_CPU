#pragma once

#include "expression_node.h"
#include <string>
#include <iostream>
#include <utility>

class number_node : public expression_node {
public:
    explicit number_node(std::string value) : value(std::move(value)) {}

    void display() const override {
        std::cout << value;
    }

private:
    std::string value;
};

