#pragma once

#include <iostream>

class Errors {
    public:
    static inline bool hadError = false;
    
    static void report(int line, const std::string& where, const std::string& message) {
        std::cerr << "[Line " << line << "] Error " << where << " : " << message << '\n';
        hadError = true;
    }

    static void error(int line, const std::string& message) {
        report(line, "", message);
    }
};

