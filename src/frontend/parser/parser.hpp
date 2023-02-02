/**
 * @file parser.hpp
 * @author fuechs
 * @brief fux parser header
 * @version 0.1
 * @date 2022-11-02
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#pragma once

#include "../../fux.hpp"
#include "../ast/ast.hpp"
#include "type.hpp"
#include "../error/error.hpp"
#include "../lexer/lexer.hpp"
#include "../lexer/token.hpp"

class Parser {
public:
    Parser(ErrorManager *error, const string &fileName, const string &source, const bool mainFile = false);
    ~Parser();

    // parse the Tokens and return AST root
    RootPtr parse();

private:
    Token::Vec tokens;
    Token::Iter current;
    ErrorManager *error;
    Lexer *lexer;
    RootPtr root;
    const bool mainFile;

    // parse a statement
    StmtPtr parseStmt(bool expectSemicolon = true);
    // function declaration
    StmtPtr parseFunctionDeclStmt();
    // for (each) loop
    StmtPtr parseForLoopStmt();
    // (do ...) while loop
    StmtPtr parseWhileLoopStmt();
    // code block {}
    StmtPtr parseBlockStmt();
    // if else statement
    StmtPtr parseIfElseStmt();
    // inbuilt call 
    StmtPtr parseInbuiltCallStmt();
    // variable declaration
    StmtPtr parseVariableDeclStmt();

    // <expr>, <expr>, ...
    ExprList parseExprList();

    // parse an expression
    ExprPtr parseExpr();
    // <symbol> [...]= <value>
    ExprPtr parseAssignmentExpr();
    // <expr> <<|>> <expr>
    ExprPtr parsePipeExpr();
    // <expr> ? <expr> : <expr>
    ExprPtr parseTernaryExpr();
    // <expr> || <expr>
    ExprPtr parseLogicalOrExpr();
    // <expr> && <expr> 
    ExprPtr parseLogicalAndExpr();
    // <expr> | <expr> 
    ExprPtr parseBitwiseOrExpr();
    // <expr> >< <expr> 
    ExprPtr parseBitwiseXorExpr();
    // <expr> & <expr> 
    ExprPtr parseBitwiseAndExpr();
    // <expr> == <expr> , <expr> != <expr> 
    ExprPtr parseEqualityExpr();
    // <expr> < <expr> , <expr> <= <expr> , <expr> > <expr> , <expr> >= <expr> 
    ExprPtr parseRelationalExpr();
    // <expr> <| <expr> , <expr> |> <expr> 
    ExprPtr parseBitwiseShiftExpr();
    // <expr> + <expr> , <expr> - <expr> 
    ExprPtr parseAdditiveExpr();
    // <expr> * <expr> , <expr> / <expr> , <expr> % <expr> 
    ExprPtr parseMultiplicativeExpr();
    // <expr> ^ <expr> 
    ExprPtr parsePowerExpr();

    // & <expr>
    ExprPtr parseAddressExpr();
    // * <expr>
    ExprPtr parseDereferenceExpr();
    // ( <type> ) <expr>
    ExprPtr parseTypeCastExpr();
    //° ! (log not), ~ (bit not), ? (existence)
    ExprPtr parseLogBitUnaryExpr();
    // + <expr> , - <expr> 
    ExprPtr parsePlusMinusUnaryExpr();
    // ++<expr> , --<expr> 
    ExprPtr parsePreIncDecExpr();
    // <expr> [ <expr> ]
    ExprPtr parseIndexExpr();
    // <expr>  ( <expr>, <expr>, ... )
    ExprPtr parseCallExpr();
    // <expr>++, <expr>--
    ExprPtr parsePostIncDecExpr();
    // <identifier>, <value>, (<expr>)
    ExprPtr parsePrimaryExpr();

    // parse : | -> + access + pointer depth + type name + array
    // primitive: only parse pointer depth + type name + array
    FuxType parseType(bool primitive = false);

    // parse number with correct type
    ExprPtr parseNumberExpr(Token &tok);
    // parse char with correct type and escape sequence
    ExprPtr parseCharExpr(Token &tok);

    // get next token
    Token &eat();
    // expect and get next token
    Token &eat(TokenType type, ErrorType = UNEXPECTED_TOKEN);
    // peek to Nth token
    Token &peek(size_t steps = 1);
    // check current token and advance if true
    bool check(TokenType type);
    // check curret and next token ...
    bool check(TokenType type, TokenType type0);

    // check wether end of file is reached
    constexpr bool notEOF();

    void debugPrint(const string message);
};