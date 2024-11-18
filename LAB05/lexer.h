#ifndef COMPILER_LEXER
#define COMPILER_LEXER

#include <FlexLexer.h>
#include "tokens.h"
#include <string>

#include <unordered_map>
using std::unordered_map;
using std::string;

struct Token
{
	int tag;
	std::string lexeme;

	Token() : tag(0) {}
	Token(char ch) : tag(int(ch)), lexeme({ch}) {}
	Token(int t, std::string s) : tag(t), lexeme(s) {}
};


// analisador léxico
class Lexer : public yyFlexLexer
{
private:
	int line = 1;		// número da linha atual
	int lookahead;
	Token token;		// último token retornado
public:
	explicit Lexer(std::istream* in) : yyFlexLexer(in) {}
	int Lineno();		// retorna linha atual
	Token * Scan();		// retorna próximo token da entrada
};

#endif
