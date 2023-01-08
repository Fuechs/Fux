/**
 * @file analyser.cpp
 * @author fuechs
 * @brief fux ast analyser
 * @version 0.1
 * @date 2023-01-08
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs. All rights reserved.
 * 
 */

#include "analyser.hpp"

ExprPtr Analyser::analyse() { 
    return move(origin->analyse());
}

ExprPtr RootAST::analyse() { return move(nullExpr); }
ExprPtr NumberExprAST::analyse() { return move(nullExpr); }
ExprPtr VariableExprAST::analyse() { return move(nullExpr); }
ExprPtr BinaryExprAST::analyse() { return move(nullExpr); }
ExprPtr ComparisonExprAST::analyse() { return move(nullExpr); }
ExprPtr LogicalExprAST::analyse() { return move(nullExpr); }
ExprPtr CallExprAST::analyse() { return move(nullExpr); }
ExprPtr VariableDeclAST::analyse() { return move(nullExpr); }
ExprPtr PutsCallAST::analyse() { return move(nullExpr); }
ExprPtr PrototypeAST::analyse() { return move(nullExpr); }
ExprPtr FunctionAST::analyse() { return move(nullExpr); }