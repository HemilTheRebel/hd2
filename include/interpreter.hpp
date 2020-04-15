#pragma once

#include "expression.hpp"

class Interpreter : public ExprVisitor
{

public:
    Interpreter(/* args */);
    ~Interpreter();
};
