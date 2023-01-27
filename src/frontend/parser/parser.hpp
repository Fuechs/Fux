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

    // parse a statement
    StmtPtr parseStmt();
    // function declaration
    StmtPtr parseFunctionDeclStmt();
    // TODO: for (-each) loop
    StmtPtr parseForLoopStmt();
    // TODO: (do) while loop
    StmtPtr parseWhileLoopStmt();
    // code block {}
    StmtPtr parseBlockStmt();
    // if statement
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
    // <bool> ? <expr> : <expr>
    ExprPtr parseTernaryExpr();
    // |
    ExprPtr parseLogicalOrExpr();
    // &&
    ExprPtr parseLogicalAndExpr();
    // |
    ExprPtr parseBitwiseOrExpr();
    // TODO: bitwise xor, no operator yet, ^ is taken by pow
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
    // ( <type> ) <value>
    ExprPtr parseTypeCastExpr();
    //° ! (log not), ~ (bit not), ? (existence)
    ExprPtr parseLogBitUnaryExpr();
    // +, -
    ExprPtr parsePlusMinusUnaryExpr();
    // ++x, --x
    ExprPtr parsePreIncDecExpr();
    // <array> [ <index> ]
    ExprPtr parseIndexExpr();
    // <callee> ( <arguments> )
    ExprPtr parseCallExpr(ExprPtr &symbol = nullExpr, StmtList arguments = StmtList());
    // x++, x--
    ExprPtr parsePostIncDecExpr();
    // identifier, value, sub expr
    // x         , 1    , (...)
    ExprPtr parsePrimaryExpr();

    // parse : | -> + access + pointer depth + type name + array
    // primitive: only parse pointer depth + type name + array
    FuxType parseType(bool primitive = false);

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