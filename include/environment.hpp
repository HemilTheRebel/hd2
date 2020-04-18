#pragma once

#include <unordered_map>
#include <any>

#include "runtime_error.hpp"

class Environment {
    std::unordered_map<std::string, std::any> values;

public:
    void define(std::string name, std::any value) {
        values.insert_or_assign(name, value);
    }

    void assign(Token name, std::any value) {
        if (values.find(name.lexeme) != values.end()) {
            values[name.lexeme] = value;
            return;
        }

        throw RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
    }

    std::any get(Token name) {
        if (values.find(name.lexeme) != values.cend()) {
            return values[name.lexeme];
        }

        throw RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
    }
};