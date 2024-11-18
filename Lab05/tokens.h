#ifndef TOKEN_H
#define TOKEN_H

#include <string>
using std::string;

// Enumeração dos tags dos tokens
enum Tag {
    ID = 256, INTEGER, FLOATING, BOOLEAN, TRUE, FALSE, MAIN, IF, ELSE, WHILE, DO, OR, AND, EQ, NEQ, LT, LTE, GT, GTE
};

// Estrutura que representa um token
struct Token {
    int tag;
    string lexeme;

    Token() : tag(0) {}
    Token(char ch) : tag(int(ch)), lexeme({ch}) {}
    Token(int t, const char* s) : tag(t), lexeme(s) {}
};

#endif // TOKEN_H
