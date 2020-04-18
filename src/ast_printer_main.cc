#include "expression.hpp"
#include "ast_printer.hpp"

int main() {
    // Expr *expression = new BinaryExpr(
    //     new UnaryExpr(
    //         Token(TokenType::MINUS, "-", "", 1),
    //         new LiteralExpr(123)),
    //     Token(TokenType::STAR, "*", "", 1),
    //     new GroupingExpr( new LiteralExpr(45.67))
    // );
    
    // Expr *expression = new BinaryExpr(
    //     new LiteralExpr(1.0),
    //     Token(TokenType::EQUAL_EQUAL, "", "", 1),
    //     new LiteralExpr(1.0)
    // );

    // auto value = ASTPrinter().print(expression);
    // std::cout << std::any_cast<std::string>(value) << std::endl;
}