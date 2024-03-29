/**
 * @file ast.hpp
 * @author fuechs
 * @brief fux abstract syntax tree header
 * @version 0.1
 * @date 2022-10-31
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#pragma once

#include "../../fux.hpp"
#include "../../backend/llvmheader.hpp"
#include "../lexer/token.hpp"
#include "../parser/type.hpp"
#include "../parser/value.hpp"
#include "expr.hpp"
#include "op.hpp"

typedef map<string, FuxType> ArgMap;

/// EXPRESSIONS ///

class VariadicExprAST : public ExprAST {
public:
    FUX_BC(Eisdrache::Local &codegen(Eisdrache *eisdrache) override;)
    StmtAST::Ptr analyse(Analyser *analyser) override;
    AST getASTType() override;
    FuxType getFuxType() override;
    void debugPrint(size_t indent = 0) override;
};

class NullExprAST : public ExprAST {
public:    
    FUX_BC(Eisdrache::Local &codegen(Eisdrache *eisdrache) override;)
    StmtAST::Ptr analyse(Analyser *analyser) override;
    AST getASTType() override;
    FuxType getFuxType() override;
    void debugPrint(size_t indent = 0) override;    
};

class BoolExprAST : public ExprAST {
    ValueStruct *value;

public:
    BoolExprAST(bool value) : value(new ValueStruct(value)) {}
    ~BoolExprAST();

    FUX_BC(Eisdrache::Local &codegen(Eisdrache *eisdrache) override;)
    StmtAST::Ptr analyse(Analyser *analyser) override;
    AST getASTType() override;
    FuxType getFuxType() override;
    void debugPrint(size_t indent = 0) override;
};

class NumberExprAST : public ExprAST {
    ValueStruct *value;

public:
    template<typename T>
    NumberExprAST(T value) : value(new ValueStruct(value)) {}
    ~NumberExprAST();

    FUX_BC(Eisdrache::Local &codegen(Eisdrache *eisdrache) override;)
    StmtAST::Ptr analyse(Analyser *analyser) override;
    AST getASTType() override;
    FuxType getFuxType() override;
    void debugPrint(size_t indent = 0) override;
};

class CharExprAST : public ExprAST {
    ValueStruct *value;

public:
    template<typename T>
    CharExprAST(T value) : value(new ValueStruct(value)) {}
    ~CharExprAST();

    FUX_BC(Eisdrache::Local &codegen(Eisdrache *eisdrache) override;)
    StmtAST::Ptr analyse(Analyser *analyser) override;
    AST getASTType() override;
    FuxType getFuxType() override;
    void debugPrint(size_t indent = 0) override;
};

class StringExprAST : public ExprAST {
    ValueStruct *value;

public:
    StringExprAST(string value) : value(new ValueStruct(value)) {}
    ~StringExprAST();

    FUX_BC(Eisdrache::Local &codegen(Eisdrache *eisdrache) override;)
    StmtAST::Ptr analyse(Analyser *analyser) override; 
    AST getASTType() override;
    FuxType getFuxType() override;
    void debugPrint(size_t indent = 0) override;
};

class RangeExprAST : public ExprAST {
    ExprAST::Ptr begin; 
    ExprAST::Ptr end; 

public:
    RangeExprAST(ExprAST::Ptr &begin, ExprAST::Ptr &end) 
    : begin(std::move(begin)), end(std::move(end)) {}

    FUX_BC(Eisdrache::Local &codegen(Eisdrache *eisdrache) override;)
    StmtAST::Ptr analyse(Analyser *analyser) override;  
    AST getASTType() override;
    FuxType getFuxType() override;
    void debugPrint(size_t indent = 0) override;
};

class ArrayExprAST : public ExprAST {
    ExprAST::Vec elements;
    
public:
    ArrayExprAST(ExprAST::Vec &elements) : elements(std::move(elements)) {}

    FUX_BC(Eisdrache::Local &codegen(Eisdrache *eisdrache) override;)
    StmtAST::Ptr analyse(Analyser *analyser) override; 
    AST getASTType() override;
    FuxType getFuxType() override;
    void debugPrint(size_t indent = 0) override;
};

class VariableExprAST : public ExprAST {
    string name;

public:
    VariableExprAST(const string& name) : name(name) {}
    ~VariableExprAST() override;

    FUX_BC(Eisdrache::Local &codegen(Eisdrache *eisdrache) override;)
    StmtAST::Ptr analyse(Analyser *analyser) override;  
    AST getASTType() override;
    FuxType getFuxType() override;
    string getSymbol() override;
    void debugPrint(size_t indent = 0) override;
};

class CallExprAST : public ExprAST {
    ExprAST::Ptr callee;
    ExprAST::Vec args;
    bool asyncCall;

public:
    // parser has to insert end position!
    CallExprAST(const string &callee, ExprAST::Vec &args, bool asyncCall = false);
    [[deprecated("Is not able to copy the position of the callee.")]]
    CallExprAST(ExprAST::Ptr &callee, ExprAST::Vec &args, bool asyncCall = false);

    FUX_BC(Eisdrache::Local &codegen(Eisdrache *eisdrache) override;)
    StmtAST::Ptr analyse(Analyser *analyser) override; 
    AST getASTType() override;
    FuxType getFuxType() override;
    string getSymbol() override;
    void debugPrint(size_t indent = 0) override;
};

class MemberExprAST : public ExprAST {
    ExprAST::Ptr parent;
    string member;

public:
    MemberExprAST(ExprAST::Ptr &parent, const Token &member);
    [[deprecated("Is not able to copy the position of the member.")]]
    MemberExprAST(ExprAST::Ptr &parent, const string &member);

    FUX_BC(Eisdrache::Local &codegen(Eisdrache *eisdrache) override;)
    StmtAST::Ptr analyse(Analyser *analyser) override;    
    AST getASTType() override;
    FuxType getFuxType() override;
    string getSymbol() override;
    void debugPrint(size_t indent = 0) override;
};

class UnaryExprAST : public ExprAST {
    UnaryOp op;
    ExprAST::Ptr expr;

public:
    UnaryExprAST(const Token &op, ExprAST::Ptr &expr, bool postOp = false);
    [[deprecated("Is not able to copy the position of the operator.")]]
    UnaryExprAST(UnaryOp op, ExprAST::Ptr &expr);

    FUX_BC(Eisdrache::Local &codegen(Eisdrache *eisdrache) override;)
    StmtAST::Ptr analyse(Analyser *analyser) override;  
    AST getASTType() override;
    FuxType getFuxType() override;
    void debugPrint(size_t indent = 0) override;
};

class BinaryExprAST : public ExprAST {
    BinaryOp op;
    ExprAST::Ptr LHS, RHS;

public:
    BinaryExprAST(BinaryOp op, ExprAST::Ptr &LHS, ExprAST::Ptr &RHS = nullExpr);

    FUX_BC(Eisdrache::Local &codegen(Eisdrache *eisdrache) override;)
    StmtAST::Ptr analyse(Analyser *analyser) override;   
    AST getASTType() override;
    FuxType getFuxType() override;
    void debugPrint(size_t indent = 0) override;
};

class TypeCastExprAST : public ExprAST {
    FuxType type;
    ExprAST::Ptr expr;

public:
    TypeCastExprAST(FuxType type, ExprAST::Ptr &expr);
    
    FUX_BC(Eisdrache::Local &codegen(Eisdrache *eisdrache) override;)
    StmtAST::Ptr analyse(Analyser *analyser) override;
    AST getASTType() override;
    FuxType getFuxType() override;
    void debugPrint(size_t indent = 0) override;
};

class TernaryExprAST : public ExprAST {
    ExprAST::Ptr condition;
    ExprAST::Ptr thenExpr;
    ExprAST::Ptr elseExpr;

public:
    TernaryExprAST(ExprAST::Ptr &condition, ExprAST::Ptr &thenExpr, ExprAST::Ptr &elseExpr);

    FUX_BC(Eisdrache::Local &codegen(Eisdrache *eisdrache) override;)
    StmtAST::Ptr analyse(Analyser *analyser) override; 
    AST getASTType() override;
    FuxType getFuxType() override;
    void debugPrint(size_t indent = 0) override;
};

/// STATEMENTS ///

class NoOperationAST : public StmtAST {
public:
    FUX_BC(Eisdrache::Local &codegen(Eisdrache *eisdrache) override;)
    StmtAST::Ptr analyse(Analyser *analyser) override;
    AST getASTType() override;
    FuxType getFuxType() override;
    void debugPrint(size_t indent = 0) override;    
};

class VariableDeclAST : public StmtAST {
    string symbol;
    FuxType type;
    ExprAST::Ptr value;

public:
    using Ptr = unique_ptr<VariableDeclAST>;

    VariableDeclAST(string symbol, FuxType type = FuxType(), ExprAST::Ptr &value = nullExpr) 
    : symbol(symbol), type(type), value(std::move(value)) {}
    ~VariableDeclAST() override;
    
    FuxType &getType();
    ExprAST::Ptr &getValue();

    FUX_BC(Eisdrache::Local &codegen(Eisdrache *eisdrache) override;)
    StmtAST::Ptr analyse(Analyser *analyser) override;
    AST getASTType() override;
    FuxType getFuxType() override;
    string getSymbol() override;
    void debugPrint(size_t indent = 0) override;
};

typedef unique_ptr<VariableDeclAST> VarDeclPtr; 

class InbuiltCallAST : public StmtAST {
    Inbuilts callee;
    ExprAST::Vec arguments;

public:
    InbuiltCallAST(Inbuilts callee, ExprAST::Vec &arguments) 
    : callee(callee), arguments(std::move(arguments)) {}

    FUX_BC(Eisdrache::Local &codegen(Eisdrache *eisdrache) override;)
    StmtAST::Ptr analyse(Analyser *analyser) override;
    AST getASTType() override;
    FuxType getFuxType() override;
    void debugPrint(size_t indent = 0) override;
};

class IfElseAST : public StmtAST {
    ExprAST::Ptr condition;
    StmtAST::Ptr thenBody;
    StmtAST::Ptr elseBody;

public:
    IfElseAST(ExprAST::Ptr &condition, StmtAST::Ptr &thenBody, StmtAST::Ptr &elseBody = nullStmt)
    : condition(std::move(condition)), thenBody(std::move(thenBody)), elseBody(std::move(elseBody)) {}

    FUX_BC(Eisdrache::Local &codegen(Eisdrache *eisdrache) override;)
    StmtAST::Ptr analyse(Analyser *analyser) override;
    AST getASTType() override;
    FuxType getFuxType() override;
    void debugPrint(size_t indent = 0) override;
};

class CodeBlockAST : public StmtAST {
    StmtAST::Vec body;

public:
    CodeBlockAST() : body(StmtAST::Vec()) {}
    CodeBlockAST(StmtAST::Vec &body) : body(std::move(body)) {}

    FUX_BC(Eisdrache::Local &codegen(Eisdrache *eisdrache) override;)
    StmtAST::Ptr analyse(Analyser *analyser) override;
    AST getASTType() override;
    FuxType getFuxType() override;
    void debugPrint(size_t indent = 0) override;
 
    void addSub(StmtAST::Ptr &sub);
};

class WhileLoopAST : public StmtAST {
    ExprAST::Ptr condition;
    StmtAST::Ptr body;
    bool postCondition;

public:
    WhileLoopAST(ExprAST::Ptr &condition, StmtAST::Ptr &body, bool postCondition = false)
    : condition(std::move(condition)), body(std::move(body)), postCondition(postCondition) {}

    FUX_BC(Eisdrache::Local &codegen(Eisdrache *eisdrache) override;)
    StmtAST::Ptr analyse(Analyser *analyser) override;
    AST getASTType() override;
    FuxType getFuxType() override;
    void debugPrint(size_t indent = 0) override;
};

class ForLoopAST : public StmtAST {
    bool forEach;
    /*for (*/StmtAST::Ptr initial; 
            ExprAST::Ptr condition; 
            ExprAST::Ptr iterator;//) {
        StmtAST::Ptr body;
    // }
