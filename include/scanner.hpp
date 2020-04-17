#pragma once

#include <vector>
#include <string>
#include <map>
#include <algorithm>

#include "tokens.hpp"
#include "errors.hpp"

std::map<std::string, TokenType> getKeywords() {
    static std::map<std::string, TokenType> keywords;

    keywords.insert( std::make_pair("and",    TokenType::AND)    );                    
    keywords.insert( std::make_pair("class",  TokenType::CLASS)  );                    
    keywords.insert( std::make_pair("else",   TokenType::ELSE)   );                    
    keywords.insert( std::make_pair("false",  TokenType::FALSE)  );                    
    keywords.insert( std::make_pair("for",    TokenType::FOR)    );                    
    keywords.insert( std::make_pair("fun",    TokenType::FUN)    );                    
    keywords.insert( std::make_pair("if",     TokenType::IF)     );                    
    keywords.insert( std::make_pair("nil",    TokenType::NIL)    );                    
    keywords.insert( std::make_pair("or",     TokenType::OR)     );                    
    keywords.insert( std::make_pair("print",  TokenType::PRINT)  );                    
    keywords.insert( std::make_pair("return", TokenType::RETURN) );                    
    keywords.insert( std::make_pair("super",  TokenType::SUPER)  );                    
    keywords.insert( std::make_pair("this",   TokenType::THIS)   );                    
    keywords.insert( std::make_pair("true",   TokenType::TRUE)   );                    
    keywords.insert( std::make_pair("var",    TokenType::VAR)    );                    
    keywords.insert( std::make_pair("while",  TokenType::WHILE)  );

    return keywords;
}

class Scanner {
    std::string source;
    std::vector<Token> tokens;

    static const inline std::map<std::string, TokenType> keywords = getKeywords();

    size_t start = 0, current = 0, line = 1;

    bool isAtEnd() {
        return current >= source.length();
    }

     char advance() {                               
        current++;                                           
        return source.at(current - 1);                   
    }

    void addToken(TokenType type) {                
        addToken(type,  "");                                
    }           

    void addToken(TokenType type, const std::string& literal) {
        std::string text = source.substr(start, current-start);      
        tokens.push_back(Token(type, text, literal, line));
    }

    void scanToken() {
        char c = advance();
        switch (c)
        {
            // single character lexemes
            case '(': addToken(TokenType::LEFT_PAREN   ); break;     
            case ')': addToken(TokenType::RIGHT_PAREN  ); break;    
            case '{': addToken(TokenType::LEFT_BRACE   ); break;     
            case '}': addToken(TokenType::RIGHT_BRACE  ); break;    
            case ',': addToken(TokenType::COMMA        ); break;          
            case '.': addToken(TokenType::DOT          ); break;            
            case '-': addToken(TokenType::MINUS        ); break;          
            case '+': addToken(TokenType::PLUS         ); break;           
            case ';': addToken(TokenType::SEMICOLON    ); break;      
            case '*': addToken(TokenType::STAR         ); break; 

            // two character lexeme
            case '!': addToken(matchToken('=') ? TokenType::BANG_EQUAL : TokenType::BANG); break;      
            case '=': addToken(matchToken('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL); break;    
            case '<': addToken(matchToken('=') ? TokenType::LESS_EQUAL : TokenType::LESS); break;      
            case '>': addToken(matchToken('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER); break;

            case '/':
                // A comment goes until the end of the line.
                // Checks if token is a comment                                                       
                if (matchToken('/')) {                                                            
                    while (peek() != '\n' && !isAtEnd()) 
                        advance();             
                } else {                                                      
                    addToken(TokenType::SLASH);                                            
                }                                                             
            break; 

            // meaningless characters
            case ' ':                                    
            case '\r':                                   
            case '\t':                                   
                // Ignore whitespace.                      
            break;

            case '\n':                                   
                line++;                                    
            break;  

            // string
            case '"': string(); break;
        
            default:
                if (isDigit(c)) {
                    number();
                } else if (isAlpha(c)) {
                    identifier();
                } else {
                    Errors::error(line, "Unexpected character");
                }
            break;
        }
    }

    void identifier() {
        while (isAlphaNumeric(peek())) 
            advance();

        std::string text = source.substr(start, current - start);

        TokenType type;

        // if text is a keyword, set type to the keyword
        if (auto it = keywords.find(text); it != keywords.cend()) {
            type = it->second;
        } else {
            // else it is an identifier
            type = TokenType::IDENTIFIER;
        }

        addToken(type);

    }

    bool isAlpha(char c) {
        return (c >= 'a' && c <= 'z') || 
               (c >= 'A' && c <= 'Z') ||
               c == '_';
    }

    bool isDigit(char c) {
        return c >= '0' && c <= '9';
    }

    bool isAlphaNumeric(char c) {
        return isAlpha(c) || isDigit(c);      
    }       

    void number() {
        // while you find a number, continue reading
        while (isDigit(peek())) advance();

        // if number contains fractional part
        if (peek() == '.' && isDigit(peekNext())) {
            advance();

            // continue reading digits after decimal point
            while (isDigit(peek())) advance();
        }

        addToken(TokenType::NUMBER, source.substr(start, current - start));
    }

    char peekNext() {
        if (current + 1 >= source.length())
            return '\0';
        
        return source.at(current + 1);
    }

    void string() {
        while (peek() != '"' && !isAtEnd()) {
            if (peek() == '\n')
                line++;
            
            advance();
        }

        // if string i untermianted
        if (isAtEnd()) {
            Errors::error(line, "Unterminated string");
            return;
        }

        // read the ending '"'
        advance();

        // trim double qoutes
        std::string value = source.substr(start + 1, current - start - 2);
        addToken(TokenType::STRING, value);
    }

    char peek() {           
        if (isAtEnd()) return '\0';   
        return source.at(current);
    }               

    bool matchToken(char expected) {
        if (isAtEnd())
            return false;
        if (source.at(current) != expected)
            return false;
        
        current++;
        return true;
    }

    public:

    Scanner(const std::string& source)
    : source(source) { }
    
    std::vector<Token> scanTokens() {
        while (!isAtEnd()){
            start = current;
            scanToken();
        }

        tokens.push_back(Token(TokenType::EndOfFile, "", "", line));
        return tokens;
    }

};