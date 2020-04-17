#pragma once

#include <iostream>

#include "runtime_error.hpp"
class Errors {
    public:
    static inline bool hadError = false;
    static inline bool hadRuntimeError = false;
    
    static void report(int line, const std::string& where, const std::string& message) {
        std::cerr << "[Line " << line << "] Error " << where << " : " << message << '\n';
        hadError = true;
    }

    static void error(int line, const std::string& message) {
        report(line, "", message);
    }

    static void error(Token token, const std::string& message) {
        if (token.type == TokenType::EndOfFile) {
            report(token.line, " at end", message);
        } else  {
            report(token.line, " at '" + token.lexeme + "'", message);
        }
    }

    static void runtimeError(RuntimeError &e) {
        std::cerr << e.what() << "\n[line " + std::to_string(e.token.line) + "]\n";
        hadRuntimeError = true;
    }
};

