/**
 * @file parser.cpp
 * @author fuechs
 * @brief fux parser 
 * @version 0.1
 * @date 2022-09-27
 * 
 * @copyright Copyright (c) 2020-2022, Fuechs. All rights reserved.
 * 
 */

#include "parser.hpp"

namespace fux {

    unique_ptr<ExprAST> Parser::parse() {
        // 7 + 3 * 4
        // (+ 7 (* 3 4))
        auto LHS = make_unique<NumberExprAST>(1);
        auto RHS = make_unique<NumberExprAST>(2);
        root = make_unique<BinaryExprAST>('+', move(LHS), move(RHS)); 
        return NULL; // FIXME: deleted function?
    }

}