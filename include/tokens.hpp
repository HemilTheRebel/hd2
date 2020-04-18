#pragma once

#include <string>
#include <any>
#include <sstream>
#include <vector>

/*
  Copied from stack overflow. This macro defines operator << for enum
*/

#define AWESOME_ENUM(name, ...) enum class name { __VA_ARGS__, __COUNT}; \
std::string toString(name value) { \
  std::string enumName = #name; \
  std::string str = #__VA_ARGS__; \
  int len = str.length(); \
  std::vector<std::string> strings; \
  std::ostringstream temp; \
  for(int i = 0; i < len; i ++) { \
    if(isspace(str[i])) continue; \
    else if(str[i] == ',') { \
      strings.push_back(temp.str()); \
      temp.str(std::string());\
    } \
    else temp<< str[i]; \
} \
strings.push_back(temp.str()); \
return strings[static_cast<int>(value)]; \
} \
inline std::ostream& operator<<(std::ostream& os, name value) { \
 \
os << #name << "::" << toString(value); \
return os;} \

AWESOME_ENUM (TokenType,                            
  // Single-character tokens.                      
  LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
  COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR, 

  // One or two character tokens.                  
  BANG, BANG_EQUAL,                                
  EQUAL, EQUAL_EQUAL,                              
  GREATER, GREATER_EQUAL,                          
  LESS, LESS_EQUAL,                                

  // Literals.                                     
  IDENTIFIER, STRING, NUMBER,                      

  // Keywords.                                     
  AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,  
  PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,    

  EndOfFile                                              
);

class Token {

public:
    TokenType type;
    std::string lexeme;
    std::string literal;
    int line;

    Token(TokenType type, const std::string& lexeme, std::string literal, int line)
    : type(type), lexeme(lexeme), literal(literal), line(line) {}

    
    friend std::ostream& operator <<(std::ostream& out, Token t);
};

std::ostream& operator<<(std::ostream& out, Token t) {
    out << t.type << " " << t.lexeme << " " << t.literal;
    return out;
}