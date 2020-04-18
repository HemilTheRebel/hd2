#include "../include/hd.hpp"

int main(int argc, char* argv[]) {
    
    HD hd;

    if (argc > 2) {
        std::cout << "Usage: jlox [script]\n";
        return 64;
    }

    if (argc == 2) {
        return hd.runFile(argv[1]);
    } else {
        return hd.runPrompt();
    }

}