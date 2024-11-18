#include "lexer.h"
#include <FlexLexer.h>
#include <fstream>
#include <sstream>

using std::stringstream;

extern std::ifstream fin;

extern int yylex();
extern Lexer * scanner;


// retorna número da linha atual
int Lexer::Lineno()
{
	return line;
}

// retorna tokens para o analisador sintático
Token * Lexer::Scan()
{ 
	if ((lookahead = yylex()) != 0){
        token = Token{lookahead, YYText()};
        return &token;
    } else
		return nullptr;
} 
