#pragma once

#include <vector>

#include "tokens.hpp"
#include "expression.hpp"
#include "errors.hpp"
#include "statement.hpp"

class ParseError : public std::runtime_error {
    public:
    ParseError() : runtime_error("") {}
};

class Parser {
    std::vector<Token> tokens;
    int current = 0;

    Expr* expression() {
        return assignment();
    }

    Expr* assignment() {
        Expr* expr = equality();

        if (match({TokenType::EQUAL})) {
            Token equals = previous();
            Expr *value = assignment();

            if (auto var = dynamic_cast<VariableExpr*>(expr)) {
                Token name = var->name;
                return new AssignExpr(name, value);
            }

            error(equals, "Invalid assignment target");
        }

        return expr;
    }

    Expr* equality() {
        Expr* expr = comparison();

        while (match({TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL})) {
            Token Operator = previous();
            Expr* right = comparison();
            expr = new BinaryExpr(expr, Operator, right);
        }

        return expr;
    }

    Expr* comparison() {
        Expr* expr = addition();

        while (match({TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL})) {
            Token Operator = previous();
            Expr* right = addition();
            expr = new BinaryExpr(expr, Operator, right);
        }

        return expr;
    }

    Expr* addition() {
        Expr* expr = multiplication();

        while (match({TokenType::MINUS, TokenType::PLUS})) {
            Token Operator = previous();
            Expr* right = multiplication();
            expr = new BinaryExpr(expr, Operator, right);
        }

        return expr;
    }

    Expr* multiplication() {
        Expr* expr = unary();

        while (match({TokenType::SLASH, TokenType::STAR})) {
            Token Operator = previous();
            Expr* right = unary();
            expr = new BinaryExpr(expr, Operator, right);
        }

        return expr;
    }

    Expr* unary() {
        if (match({TokenType::BANG, TokenType::MINUS})) {
            Token Operator = previous();
            Expr* right = unary();
            return new UnaryExpr(Operator, right);
        }

        return primary();
    }

    Expr* primary() {
        if (match({TokenType::FALSE})) return new LiteralExpr(false);
        if (match({TokenType::TRUE})) return new LiteralExpr(true);
        if (match({TokenType::NIL})) return new LiteralExpr(nullptr);
        
        if (match({TokenType::NUMBER})) {
            return new LiteralExpr(std::stod(previous().literal));
        }

        if (match({TokenType::STRING})) {
            return new LiteralExpr(previous().literal);
        }

        if (match({TokenType::LEFT_PAREN})) {
            Expr* expr = expression();
            consume(TokenType::RIGHT_PAREN, "Expect ')' after expression");
            return new GroupingExpr(expr);
        }

        if (match({TokenType::IDENTIFIER})) {
            return new VariableExpr(previous());
        }

        throw error(peek(), "Expect expression");
    }

    bool match(const std::vector<TokenType>& types) {
        for (auto type : types) {
            if (check(type)) {
                advance();
                return true;
            }
        }

        return false;
    }

    Token consume(TokenType type, std::string message) {
        if (check(type)) return advance();

        throw error(peek(), message);
    }

    ParseError error(Token token, std::string message) {
        Errors::error(token, message);

        return ParseError();
    }

    void synchronize() {
        advance();

        while (!isAtEnd()) {
            if (previous().type == TokenType::SEMICOLON) return;

            // other types of enumeration purposely not handled

            // ignoring -Wswitch. Works for both GCC and cland

            #pragma GCC diagnostic push
            #pragma GCC diagnostic ignored "-Wswitch"
            switch (peek().type) {
                case TokenType::CLASS:                            
                case TokenType::FUN:                              
                case TokenType::VAR:                              
                case TokenType::FOR:                              
                case TokenType::IF:                               
                case TokenType::WHILE:                            
                case TokenType::PRINT:                            
                case TokenType::RETURN:                           
                return;
            }
            #pragma GCC diagnostic pop

            advance();
        }
    }

    bool check(TokenType type) {
        if (isAtEnd()) 
            return false;
        return peek().type == type;
    }

    Token advance() {
        if (!isAtEnd())
            current++;
        
        return previous();
    }

    bool isAtEnd() {
        return peek().type == TokenType::EndOfFile;
    }

    Token peek() {
        return tokens.at(current);
    }

    Token previous() {
        return tokens.at(current - 1);
    }

    Stmt* statement() {
        if (match({TokenType::PRINT})) return printStatement();

        return expressionStatement();
    }

    Stmt* printStatement() {
        Expr *value = expression();
        consume(TokenType::SEMICOLON, "Expect ; after value");
        return new PrintStmt(value);
    }

    Stmt* expressionStatement() {
        Expr* expr = expression();
        consume(TokenType::SEMICOLON, "Expect ; after value");
        return new ExpressionStmt(expr);
    }

    Stmt* declaration() {
        try {
            if (match({TokenType::VAR})) return varDeclaration();

            return statement();
        } catch (ParseError &error) {
            synchronize();
            return nullptr;
        }
    }

    Stmt* varDeclaration() {
        Token name = consume(TokenType::IDENTIFIER, "Expect variable name");

        Expr* initalizer = match({TokenType::EQUAL}) ? expression() : nullptr;

        consume(TokenType::SEMICOLON, "Expect ; after variable declaration");
        return new VarStmt(name, initalizer);
    }

public:
    Parser(const std::vector<Token>& tokens) : tokens(tokens) {}

    std::vector<Stmt*> parse() {
        std::vector<Stmt*> statements; 

        while(!isAtEnd()) {
            statements.push_back(declaration());
        }

        return statements;
    }
};