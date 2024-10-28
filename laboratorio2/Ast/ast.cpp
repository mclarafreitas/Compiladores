#include <iostream>
#include <sstream>
#include <cctype>
#include "ast.h"
#include "error.h"
using std::stringstream;

extern Lexer *scanner;

// Verifica se a string representa um número
bool isNumber(const std::string& s) {
    if (s.empty()) return false;
    size_t start = (s[0] == '-' || s[0] == '+') ? 1 : 0;
    for (size_t i = start; i < s.size(); ++i) {
        if (!std::isdigit(s[i])) return false;
    }
    return true;
}

// Converte string para bool: "0" -> false, valores numéricos não zero -> true
bool convertToBool(const std::string& s) {
    if (!isNumber(s)) {
        // Ignorar a conversão para não numéricos (como variáveis)
        return false;
    }
    return std::stoi(s) != 0;
}

// ----
// Node
// ----

Node::Node() : node_type(NodeType::UNKNOWN) {}
Node::Node(int t) : node_type(t) {}

// ---------
// Statement
// ---------

Statement::Statement() : Node(NodeType::STMT) {}
Statement::Statement(int type) : Node(type) {}

// ----------
// Expression
// ----------

Expression::Expression(Token *t) : Node(NodeType::EXPR), type(ExprType::VOID), token(t) {}
Expression::Expression(int ntype, int etype, Token *t) : Node(ntype), type(etype), token(t) {}

string Expression::Name()
{
    return token->lexeme;
}

string Expression::Type()
{
    switch (type)
    {
    case ExprType::INT:
        return "int";
    case ExprType::FLOAT:
        return "float";
    case ExprType::BOOL:
        return "bool";
    default:
        return "void";
    }
}

// --------
// Constant
// --------

Constant::Constant(int etype, Token *t) : Expression(NodeType::CONSTANT, etype, t) {}

// ----------
// Identifier
// ----------

Identifier::Identifier(int etype, Token *t) : Expression(NodeType::IDENTIFIER, etype, t) {}

// ------
// Access
// ------

Access::Access(int etype, Token * t, Expression * i, Expression * e): Expression(NodeType::ACCESS, etype, t), id(i), expr(e) {}

// -------
// Logical
// -------

Logical::Logical(Token *t, Expression *e1, Expression *e2) 
    : Expression(NodeType::LOG, ExprType::BOOL, t), expr1(e1), expr2(e2) 
{
    // Verificação de tipos após conversão
    if (expr1->type != ExprType::BOOL || expr2->type != ExprType::BOOL) {
        std::stringstream ss;
        ss << "\'" << token->lexeme << "\' usado com operandos não booleanos (" 
           << expr1->Name() << ":" << expr1->Type() << ") (" 
           << expr2->Name() << ":" << expr2->Type() << ") ";
        throw SyntaxError{scanner->Lineno(), ss.str()};
    }
}

// ----------
// Relational
// ----------

Relational::Relational(Token *t, Expression *e1, Expression *e2) 
    : Expression(NodeType::REL, ExprType::BOOL, t), expr1(e1), expr2(e2) 
{
    // Verificação de tipos após conversão
    if (expr1->type != expr2->type) {
        std::stringstream ss;
        ss << "\'" << token->lexeme << "\' usado com operandos de tipos diferentes (" 
           << expr1->Name() << ":" << expr1->Type() << ") (" 
           << expr2->Name() << ":" << expr2->Type() << ") ";
        throw SyntaxError{scanner->Lineno(), ss.str()};
    }
}

// ----------
// Arithmetic
// ----------

Arithmetic::Arithmetic(int etype, Token *t, Expression *e1, Expression *e2) : Expression(NodeType::ARI, etype, t), expr1(e1), expr2(e2)
{
    // Verificação de tipos
    if (expr1->type != expr2->type)
    {
        stringstream ss;
        ss << "\'" << token->lexeme << "\' usado com operandos de tipos diferentes ("
           << expr1->Name() << ":" << expr1->Type() << ") ("
           << expr2->Name() << ":" << expr2->Type() << ") ";
        throw SyntaxError{scanner->Lineno(), ss.str()};
    }
}

// ---------
// UnaryExpr
// ---------

UnaryExpr::UnaryExpr(int etype, Token *t, Expression *e) 
    : Expression(NodeType::UNARY, etype, t), expr(e) 
{
    // Verificação de tipo
    if (expr->type != ExprType::BOOL) {
        std::stringstream ss;
        ss << "\'" << token->lexeme << "\' usado com operando não booleano (" 
           << expr->Name() << ":" << expr->Type() << ")";
        throw SyntaxError{scanner->Lineno(), ss.str()};
    }
}

// ----
// Seq
// ----

Seq::Seq(Statement *s, Statement *ss) : Statement(NodeType::SEQ), stmt(s), stmts(ss) {}

// ------
// Assign
// ------

Assign::Assign(Expression *i, Expression *e) : Statement(NodeType::ASSIGN), id(i), expr(e)
{
    // Conversão de int para bool em atribuições
    if (id->type == ExprType::BOOL && expr->type == ExprType::INT) {
        expr->type = ExprType::BOOL;
        expr->token->lexeme = (expr->token->lexeme == "0") ? "false" : "true";
    }
    if (id->type != expr->type)
    {
        stringstream ss;
        ss << "\'=\' usado com operandos de tipos diferentes ("
           << id->Name() << ":" << id->Type() << ") ("
           << expr->Name() << ":" << expr->Type() << ") ";
        throw SyntaxError{scanner->Lineno(), ss.str()};
    }
}

// ----
// If
// ----

If::If(Expression *e, Statement *s) : Statement(NodeType::IF_STMT), expr(e), stmt(s)
{
    // Verificação de tipo
    if (expr->type != ExprType::BOOL) {
        expr->type = ExprType::BOOL;
    }
}

// -----
// While
// -----

While::While(Expression *e, Statement *s) : Statement(NodeType::WHILE_STMT), expr(e), stmt(s)
{
    // Verificação de tipo
    if (expr->type != ExprType::BOOL) {
        expr->type = ExprType::BOOL;
    }
}

// --------
// Do-While
// --------

DoWhile::DoWhile(Statement *s, Expression *e) : Statement(NodeType::DOWHILE_STMT), stmt(s), expr(e)
{
    // Verificação de tipo
    if (expr->type != ExprType::BOOL) {
        expr->type = ExprType::BOOL;
    }
}
