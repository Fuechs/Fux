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
    typedef pair<_i64, FuxType::AccessList> TypePrefix;
    typedef std::optional<TypePrefix> OptTypePrefix;
    typedef std::optional<FuxType> OptType;

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
    StmtPtr parseVariableDeclStmt(TypePrefix typePrefix = TypePrefix(false, {}));

    ExprPtr parseExpr();
    // TODO: <expr>, <expr>, ...
    ExprPtr parseExprList();
    // <symbol> = <value>
    ExprPtr parseAssignmentExpr();
    // TODO: <callee> ( <arguments> )
    ExprPtr parseCallExpr();
    // TODO: ||, &&
    ExprPtr parseLogicalExpr();
    // TODO: ==, !=, <, <=, >, >=
    ExprPtr parseComparisonExpr();
    // +, -
    ExprPtr parseAdditiveExpr();
    // *, /, %
    ExprPtr parseMultiplicativeExpr();
    // ^
    ExprPtr parsePowerExpr();
    // TODO: -, !
    ExprPtr parseUnaryExpr();
    // identifier, value, sub expr
    // x         , 1    , (...)
    ExprPtr parsePrimaryExpr();
    
    // parse type name + array types
    FuxType parseTypeName(TypePrefix &typePrefix);
    // parse access and pointer depth
    OptTypePrefix parseTypePrefix();
    // parse type suffix (: | -> ...)
    OptType parseTypeSuffix(TypePrefix &typePrefix);

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