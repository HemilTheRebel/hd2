#include <iostream>
#include <memory>

#include "string_util.hpp"
#include "scanner.hpp"
#include "errors.hpp"
#include "parser.hpp"
#include "ast_printer.hpp"
#include "interpreter.hpp"

class HD {

    Interpreter interpreter;

    void run(const std::string& input) {
        Scanner scanner(input);
        std::vector<Token> tokens = scanner.scanTokens();

        auto parser = std::make_unique<Parser>(tokens);

        auto statements = parser->parse();

        if (Errors::hadError) return;

        // auto result = ASTPrinter().print(ast);
        // std::cout << std::any_cast<std::string>(result);

        // std::cout << "\n";

        interpreter.interpret(statements);
    }

    public:
    
    int runFile(const std::string& path) {
        std::ifstream file(path);

        std::string contents = slurp(file);

        run(contents);

        if (Errors::hadError) return 65;

        if (Errors::hadRuntimeError) return 70;

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

