#include <iostream>

#include "string_util.hpp"
#include "scanner.hpp"
#include "errors.hpp"

class Lox {
    void run(const std::string& input) {
        Scanner scanner(input);
        std::vector<Token> tokens = scanner.scanTokens();

        for(Token token : tokens) {
            std::cout << token << "\n";
        }
    }

    public:
    
    int runFile(const std::string& path) {
        std::ifstream file(path);

        std::string contents = slurp(file);

        run(contents);

        if (Errors::hadError) {
            return 65;
        }

        return 0;
    }

    int runPrompt() {
        while (true) {
            std::cout << "\nhd> ";
            
            std::string input;
            std::getline(std::cin, input);

            run(input);

            Errors::hadError = false;
        }
    }
};

