/**
 * @file parser.hpp
 * @author fuechs
 * @brief fux parser header
 * @version 0.1
 * @date 2022-11-02
 * 
 * @copyright Copyright (c) 2020-2022, Fuechs. All rights reserved.
 * 
 */

#pragma once

#include "../../fux.hpp"
#include "ast.hpp"
#include "../error/error.hpp"
#include "../lexer/lexer.hpp"
#include "../lexer/token.hpp"
#include "../../util/threading.hpp"

class Parser {
public:
    Parser(ErrorManager *error, const string &fileName, const string &source, const bool mainFile = false) 
    : error(error), mainFile(mainFile) {
        lexer = new Lexer(source, fileName, error);
        if (mainFile) {
            fux.options.fileLines = lexer->getLines();
            threader = new fuxThread::ThreadManager();
        }
        root = new RootAST();
    }

    ~Parser() {
        delete lexer;
        delete threader;
        tokens.clear();
    }

    // parse the Tokens and return AST root
    RootAST *parse();

private:
    TokenList tokens;
    TokenIter current;
    ErrorManager *error;
    Lexer *lexer;
    fuxThread::ThreadManager *threader;
    RootAST *root;
    const bool mainFile;

    ExprPtr parseStmt();
    ExprPtr parseExpr();
    // <symbol> = <value>;
    ExprPtr parseAssignmentExpr();
    ExprPtr parseMemberExpr();
    ExprPtr parseCallExpr();
    ExprPtr parseLogicalExpr();
    ExprPtr parseComparisonExpr();
    // +, -
    ExprPtr parseAdditiveExpr();
    // *, /, %
    ExprPtr parseMultiplicativeExpr();
    ExprPtr parseUnaryExpr();
    // identifier, value, sub expr
    // x         , 1    , (...)
    ExprPtr parsePrimaryExpr();
    
    // get next token
    Token eat();
    // expect an token
    Token expect(TokenType type, ErrorType errType = UNEXPECTED_TOKEN);
    // check wether end of file is reached
    bool notEOF();
};