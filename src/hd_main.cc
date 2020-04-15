#include "../include/lox.hpp"

int main(int argc, char* argv[]) {
    
    Lox lox;

    if (argc > 2) {
        std::cout << "Usage: jlox [script]\n";
        return 64;
    }

    if (argc == 2) {
        return lox.runFile(argv[1]);
    } else {
        return lox.runPrompt();
    }

}