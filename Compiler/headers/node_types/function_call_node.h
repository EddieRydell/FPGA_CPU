#pragma once

#include "expression_node.h"
#include <string>
#include <utility>
#include <vector>
#include <memory>
#include <iostream>

class function_call_node : public expression_node {
public:
    function_call_node(std::string  function_name,
                       const std::vector<std::shared_ptr<expression_node>>& args)
            : function_name(std::move(function_name)), arguments(args) {}

    void display() const override {
        std::cout << function_name << "(";
        for (size_t i = 0; i < arguments.size(); ++i) {
            arguments[i]->display();
            if (i < arguments.size() - 1)
                std::cout << ", ";
        }
        std::cout << ")";
    }

private:
    std::string function_name;
    std::vector<std::shared_ptr<expression_node>> arguments;
};

