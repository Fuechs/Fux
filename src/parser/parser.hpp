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

#include "../fux.hpp"
#include "../ast/ast.hpp"
#include "type.hpp"
#include "../error/error.hpp"
#include "../lexer/lexer.hpp"
#include "../lexer/token.hpp"

class Parser {
public:
    using Ptr = shared_ptr<Parser>;

    Parser(const string &fileName, const string &source, const bool mainFile = false);
    ~Parser();

    // parse the Tokens and return AST root
    Root::Ptr parse();

private:
    const string &fileName;
    Token::Vec tokens;
    Token::Iter current;
    Lexer *lexer;
    Root::Ptr root;
    const bool mainFile;

    // current parent function
    FunctionStmt *parent = nullptr;

    // parse a statement
    Stmt::Ptr parseStmt(bool expectSemicolon = true);
    // macro
    Stmt::Ptr parseMacroStmt();
    // enumeration
    Stmt::Ptr parseEnumStmt();
    // function declaration
    Stmt::Ptr parseFunctionDeclStmt();
    // for (each) loop
    Stmt::Ptr parseForLoopStmt();
    // (do ...) while loop
    Stmt::Ptr parseWhileLoopStmt();
    // code block {}
    Stmt::Ptr parseBlockStmt();
    // if else statement
    Stmt::Ptr parseIfElseStmt();
    // inbuilt call 
    Stmt::Ptr parseInbuiltCallStmt();
    // variable declaration
    Stmt::Ptr parseVariableStmt();

    // <expr>, <expr>, ...
    Expr::Vec parseExprList(TokenType end);

    // parse an expression
    Expr::Ptr parseExpr();
    // <symbol> [...]= <value>
    Expr::Ptr parseAssignmentExpr();
    // <expr> <<|>> <expr>
    Expr::Ptr parsePipeExpr();
    // <expr> ? <expr> : <expr>
    Expr::Ptr parseTernaryExpr();
    // <expr> || <expr>
    Expr::Ptr parseLogicalOrExpr();
    // <expr> && <expr> 
    Expr::Ptr parseLogicalAndExpr();
    // <expr> | <expr> 
    Expr::Ptr parseBitwiseOrExpr();
    // <expr> >< <expr> 
    Expr::Ptr parseBitwiseXorExpr();
    // <expr> & <expr> 
    Expr::Ptr parseBitwiseAndExpr();
    // <expr> == <expr> , <expr> != <expr> 
    Expr::Ptr parseEqualityExpr();
    // <expr> < <expr> , <expr> <= <expr> , <expr> > <expr> , <expr> >= <expr> 
    Expr::Ptr parseRelationalExpr();
    // <expr> <| <expr> , <expr> |> <expr> 
    Expr::Ptr parseBitwiseShiftExpr();
    // <expr> + <expr> , <expr> - <expr> 
    Expr::Ptr parseAdditiveExpr();
    // <expr> * <expr> , <expr> / <expr> , <expr> % <expr> 
    Expr::Ptr parseMultiplicativeExpr();
    // <expr> ^ <expr> 
    Expr::Ptr parsePowerExpr();

    // & <expr>
    Expr::Ptr parseAddressExpr();
    // * <expr>
    Expr::Ptr parseDereferenceExpr();
    // ( <type> ) <expr>
    Expr::Ptr parseTypeCastExpr();
    //° ! (log not), ~ (bit not), ? (existence)
    Expr::Ptr parseLogBitUnaryExpr();
    // + <expr> , - <expr> 
    Expr::Ptr parsePlusMinusUnaryExpr();
    // ++<expr> , --<expr> 
    Expr::Ptr parsePreIncDecExpr(Expr::Ptr parent = nullptr);
    // <expr>.<symbol> 
    Expr::Ptr parseMemberExpr();
    // <expr>++, <expr>--
    Expr::Ptr parsePostIncDecExpr(Expr::Ptr parent = nullptr); 
    // <expr> [ <expr> ]
    Expr::Ptr parseIndexExpr(Expr::Ptr parent = nullptr);
    // <expr>  ( <expr>, <expr>, ... )
    Expr::Ptr parseCallExpr(Expr::Ptr callee = nullptr);
    // <identifier>, <value>, (<expr>)
    Expr::Ptr parsePrimaryExpr();

    // parse a type
    // needColon: requires ':' or '->' before type
    Fux::Type parseType(bool needColon = false);
    Fux::Type parseReferenceType();
    Fux::Type parseArrayType();
    Fux::Type parsePointerType();
    Fux::Type parsePrimaryType();

    // parse number with correct type
    Expr::Ptr parseNumberExpr(Token &tok);
    // parse char with correct type and escape sequence
    Expr::Ptr parseCharExpr(Token &tok);

    // ( <args> ) -> <stmt>
    MacroStmt::Case *parseMacroCase();
    // <symbol> : <type>
    MacroStmt::Arg parseMacroArg();

    // parse a function parameter
    FunctionStmt::Parameter::Ptr parseFuncParameter();

    // get next token
    Token &eat();
    // expect and get next token
    Token &eat(TokenType type, Error::Type = Error::UNEXPECTED_TOKEN);
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
        Error::Type type, string title, 
        const Token &token, string info, size_t ptr = 0, string ptrText = "",
        bool warning = false, bool aggressive = false);
    
    void createError(
        Error::Type type, string title,
        const Token &token, string info, 
        const Token &refTok, string refInfo,
        bool warning = false, bool aggressive = true);

    void debugPrint(const string message);
};