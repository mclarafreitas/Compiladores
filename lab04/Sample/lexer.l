%{
#include <iostream>
#include "tokens.h"
using std::cout;
%}

%option noyywrap

delim	    [ \t\n]
whitespace  {delim}+
letter	    [A-Za-z]
digit	    [0-9]
identifier  ({letter}|_)({letter}|{digit}|_)*

number	    {digit}+(\.{digit}+)?(E[+-]?{digit}+)?

str_literal  \"([^"\\]|\\.)*\"

%%

{whitespace}    /* Ignorar espaços em branco e quebras de linha */ ;
if		        return IF;
then		    return THEN;
else		    return ELSE;
while           return WHILE;
{identifier}    return ID;
{number}	    return NUM;
{str_literal}   return STRING;
"<"		        return RELOP;
"<="		    return RELOP;
"=="		    return RELOP;
"!="		    return RELOP;
">"		        return RELOP;
">="		    return RELOP;
.               cout << YYText() << " é um token inválido!\n";
%%

