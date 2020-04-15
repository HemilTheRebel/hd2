#include "expression.hpp"
#include "ast_printer.hpp"

int main() {
    Expr *expression = new BinaryExpr(
        new UnaryExpr(
            Token(TokenType::MINUS, "-", "", 1),
            new LiteralExpr("123")),
        Token(TokenType::STAR, "*", "", 1),
        new GroupingExpr( new LiteralExpr("45.67"))
    );
    
    std::cout << std::any_cast<std::string>(ASTPrinter().print(expression)) << std::endl;
}