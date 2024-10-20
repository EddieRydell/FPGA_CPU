#pragma once
#include "ast_node.h"

class expression_node : public ast_node {
public:
    void display() const override = 0;
};
