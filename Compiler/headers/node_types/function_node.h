#pragma once

#include "ast_node.h"
#include "parameter_node.h"
#include "scope_node.h"
#include <string>
#include <vector>
#include <memory>
#include <iostream>

class function_node : public ast_node {
public:
    function_node(const std::string& name,
                  const std::vector<std::shared_ptr<parameter_node>>& params,
                  std::shared_ptr<scope_node> body)
            : name(name), parameters(params), body(body) {}

    void display() const override {
        std::cout << "Function: " << name << "(";
        for (size_t i = 0; i < parameters.size(); ++i) {
            parameters[i]->display();
            if (i < parameters.size() - 1)
                std::cout << ", ";
        }
        std::cout << ")" << std::endl;
        body->display();
    }

private:
    std::string name;
    std::vector<std::shared_ptr<parameter_node>> parameters;
    std::shared_ptr<scope_node> body;
};

