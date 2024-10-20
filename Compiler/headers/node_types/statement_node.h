#pragma once

#include "ast_node.h"

class statement_node : public ast_node {
public:
    virtual void display() const override = 0;
};
