#pragma once

class ast_node {
public:
    virtual ~ast_node() = default;
    virtual void display() const = 0;
};
