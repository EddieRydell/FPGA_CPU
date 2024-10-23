#pragma once

#include "ast_node.h"
#include "parameter_node.h"
#include "statement_node.h"

#include <string>
#include <utility>
#include <vector>
#include <memory>
#include <iostream>

class function_node : public ast_node {
public:
    // Constructor
    function_node(std::string  name,
                  const std::vector<std::pair<std::string, std::string>>& parameters, // Pair of type and identifier
                  std::string  return_type,
                  const std::vector<std::shared_ptr<statement_node>>& body)
            : name_(std::move(name)), parameters_(parameters), return_type_(std::move(return_type)), body_(body) {}

    // Displays the function node
    void display() const override {
        std::cout << "Function: " << name_ << " :: (";
        for (size_t i = 0; i < parameters_.size(); ++i) {
            std::cout << parameters_[i].first << " " << parameters_[i].second;
            if (i < parameters_.size() - 1) std::cout << ", ";
        }
        std::cout << ") -> " << return_type_ << std::endl;
        std::cout << "Body:" << std::endl;
        for (const auto& stmt : body_) {
            stmt->display();
        }
    }

    // Getter for function name
    const std::string& get_name() const {
        return name_;
    }

    // Getter for parameters (type and identifier)
    const std::vector<std::pair<std::string, std::string>>& get_parameters() const {
        return parameters_;
    }

    // Getter for return type
    const std::string& get_return_type() const {
        return return_type_;
    }

    // Getter for function body
    const std::vector<std::shared_ptr<statement_node>>& get_body() const {
        return body_;
    }

private:
    std::string name_;
    std::vector<std::pair<std::string, std::string>> parameters_; // Each pair contains type and identifier
    std::string return_type_;
    std::vector<std::shared_ptr<statement_node>> body_;
};

