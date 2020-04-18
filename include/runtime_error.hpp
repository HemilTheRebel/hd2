#pragma once

#include <stdexcept>

#include "tokens.hpp"

class RuntimeError : public std::runtime_error {
public:
    Token token;
    std::string message;

    RuntimeError(Token token, std::string message) 
    : runtime_error(message), token(token) { }

//     const char* what() const noexcept override {
//         return message.c_str();
//     }
};