/**
 * @file analyser.cpp
 * @author fuechs
 * @brief fux ast analyser
 * @version 0.1
 * @date 2023-01-08
 * 
 * @copyright Copyright (c) 2020-2023, Fux programming language and authors. All rights reserved.
 * 
 */

#include "analyser.hpp"

ExprPtr Analyser::analyse() { 
    return std::move(origin->analyse());
}

ExprPtr RootAST::analyse() { return std::move(nullExpr); }
ExprPtr NumberExprAST::analyse() { return std::move(nullExpr); }
ExprPtr VariableExprAST::analyse() { return std::move(nullExpr); }
ExprPtr BinaryExprAST::analyse() { return std::move(nullExpr); }
ExprPtr ComparisonExprAST::analyse() { return std::move(nullExpr); }
ExprPtr LogicalExprAST::analyse() { return std::move(nullExpr); }
ExprPtr CallExprAST::analyse() { return std::move(nullExpr); }
ExprPtr VariableDeclAST::analyse() { return std::move(nullExpr); }
ExprPtr PutsCallAST::analyse() { return std::move(nullExpr); }
ExprPtr IfElseAST::analyse() { return std::move(nullExpr); }
ExprPtr PrototypeAST::analyse() { return std::move(nullExpr); }
ExprPtr FunctionAST::analyse() { return std::move(nullExpr); }