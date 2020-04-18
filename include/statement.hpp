#pragma once

#include "tokens.hpp"
#include "expression.hpp"

class ExpressionStmt;
class PrintStmt;
class VarStmt;

class StmtVisitor {
public:
    virtual ~StmtVisitor() {}
    virtual void visitExpressionStmt (ExpressionStmt * Stmt) = 0;
    virtual void visitPrintStmt      (PrintStmt    * Stmt) = 0;
    virtual void visitVarStmt        (VarStmt    * stmt) = 0;
};

class Stmt {
public:
    virtual ~Stmt() {}
    virtual void accept(StmtVisitor* visitor) = 0;
};

class ExpressionStmt  : public Stmt { 
public: 
    ExpressionStmt (Expr *expression)  : expression(expression) {}
    
    void accept(StmtVisitor* visitor) override {
        visitor->visitExpressionStmt(this);
    }
public: 
    Expr *expression;
};

class PrintStmt: public Stmt { 
public: 
    PrintStmt(Expr *expression)  : expression(expression) {}
    
    void accept(StmtVisitor* visitor) override {
        visitor->visitPrintStmt(this);
    }

public: 
    Expr *expression;
};

class VarStmt : public Stmt {
public:
    VarStmt(Token name, Expr *initializer) : name(name), initializer(initializer) {}

    void accept(StmtVisitor *visitor) {
        visitor->visitVarStmt(this);
    }
    
public:
    Token name;
    Expr *initializer;
};