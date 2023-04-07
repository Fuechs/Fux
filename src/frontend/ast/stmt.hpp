/**
 * @file stmt.hpp
 * @author fuechs
 * @brief statement AST
 * @version 0.1
 * @date 2023-04-07
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#pragma once

#include "../../fux.hpp"
#include "../symbol.hpp"
#include "../type.hpp"

class Debug { public: using Ptr = std::shared_ptr<Debug>; };
class Analyser { public: using Ptr = std::shared_ptr<Analyser>; };
class Generator { public: using Ptr = std::shared_ptr<Generator>; };

enum class AST {
    Stmt,
    Expr,
};

class Stmt {
public:
    using Ptr = std::unique_ptr<Stmt>;
    using Vec = std::vector<Ptr>;

    virtual Ptr analyse(Analyser::Ptr analyser);
    virtual Sym::Ptr codegen(Generator::Ptr generator);

    virtual void debugPrint(Debug::Ptr debug);
    virtual std::string getRawSym();
    virtual Type::Ptr getType();
    virtual AST getASTType() = 0;

    bool isExpr();
};