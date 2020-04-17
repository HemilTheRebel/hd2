#pragma once

#include <iostream> 
#include <vector>
#include "expression.hpp"

class ASTPrinter : public ExprVisitor {
public:
    std::any print(Expr* expr) {
        return expr->accept(this);
    }

    std::any visitBinaryExpr(BinaryExpr* expr) override {
        return parenthesize(expr->Operator.lexeme,
                            {expr->left, expr->right});
    }

    std::any visitGroupingExpr(GroupingExpr* expr) override {
        return parenthesize("group", {expr->expression});
    }

    std::any visitLiteralExpr(LiteralExpr* expr) override {
        if (!expr->value.has_value())
            return "nil";
        return expr->value;
    }

    std::any visitUnaryExpr(UnaryExpr* expr) override {
        return parenthesize(expr->Operator.lexeme, {expr->right});
    }

    std::any parenthesize(std::string name, std::vector<Expr*> exprs) {
        std::string pp = "(" + name;
        
        for (auto expr : exprs) {
            pp += " ";

            auto result = expr->accept(this);

            if (result.type() == typeid(bool))
                pp += std::any_cast<bool>(result) ? "true" : "false";
            else if (result.type() == typeid(double))
                pp += std::to_string(std::any_cast<double>(result));
            else
                pp += std::any_cast<std::string>(result);
        }
        pp += ")";

        return pp;
    }
};