public:
    ForLoopAST(StmtAST::Ptr &initial, ExprAST::Ptr &iterator, StmtAST::Ptr &body)
    : forEach(true), initial(std::move(initial)), condition(nullptr), 
        iterator(std::move(iterator)), body(std::move(body)) {}
    ForLoopAST(StmtAST::Ptr &initial, ExprAST::Ptr &condition, ExprAST::Ptr &iterator, StmtAST::Ptr &body)
    : forEach(false), initial(std::move(initial)), condition(std::move(condition)),
        iterator(std::move(iterator)), body(std::move(body)) {}

    FUX_BC(Eisdrache::Local &codegen(Eisdrache *eisdrache) override;)
    StmtAST::Ptr analyse(Analyser *analyser) override;
    AST getASTType() override;
    FuxType getFuxType() override;
    void debugPrint(size_t indent = 0) override;
};

typedef unique_ptr<CodeBlockAST> BlockPtr;

// prototype of a function
// name and arguments
class PrototypeAST : public StmtAST {
    FuxType type;
    string symbol;
    StmtAST::Vec args;

public:
    typedef unique_ptr<PrototypeAST> Ptr;

    PrototypeAST(FuxType type, const string &symbol, StmtAST::Vec &args)
    : type(type), symbol(symbol), args(std::move(args)) {}
    ~PrototypeAST() override;
    
