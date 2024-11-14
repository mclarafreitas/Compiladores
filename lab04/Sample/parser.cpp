#include "parser.h"
#include "tokens.h"
#include <iostream>
#include <string>

using std::cout;
using std::string;

// Função para remover escapes de strings, tratando \" e \\.
string processString(const string& str) {
    string processed;
    bool in_escape = false;
    for (size_t i = 1; i < str.length() - 1; ++i) {  // Ignorar as aspas de abertura e fechamento
        if (str[i] == '\\' && (i + 1) < str.length()) {
            if (str[i + 1] == '"' || str[i + 1] == '\\') {
                processed += str[i + 1];  // Adiciona o caractere escapado
                ++i;  // Pula o caractere de escape
            } else {
                processed += str[i];  // Adiciona o caractere não escapado
            }
        } else {
            processed += str[i];  // Adiciona o caractere normal
        }
    }
    return processed;
}



void Parser::Start() {
    // Executa o parser enquanto ainda houver tokens
    while ((lookahead = scanner.yylex()) != 0) {
        switch(lookahead) {
            case IF: cout << "IF\n"; break;
            case THEN: cout << "THEN\n"; break;
            case ELSE: cout << "ELSE\n"; break;
            case WHILE: cout << "WHILE\n"; break;
            case ID: cout << "ID: " << scanner.YYText() << "\n"; break;
            case NUM: cout << "NUM: " << scanner.YYText() << "\n"; break;
            case RELOP: cout << "RELOP: " << scanner.YYText() << "\n"; break;
            case STRING: 
                {
                    string unescaped = processString(scanner.YYText());
                    cout << "STRING: " << unescaped << "\n";
                }
                break;
            default: cout << "Token desconhecido: " << scanner.YYText() << "\n"; break;
        }
    }
}
