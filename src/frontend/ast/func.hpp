/**
 * @file func.hpp
 * @author fuechs
 * @brief Function Declaration Statement AST Header
 * @version 0.1
 * @date 2023-04-09
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#pragma once

#include "../../fux.hpp"
#include "expr.hpp"

class FunctionStmt : public Stmt {
public:
    using Ptr = shared_ptr<FunctionStmt>;

    struct Parameter {
        using Ptr = shared_ptr<Parameter>;
        using Vec = vector<Ptr>;

        Parameter(Metadata meta = Metadata(), string symbol = "", FuxType type = FuxType::NO_TYPE, 
            Expr::Ptr value = nullptr, bool variadic = false);
        ~Parameter();

        Parameter &operator=(const Parameter &copy);

        string symbol;      
        FuxType type;
        Expr::Ptr value;    // default value if no value was passed in call
        bool variadic;      // variadic argument
                            // e.g.: func(args: var[], ...): void;
                            //      func(1, 2, 3);
                            //      func("er", 3, 214.2, 'a', false);
        Metadata meta;
    };

    FunctionStmt(Metadata meta = Metadata(), string symbol = "", FuxType type = FuxType::NO_TYPE, 
        Parameter::Vec parameters = {}, Stmt::Ptr body = nullptr, Stmt::Vec locals = {});
    ~FunctionStmt();

    FUX_BC(Eisdrache::Local &codegen(Eisdrache *eisdrache) override;)
    FUX_AC(Stmt::Ptr analyse(Analyser *analyser) override;)

    AST getASTType() override;
    FuxType getFuxType() override;
    string getSymbol() override;
    void debugPrint(size_t indent = 0) override;

    string symbol;              
    FuxType type;               
    Parameter::Vec parameters;  
    Stmt::Ptr body;             
    Stmt::Vec locals;   // any variable that was declared in the body of this function;
                        // when generating the IR, every local will be allocated first;
                        // the parser just leaves a reference or an assignment behind 
                        //                         (SymbolExpr)    (BinaryExpr)
};