    FUX_BC(Eisdrache::Local &codegen(Eisdrache *eisdrache) override;)
    StmtAST::Ptr analyse(Analyser *analyser) override;
    AST getASTType() override;
    FuxType getFuxType() override;
    string getSymbol() override;
    void debugPrint(size_t indent = 0) override;
    
    StmtAST::Vec &getArgs();
};

class FunctionAST : public StmtAST {
    PrototypeAST::Ptr proto;
    StmtAST::Ptr body;
    StmtAST::Vec locals; // local variables that are declared in this function

public:
    typedef unique_ptr<FunctionAST> Ptr;

    FunctionAST(FuxType type, const string &symbol, StmtAST::Vec &args)
    : proto(make_unique<PrototypeAST>(type, symbol, args)), body(nullptr), locals(StmtAST::Vec()) {}
    FunctionAST(PrototypeAST::Ptr &proto, StmtAST::Ptr &body)
    : proto(std::move(proto)), body(std::move(body)) {}

    FUX_BC(Eisdrache::Local &codegen(Eisdrache *eisdrache) override;)
    StmtAST::Ptr analyse(Analyser *analyser) override;
    AST getASTType() override;
    FuxType getFuxType() override;
    string getSymbol() override;
    void debugPrint(size_t indent = 0) override;    

