#pragma once

#include <iostream>

#include "expression.hpp"
#include "errors.hpp"
#include "runtime_error.hpp"

class Interpreter : public ExprVisitor {

    std::any evaluate(Expr *expr) {
        return expr->accept(this);
    }

    bool isTrue(std::any expr) {
        if (!expr.has_value()) return false;

        if (expr.type() == typeid(bool)) return std::any_cast<bool>(expr);
        
        return false;
    }

    bool isEqual(std::any left, std::any right) {
        if (!left.has_value() && !right.has_value()) return true;

        if (!left.has_value()) return false;

        if (left.type() != right.type()) return false;

        if (left.type() == typeid(double)) return std::any_cast<double>(left) == std::any_cast<double>(right);

        if (left.type() == typeid(bool)) return std::any_cast<bool>(left) == std::any_cast<bool>(right);

        if (left.type() == typeid(std::string)) return std::any_cast<std::string>(left) == std::any_cast<std::string>(right);
        
        return false;
    }

    void checkNumberOperand(Token Operator, std::any operand) {
        if (operand.type() == typeid(double)) return;

        throw RuntimeError (Operator, "Operand must be a number");
    }

    void checkNumberOperands(Token Operator, std::any left, std::any right) {
        if (left.type() == typeid(double) && right.type() == typeid(double)) return;

        throw RuntimeError(Operator, "Operands must be a numbers");
    }

    std::string stringify(std::any object) {
        if (!object.has_value()) return "nil";
 
        if (object.type() == typeid(double)) return std::to_string(std::any_cast<double>(object));

        if (object.type() == typeid(bool)) return std::any_cast<bool>(object) ? "true" : "false";

        return std::any_cast<std::string>(object);
    }
public:
    std::any visitLiteralExpr(LiteralExpr *expr) override {
        return expr->value;
    }

    std::any visitGroupingExpr(GroupingExpr *expr) override {
        return evaluate(expr->expression);
    }

    std::any visitUnaryExpr(UnaryExpr *expr) override {
        std::any right = evaluate(expr->right);

        switch (expr->Operator.type) {
        case TokenType::BANG:
            return !isTrue(right);

        case TokenType::MINUS:
            checkNumberOperand(expr->Operator, right);
            return - std::any_cast<double>(right);
        
        default:
            break;
        }

        return nullptr;
    }

    std::any visitBinaryExpr(BinaryExpr* expr) override {
        std::any left = evaluate(expr->left),
                 right = evaluate(expr->right);

        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wswitch"

        switch (expr->Operator.type) {
            // comparison operators

            case TokenType::GREATER:
                checkNumberOperands(expr->Operator, left, right);
                return std::any_cast<double>(left) > std::any_cast<double>(right);

            case TokenType::GREATER_EQUAL:
                checkNumberOperands(expr->Operator, left, right);
                return std::any_cast<double>(left) >= std::any_cast<double>(right);

            case TokenType::LESS:
                checkNumberOperands(expr->Operator, left, right);
                return std::any_cast<double>(left) < std::any_cast<double>(right);

            case TokenType::LESS_EQUAL:
                checkNumberOperands(expr->Operator, left, right);
                return std::any_cast<double>(left) <= std::any_cast<double>(right);

            case TokenType::BANG_EQUAL: return !isEqual(left, right);

            case TokenType::EQUAL_EQUAL: return isEqual(left, right);

            // arithmetic operators

            case TokenType::MINUS:
                checkNumberOperands(expr->Operator, left, right);
                return std::any_cast<double>(left) - std::any_cast<double>(right);
            
            case TokenType::SLASH:
                checkNumberOperands(expr->Operator, left, right);
                return std::any_cast<double>(left) / std::any_cast<double>(right);

            case TokenType::STAR:
                checkNumberOperands(expr->Operator, left, right);
                return std::any_cast<double>(left) * std::any_cast<double>(right);

            case TokenType::PLUS:
                if (left.type() == typeid(double) && right.type() == typeid(double) ) {
                    return std::any_cast<double>(left) + std::any_cast<double>(right);
                }
                if (left.type() == typeid(std::string) && right.type() == typeid(std::string)) {
                    return std::any_cast<std::string>(left) + std::any_cast<std::string>(right);
                }

                throw RuntimeError(expr->Operator, "Operands must be two strings or numbers");
        }
        #pragma GCC diagnostic pop

        return nullptr;
    }

    void interpret(Expr *expression) {
        try {
            std::any value = evaluate(expression);
            std::cout << stringify(value);
        } catch (RuntimeError &e) {
            Errors::runtimeError(e);
        }
    }
};
