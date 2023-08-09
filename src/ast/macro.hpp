/**
 * @file macro.hpp
 * @author fuechs
 * @brief Macro Declaration Statement AST Header
 * @version 0.1
 * @date 2023-04-14
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#pragma once

#include "../fux.hpp"
#include "expr.hpp"

class MacroStmt : public Stmt {
public:
    enum ID {
        TYPE,
        IDENT,
        EXPR,
        STMT,
        BLOCK,
        WILDCARD, // "all other cases"
        NONE,
    };

    struct Arg {
        using Vec = vector<Arg>;

        Arg(Metadata meta = Metadata(), string symbol = "", ID type = NONE, bool array = false, bool variadic = false);

        Arg &operator=(const Arg &copy);

        string symbol;
        ID type;
        bool array, variadic;
        Metadata meta;
    };

    struct Case {
        using Vec = vector<Case *>;

        Case(Arg::Vec args, Stmt::Ptr &ret);

        Arg::Vec args;
        Stmt::Ptr ret;
        Metadata meta;
    };

    MacroStmt(const string &symbol, Case::Vec cases = {});

    FUX_BC(Eisdrache::Local &codegen(Eisdrache *eisdrache) override;)
    FUX_AC(Stmt::Ptr analyse(Analyser *analyser) override;)
    AST getASTType() override;
    string getSymbol() override;
    void debugPrint(size_t indent = 0) override;

private:
    string symbol;
    Case::Vec cases;    
};