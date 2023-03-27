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
    RootAST::Ptr parse();

private:
    const string &fileName;
    Token::Vec tokens;
    Token::Iter current;
    ErrorManager *error;
    Lexer *lexer;
    RootAST::Ptr root;
    const bool mainFile;

    FunctionAST *parent = nullptr;

    // parse a statement
    StmtAST::Ptr parseStmt(bool expectSemicolon = true);
    // enumeration
    StmtAST::Ptr parseEnumStmt();
    // function declaration
    StmtAST::Ptr parseFunctionDeclStmt();
    // for (each) loop
    StmtAST::Ptr parseForLoopStmt();
    // (do ...) while loop
    StmtAST::Ptr parseWhileLoopStmt();
    // code block {}
    StmtAST::Ptr parseBlockStmt();
    // if else statement
    StmtAST::Ptr parseIfElseStmt();
    // inbuilt call 
    StmtAST::Ptr parseInbuiltCallStmt();
    // variable declaration
    StmtAST::Ptr parseVariableDeclStmt();

    // <expr>, <expr>, ...
    ExprAST::Vec parseExprList(TokenType end);

    // parse an expression
    ExprAST::Ptr parseExpr();
    // <symbol> [...]= <value>
    ExprAST::Ptr parseAssignmentExpr();
    // <expr> <<|>> <expr>
    ExprAST::Ptr parsePipeExpr();
    // <expr> ? <expr> : <expr>
    ExprAST::Ptr parseTernaryExpr();
    // <expr> || <expr>
    ExprAST::Ptr parseLogicalOrExpr();
    // <expr> && <expr> 
    ExprAST::Ptr parseLogicalAndExpr();
    // <expr> | <expr> 
    ExprAST::Ptr parseBitwiseOrExpr();
    // <expr> >< <expr> 
    ExprAST::Ptr parseBitwiseXorExpr();
    // <expr> & <expr> 
    ExprAST::Ptr parseBitwiseAndExpr();
    // <expr> == <expr> , <expr> != <expr> 
    ExprAST::Ptr parseEqualityExpr();
    // <expr> < <expr> , <expr> <= <expr> , <expr> > <expr> , <expr> >= <expr> 
    ExprAST::Ptr parseRelationalExpr();
    // <expr> <| <expr> , <expr> |> <expr> 
    ExprAST::Ptr parseBitwiseShiftExpr();
    // <expr> + <expr> , <expr> - <expr> 
    ExprAST::Ptr parseAdditiveExpr();
    // <expr> * <expr> , <expr> / <expr> , <expr> % <expr> 
    ExprAST::Ptr parseMultiplicativeExpr();
    // <expr> ^ <expr> 
    ExprAST::Ptr parsePowerExpr();

    // & <expr>
    ExprAST::Ptr parseAddressExpr();
    // * <expr>
    ExprAST::Ptr parseDereferenceExpr();
    // ( <type> ) <expr>
    ExprAST::Ptr parseTypeCastExpr();
    //° ! (log not), ~ (bit not), ? (existence)
    ExprAST::Ptr parseLogBitUnaryExpr();
    // + <expr> , - <expr> 
    ExprAST::Ptr parsePlusMinusUnaryExpr();
    // ++<expr> , --<expr> 
    ExprAST::Ptr parsePreIncDecExpr();
    // <expr>++, <expr>--
    ExprAST::Ptr parsePostIncDecExpr();
    // <expr>.<symbol> 
    ExprAST::Ptr parseTopMemberExpr();
    // <expr> [ <expr> ]
    ExprAST::Ptr parseIndexExpr(ExprAST::Ptr parent = nullptr);
    // <expr>.<symbol>
    ExprAST::Ptr parseMidMemberExpr();
    // <expr>  ( <expr>, <expr>, ... )
    ExprAST::Ptr parseCallExpr(ExprAST::Ptr callee = nullptr);
    // <expr>.<symbol>
    ExprAST::Ptr parseBotMemberExpr();
    // <identifier>, <value>, (<expr>)
    ExprAST::Ptr parsePrimaryExpr();

    /**
     * Since member expressions can contain 
     *  call expressions, index expressions
     *  and vice versa, there are three different 
     *  functions to parse a member expression.
     * - TOP: Has Call, Index 
     * - MID: Has Call, Can be carried by Index
     * - BOT: Can be carried by Call, Index
     */

    // parse : | -> + access + pointer depth + type name + array
    // primitive: only parse pointer depth + type name + array
    FuxType parseType(bool primitive = false);

    // parse number with correct type
    ExprAST::Ptr parseNumberExpr(Token &tok);
    // parse char with correct type and escape sequence
    ExprAST::Ptr parseCharExpr(Token &tok);

    // get next token
    Token &eat();
    // expect and get next token
    Token &eat(TokenType type, ParseError::Type = ParseError::UNEXPECTED_TOKEN);
    // peek to Nth token
    Token &peek(size_t steps = 1);
    // check current token and advance if true
    bool check(TokenType type);
    // check current and next token ...
    bool check(TokenType type, TokenType type0);
    // advance until given tokentype is reached (error recovery)
    void recover(TokenType type = SEMICOLON);

    // check wether end of file is reached
    constexpr bool notEOF();

    void createError(
        ParseError::Type type, string title, 
        const Token &token, string info, size_t ptr = 0, string ptrText = "",
        bool warning = false, bool aggressive = false);
    
    void createError(
        ParseError::Type type, string title,
        const Token &token, string info, 
        const Token &refTok, string refInfo,
        bool warning = false, bool aggressive = true);

    void debugPrint(const string message);
};