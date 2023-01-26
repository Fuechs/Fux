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
    TokenList tokens;
    TokenIter current;
    ErrorManager *error;
    Lexer *lexer;
    RootPtr root;
    const bool mainFile;

    StmtPtr parseStmt();
    StmtPtr parseFunctionDeclStmt();
    StmtPtr parseBlockStmt();
    StmtPtr parseIfElseStmt();
    StmtPtr parsePutsStmt();
    StmtPtr parseReturnStmt();
    StmtPtr parseVariableDeclStmt();

    // TODO: <expr>, <expr>, ...
    ExprList parseExprList();

    ExprPtr parseExpr();
    // <symbol> [...]= <value>
    ExprPtr parseAssignmentExpr();
    // TODO: <bool> ? <expr> : <expr>
    ExprPtr parseTernaryExpr();
    // |
    ExprPtr parseLogicalOrExpr();
    // &&
    ExprPtr parseLogicalAndExpr();
    // |
    ExprPtr parseBitwiseOrExpr();
    // TODO: bitwise or, no operator yet, ^ is taken by pow
    ExprPtr parseBitwiseXorExpr();
    // &
    ExprPtr parseBitwiseAndExpr();
    // ==, !=
    ExprPtr parseEqualityExpr();
    // <, <=, >, >=
    ExprPtr parseRelationalExpr();
    // <|, |>
    ExprPtr parseBitwiseShiftExpr();
    // +, -
    ExprPtr parseAdditiveExpr();
    // *, /, %
    ExprPtr parseMultiplicativeExpr();
    // ^
    ExprPtr parsePowerExpr();

    // & <variable>
    ExprPtr parseAddressExpr();
    // * <address>
    ExprPtr parseDereferenceExpr();
    // TODO: ( <type> ) <value>
    ExprPtr parseTypeCastExpr();
    //° ! (log not), ~ (bit not), ? (existence)
    ExprPtr parseLogBitUnaryExpr();
    // +, -
    ExprPtr parsePlusMinusUnaryExpr();
    // ++x, --x
    ExprPtr parsePreIncDecExpr();
    // TODO: <array> [ <index> ]
    ExprPtr parseIndexExpr();
    // <callee> ( <arguments> )
    ExprPtr parseCallExpr(string symbol = "", StmtList arguments = StmtList());
    // x++, x--
    ExprPtr parsePostIncDecExpr();
    // identifier, value, sub expr
    // x         , 1    , (...)
    ExprPtr parsePrimaryExpr();

    // parse : | -> + pointer depth + type name + array
    FuxType parseType();

    // parse number with correct type
    ExprPtr parseNumberExpr(Token &tok);

    // get next token
    Token &eat();
    // expect an token
    Token expect(TokenType type, ErrorType errType = UNEXPECTED_TOKEN);
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