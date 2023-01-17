/**
 * @file stmt.hpp
 * @author fuechs
 * @brief stmt ast class header
 * @version 0.1
 * @date 2023-01-15
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#pragma once

class StmtAST {
public:
    virtual ~StmtAST() {}
    virtual Value *codegen(LLVMWrapper *fuxLLVM) = 0;
    virtual unique_ptr<StmtAST> analyse() = 0;
    virtual void debugPrint() = 0;

    Position pos = Position();
};

typedef unique_ptr<StmtAST> StmtPtr;
typedef vector<StmtPtr> StmtList;

extern StmtPtr nullStmt;