    void setBody(StmtAST::Ptr &body);
    void addLocal(StmtAST::Ptr &local);
};

class EnumerationAST : public StmtAST {
    string symbol;
    vector<string> elements;

public:
    EnumerationAST(const string &symbol, vector<string> elements = {})
    : symbol(symbol), elements(elements) {}

    FUX_BC(Eisdrache::Local &codegen(Eisdrache *eisdrache) override;)
    StmtAST::Ptr analyse(Analyser *analyser) override;
    AST getASTType() override;
    FuxType getFuxType() override;
    string getSymbol() override;
    void debugPrint(size_t indent = 0) override; 
};

class MacroAST : public StmtAST {
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

        Arg(string symbol = "", ID type = NONE, bool array = false, bool variadic = false) 
        : symbol(symbol), type(type), array(array), variadic(variadic) {}

        Arg &operator=(const Arg &copy);

        string symbol;
        ID type;
        bool array, variadic;
        Metadata meta;
    };

    struct Case {
        using Vec = vector<Case *>;

        Case(Arg::Vec args, StmtAST::Ptr &ret) : args(args), ret(std::move(ret)) {}

        Arg::Vec args;
        StmtAST::Ptr ret;
        Metadata meta;
    };

    MacroAST(const string &symbol, Case::Vec cases = {})
    : symbol(symbol), cases(cases) {}

    FUX_BC(Eisdrache::Local &codegen(Eisdrache *eisdrache) override;)
    StmtAST::Ptr analyse(Analyser *analyser) override;
    AST getASTType() override;
    FuxType getFuxType() override;
    string getSymbol() override;
    void debugPrint(size_t indent = 0) override;

private:
    string symbol;
    Case::Vec cases;    
};

class RootAST : public StmtAST {
    StmtAST::Vec program;
    // resting place for array size expressions
    // FuxTypes refer to these by IDs
    ExprAST::Vec arraySizeExprs; 

public:
    typedef unique_ptr<RootAST> Ptr;
    typedef vector<Ptr> Vec;

    RootAST() : program(StmtAST::Vec()) {}        
    
    FUX_BC(Eisdrache::Local &codegen(Eisdrache *eisdrache) override;)
    StmtAST::Ptr analyse(Analyser *analyser) override;
    AST getASTType() override;
    FuxType getFuxType() override;
    string getSymbol() override;
    void debugPrint(size_t indent = 0) override;
 
    void addSub(StmtAST::Ptr &sub);
    _i64 addSizeExpr(ExprAST::Ptr &sizeExpr);
};