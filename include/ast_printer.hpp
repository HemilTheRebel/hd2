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
        if (expr->value.empty())
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
            pp += std::any_cast<std::string>(expr->accept(this));
        }
        return pp + ")";
    }
};


// class ASTPrinter : ExprVisitor {
//     void parenthesize(const std::string& name, std::vector<Expr*> exprs) {
//         std::string prettyPrint = "(" + name;

//         std::cout << prettyPrint;
//         for (auto expr : exprs) {
//             std::cout << " ";
//             expr->accept(this);
//         }

//         std::cout << ")";
//     }

//     public:

//     void print(Expr *expr) {
//         return expr->accept(this);
//     }

//     void visitBinaryExpr(BinaryExpr *expr) override {
//         return parenthesize(expr->Operator.lexeme, {expr->left, expr->right});
//     }

//     void visitGroupingExpr(GroupingExpr *expr) override {
//         return parenthesize("group", {expr->expression});
//     }

//     void visitLiteralExpr(LiteralExpr *expr) override {
//         if (expr->value.empty()) {
//             std::cout << "nil";
//         }

//         std::cout << expr->value;
//     }

//     void visitUnaryExpr(UnaryExpr *expr) override {
//         return parenthesize(expr->Operator.lexeme, {expr->right});
//     }


// };