#pragma once

#include <iostream> 
#include <vector>
#include "expression.hpp"

class ASTPrinter : public ExprVisitor {
      public:
        void print(Expr* expr) {
            return expr->accept(this);
        }
        void visitBinaryExpr(BinaryExpr* expr) override {
            return parenthesize(expr->Operator.lexeme,
                                {expr->left, expr->right});
        }
        void visitGroupingExpr(GroupingExpr* expr) override {
            return parenthesize("group", {expr->expression});
        }
        void visitLiteralExpr(LiteralExpr* expr) override {
            if (expr->value.empty())
                std::cout << "nil";
            std::cout << " " << expr->value;
        }
        void visitUnaryExpr(UnaryExpr* expr) override {
            return parenthesize(expr->Operator.lexeme, {expr->right});
        }
        void parenthesize(std::string name, std::vector<Expr*> exprs) {
            std::string pp = "(" + name;
            // print
            std::cout << pp;
            for (auto expr : exprs) {
                expr->accept(this);
            }
            std::cout << ")";
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