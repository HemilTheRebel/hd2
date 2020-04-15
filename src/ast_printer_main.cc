#include "expression.hpp"
#include "ast_printer.hpp"

int main() {
    Expr *expression = new BinaryExpr(
        new UnaryExpr(
            Token(TokenType::MINUS, "-", nullptr, 1),
            new LiteralExpr("123")),
        Token(TokenType::STAR, "*", nullptr, 1),
        new GroupingExpr( new LiteralExpr("45.67"))
    );
    
    ASTPrinter().print(expression);

    std::cout << '\n';
}