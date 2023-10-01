/**
 * @file eisdrache.hpp
 * @author fuechs
 * @brief Eisdrache class header
 * @version 0.3
 * @date 2023-01-30
 * 
 * @copyright Copyright (c) 2023, Fuechs.
 * 
 */

/*  
    MIT License

    Copyright (c) 2023, Fuechs.

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE. 
*/

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include <llvm/PassRegistry.h>
#include <llvm/InitializePasses.h>
// #include <llvm/ADT/Triple.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IR/Instructions.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>


namespace llvm {

/**
 * @brief Eisdrache Wrapper for the LLVM API.
 * 
 * This wrapper offers a simplified version of the llvm::IRBuilder to avoid
 * unnecessary code.
 */
class Eisdrache : public std::enable_shared_from_this<Eisdrache> {
public:
    using Ptr = std::shared_ptr<Eisdrache>;
    using ValueVec = std::vector<Value *>;
    using TypeVec = std::vector<Type *>;

    // Binary and Unary Operations
    enum Op {
        ADD,    // addition             +
        SUB,    // subtraction          -
        MUL,    // multiplication       *
        DIV,    // division             /
        MOD,    // modulus / remainder  %
        OR,     // bit or               |
        XOR,    // bit xor              ^
        AND,    // bit and              &
        LSH,    // left bit shift       <<
        RSH,    // right bit shift      >>

        EQU,    // equals               ==
        NEQ,    // not equals           !=
        LES,    // less than            <
        LTE,    // less than equals     <=   
        GRE,    // greater than         >
        GTE,    // greater than equals  >=   

        NEG,    // negate               -
        NOT,    // bit not              ~ 
    };

    /**
     * @brief Parent class for references, locals, functions, ...
     * 
     */
    class Entity {
    public:
        using Vec = std::vector<Entity>;

        Entity(Eisdrache::Ptr eisdrache = nullptr);
        virtual ~Entity();

        enum Kind {
            REFERENCE,
            LOCAL,
            FUNC,
            ALIAS,
            VOID,
            PTR,
            INT,
            FLOAT,
            STRUCT,
            NONE,
        };

        virtual Kind kind() const = 0;

    protected:
        Eisdrache::Ptr eisdrache;
    };

    /**
     * @brief Custom Type Parent Class;
     */
    class Ty : public Entity, public std::enable_shared_from_this<Ty> {
    public:
        using Ptr = std::shared_ptr<Ty>;
        using Vec = std::vector<Ptr>;
        using OldMap = std::map<std::string, Ptr>;
        using Map = std::vector<std::pair<std::string, Ptr>>;

        Ty(Eisdrache::Ptr eisdrache = nullptr);

        static Ptr create(Eisdrache::Ptr eisdrache, Type *llvmTy);

        Ptr getPtrTo();
        virtual size_t getBit() const;

        // get the equivalent llvm::Type 
        virtual Type *getTy() const = 0;

        virtual bool isValidRHS(const Ptr comp) const = 0;
        virtual bool isEqual(const Ptr comp) const = 0;

        constexpr bool isPtrTy() const;
        constexpr bool isIntTy() const;
        constexpr bool isFloatTy() const;
        constexpr bool isSignedTy();

        virtual Kind kind() const = 0;
    };

    /**
     * @brief An alias for a type.
     * 
     */
    class AliasTy : public Ty {
    public:
        using Ptr = std::shared_ptr<AliasTy>;
        using Vec = std::vector<AliasTy>;

        AliasTy(Eisdrache::Ptr eisdrache, std::string alias, Ty::Ptr type);
        ~AliasTy() override;

        size_t getBit() const override;

        Type *getTy() const override;

        bool isValidRHS(const Ty::Ptr comp) const override;
        bool isEqual(const Ty::Ptr comp) const override;

        Kind kind() const override;

    private:
        std::string alias;
        Ty::Ptr type;
    };

    /**
     * @brief Type representing void.
     * 
     */
    class VoidTy : public Ty {
    public:
        using Ptr = std::shared_ptr<VoidTy>;
        using Vec = std::vector<Ptr>;

        VoidTy(Eisdrache::Ptr eisdrache = nullptr);

        Type *getTy() const override;

        bool isValidRHS(const Ty::Ptr comp) const override;
        bool isEqual(const Ty::Ptr comp) const override;

        Kind kind() const override;
    };

    /**
     * @brief Type representing a pointer to an adress.
     * 
     */
    class PtrTy : public Ty {
    public:
        using Ptr = std::shared_ptr<PtrTy>;
        using Vec = std::vector<Ptr>;

        PtrTy(Eisdrache::Ptr eisdrache, Ty::Ptr pointee);

        Ty::Ptr &getPointeeTy();
        
        size_t getBit() const override;

        Type *getTy() const override;

        bool isValidRHS(const Ty::Ptr comp) const override;
        bool isEqual(const Ty::Ptr comp) const override;

        Kind kind() const override;

    private:
        Ty::Ptr pointee;
    };

    class IntTy : public Ty {
    public:
        using Ptr = std::shared_ptr<IntTy>;
        using Vec = std::vector<Ptr>;

        IntTy(Eisdrache::Ptr eisdrache, size_t bit, bool _signed = false);
        
        size_t getBit() const override;

        const bool &getSigned() const;
        Ty::Ptr getSignedTy() const;

        Type *getTy() const override;

        bool isValidRHS(const Ty::Ptr comp) const override;
        bool isEqual(const Ty::Ptr comp) const override;

        Kind kind() const override;

    private:
        size_t bit;
        bool _signed;
    };

    class FloatTy : public Ty {
    public:
        using Ptr = std::shared_ptr<FloatTy>;
        using Vec = std::vector<Ptr>;

        FloatTy(Eisdrache::Ptr eisdrache, size_t bit);

        size_t getBit() const override;

        Type *getTy() const override;

        bool isValidRHS(const Ty::Ptr comp) const override;
        bool isEqual(const Ty::Ptr comp) const override;

        Kind kind() const override;
    
    private:
        size_t bit;
    };

    /**
     * @brief A reference to a symbol (local, function, ...).
     * 
     */
    class Reference : public Entity {
    public:
        using Vec = std::vector<Reference>;

        Reference(Eisdrache::Ptr eisdrache = nullptr, std::string symbol = "");
        ~Reference() override;

        Reference &operator=(const Reference &copy);

        const std::string &getSymbol() const;

        // get entity that is referred to
        Entity &getEntity() const;

        Kind kind() const override;

    private:
        std::string symbol;
        Eisdrache::Ptr eisdrache;
    };

    /**
     * @brief Wrapper for llvm::Value | llvm::AllocaInst;
     * 
     * This class contains 
     * * the value itself,
     * * wether the value is a llvm::AllocaInst,
     * * and the value to be assigned once the value is referenced. 
     * (Relevant for llvm::AllocaInst)
     */
    class Local : public Entity {
    public:
        using Vec = std::vector<Local>;
        using Map = std::map<std::string, Local>;

        Local(Eisdrache::Ptr eisdrache, Constant *constant);
        Local(Eisdrache::Ptr eisdrache = nullptr, Ty::Ptr type = nullptr, Value *ptr = nullptr, Value *future = nullptr, ValueVec future_args = ValueVec());
        
        Local &operator=(const Local &copy);
        bool operator==(const Local &comp) const;
        bool operator==(const Value *comp) const;
        AllocaInst *operator*();

        void setPtr(Value *ptr);
        void setFuture(Value *future);
        void setFutureArgs(ValueVec args);
        void setTy(Ty::Ptr ty);

        AllocaInst *getAllocaPtr();
        Value *getValuePtr();
        Ty::Ptr getTy();
        std::string getName() const;

        bool isAlloca();

        /**
         * @brief Load the value stored at the adress of the local.
         * 
         * @param force Load value even if Local is not an alloca instruction
         * @param name Name of the loaded value
         * @return Local & 
         */
        Local &loadValue(bool force = false, std::string name = "");
        /**
         * @brief This function should be called automatically when trying to access `v_ptr` or `a_ptr`.
         *      However, the user can call this function themselves if required.
         *      This function also checks wether `future` is a nullptr 
         *          and sets `future` to nullptr once it was invoked. 
         *      (`future_args` too)
         */
        void invokeFuture();
        
        Kind kind() const override;

    private:
        union {
            Value *v_ptr;
            AllocaInst *a_ptr;
        };
        Ty::Ptr type;
        Value *future;
        ValueVec future_args;
        Eisdrache::Ptr eisdrache;
    };

    /**
     * @brief Wrapper for llvm::Function.
     * 
     * This class contains all locals, parameters and blocks 
     * from the wrapped llvm::Function. Can be initialized by the user, 
     * but should be initialized by the Eisdrache Wrapper.
     * 
     * @example
     * Eisdrache::Func &main = eisdrache->declareFunction(eisdrache->getIntTy(), "main",
     *      {{"argc", eisdrache->getSizeTy()}, {"argv", eisdrache->getIntPtrPtrTy(8)}});
     */
    class Func : public Entity {
    public:
        using Vec = std::vector<Func>;
        using Map = std::map<std::string, Func>;

        Func();
        Func(Eisdrache::Ptr eisdrache, Ty::Ptr type, std::string name, Ty::Map parameters, bool entry = false);
        ~Func();

        Func &operator=(const Func &copy);
        bool operator==(const Func &comp) const;
        bool operator==(const Function *comp) const;
        // update reference local by symbol
        Local &operator[](std::string symbol);
        // get the wrapped llvm::Function
        Function *operator*();

        // get argument at index
        Local &arg(size_t index);
        // call this function
        Local &call(ValueVec args = {}, std::string name = "");
        // add a local variable to this function
        // and return reference to copy of local
        Local &addLocal(Local local);

        Ty::Ptr getTy();

        Kind kind() const override;

    private:
        Function *func;
        Ty::Ptr type;
        Local::Vec parameters;
        Local::Map locals;

        Eisdrache::Ptr eisdrache;
    };

    /**
     * @brief Wrapper for llvm::StructType
     * 
     * This class contains the struct type itself and all types of its elements. 
     * Can be initialized by the user, but should be initialized by the Eisdrache Wrapper.
     * 
     * @example
     * Eisdrache::Struct &array = eisdrache->declareStruct("array", {eisdrache->getIntPtrTy(), eisdrache->getSizeTy()});
     */
    class Struct : public Ty {
    public:
        using Ptr = std::shared_ptr<Struct>;
        using Vec = std::vector<Ptr>;
        using Map = std::map<std::string, Ptr>;

        Struct();
        Struct(Eisdrache::Ptr eisdrache, std::string name, Ty::Vec elements);
        ~Struct();

        Struct &operator=(const Struct &copy);
        bool operator==(const Struct &comp) const;
        bool operator==(const Type *comp) const;
        // get type of an element at an index
        Ty::Ptr operator[](size_t index);
        // get the wrapped llvm::StructType
        StructType *operator*();
        
        // allocate object of this type
        Local &allocate(std::string name = "");

        /**
         * @brief Create a member function of this struct.
         *      Automatically adds `ptr this` as first parameter.
         * 
         * @param type Type of returned value
         * @param name Function name
         * @param args Additional parameters
         * @return Func * 
         */
        Func *createMemberFunc(Ty::Ptr type, std::string name, Ty::Map args = Ty::Map());

        Type *getTy() const override;

        bool isValidRHS(const Ty::Ptr comp) const override;
        bool isEqual(const Ty::Ptr comp) const override;

        Kind kind() const override;

    private:
        std::string name;
        StructType *type;
        Ty::Vec elements;
    };

    class Array {
    public:
        enum Member {
            GET_BUFFER,
            SET_BUFFER,
            GET_SIZE,
            SET_SIZE,
            GET_MAX,
            SET_MAX,
            GET_FACTOR,
            SET_FACTOR,
            CONSTRUCTOR,
            CONSTRUCTOR_SIZE,
            DESTRUCTOR,
            RESIZE,
        };

        Array(Eisdrache::Ptr eisdrache = nullptr, Ty::Ptr elementTy = nullptr, std::string name = "");
        ~Array();

        Local &allocate(std::string name = "");
        Local &call(Member callee, ValueVec args = {}, std::string name = "");

    private:
        std::string name;
        Struct::Ptr self;
        Ty::Ptr elementTy;
        Ty::Ptr bufferTy;
        
        Func *get_buffer = nullptr;
        Func *set_buffer = nullptr;
        Func *get_size = nullptr;
        Func *set_size = nullptr;
        Func *get_max = nullptr;
        Func *set_max = nullptr;
        Func *get_factor = nullptr;
        Func *set_factor = nullptr;
        Func *constructor = nullptr;
        Func *constructor_size = nullptr;
        Func *destructor = nullptr;
        Func *resize = nullptr;

        Eisdrache::Ptr eisdrache;
    };

    ~Eisdrache();

    // Initialize the LLVM API
    static void initialize();

    static Ptr create(std::string moduleID, std::string targetTriple = "");

    // dump the generated LLVM IR
    void dump(raw_fd_ostream &os = errs());

    /// TYPES //

    // Type: void
    Ty::Ptr getVoidTy();
    // Type: i1
    Ty::Ptr getBoolTy();
    // Type: i64 (size_t)
    Ty::Ptr getSizeTy();
    // Type: bit
    Ty::Ptr getSignedTy(size_t bit);
    // Type: bit*
    Ty::Ptr getSignedPtrTy(size_t bit);
    // Type: bit**
    Ty::Ptr getSignedPtrPtrTy(size_t bit);
    // Type: bit
    Ty::Ptr getUnsignedTy(size_t bit);
    // Type: bit*
    Ty::Ptr getUnsignedPtrTy(size_t bit);
    // Type: bit**
    Ty::Ptr getUnsignedPtrPtrTy(size_t bit);
    // Type: 16 = half, 32 = float, 64 = double
    Ty::Ptr getFloatTy(size_t bit);
    // Type: 16 = half*, 32 = float*, 64 = double*
    Ty::Ptr getFloatPtrTy(size_t bit);
    // Type: 16 = half**, 32 = float**, 64 = double**
    Ty::Ptr getFloatPtrPtrTy(size_t bit);

    /// VALUES ///

    ConstantInt *getBool(bool value);

    ConstantInt *getInt(size_t bit, uint64_t value);
    
    Value *getNegative(ConstantInt *value);
    
    ConstantFP *getFloat(double value);
    
    Constant *getLiteral(std::string value, std::string name = "");
    
    ConstantPointerNull *getNullPtr(Ty::Ptr ptrTy);

    /// FUNCTIONS ///
    
    /**
     * @brief Declare a llvm::Function without parameters names.
     * 
     * @param type return type of the function
     * @param name name of the function
     * @param parameters parameters of the function 
     * @return Func & - Eisdrache::Func (wrapped llvm::Function)
     */
    Func &declareFunction(Ty::Ptr type, std::string name, Ty::Vec parameters);
    /**
     * @brief Declare a llvm::Function.
     * 
     * @param type return type of the function
     * @param name name of the function
     * @param parameters (optional) parameters of the function 
     * @param entry (optional) creates entry llvm::BasicBlock in function body if true
     * @return Func & - Eisdrache::Func (wrapped llvm::Function)
     */
    Func &declareFunction(Ty::Ptr type, std::string name, Ty::Map parameters = Ty::Map(), bool entry = false);
    
    /**
     * @brief Get the Eisdrache::Func wrapper object
     * 
     * @param function Pointer to llvm::Function
     * @return Func & - Eisdrache::Func (wrapped llvm::Function)
     */
    Func &getWrap(Function *function);

    /**
     * @brief Verify that a Eisdrache::Func is error-free.
     *          TODO: Implement this function.
     * @param wrap Eisdrache::Func (wrapped llvm::Function)
     * @return true - Eisdrache::Func is error-free.
     * @return false - Eisdrache::Func contains errors.
     */
    bool verifyFunc(Func &wrap);

    /**
     * @brief Call a llvm::Function by its wrap.
     * 
     * @param wrap Eisdrache::Func (wrapped llvm::Function) of the callee function
     * @param args (optional) Function call arguments
     * @param name (optional) Name of the returned value
     * @return Value * - Wrapped llvm::Value returned from call
     */
    Local &callFunction(Func &wrap, ValueVec args = {}, std::string name = "");
    /**
     * @brief Call a llvm::Function by its name.
     * 
     * @param callee Name of the callee function
     * @param args (optional) Function call arguments
     * @param name (optional) Name of the returned value
     * @return Value * - Wrapped llvm::Value returned from call
     */
    Local &callFunction(std::string callee, ValueVec args = {}, std::string name = "");

    /// LOCALS ///

    /**
     * @brief Declare (Allocate) a local variable. Automatically adds llvm::AllocaInst to the parent (Eisdrache::Func)
     * 
     * @param type Type to allocate
     * @param name (optional) Name of the AllocaInst *
     * @param value (optional) Future value to be assigned to local variable
     * @param value_args (optional) Arguments if future value is a function
     * @return Local & - Wrapped alloca instruction
     */
    Local &declareLocal(Ty::Ptr type, std::string name = "", Value *future = nullptr, ValueVec future_args = ValueVec());

    /**
     * @brief Load the value of a local variable.
     * 
     * @param local Wrapped llvm::Value
     * @param name (optional) Name of the loaded value.
     * @return Local & - Wrapped llvm::Value
     */
    Local &loadLocal(Local &local, std::string name = "");

    /**
     * @brief Store a value in a local variable.
     * 
     * @param local Local to be stored at (must be a pointer)
     * @param value Value to store in local
     * @return StoreInst * - Store instruction returned by llvm::IRBuilder
     */
    StoreInst *storeValue(Local &local, Local &value);
    /**
     * @brief Store a value in a local variable.
     * 
     * @param local Local to be stored at (must be a pointer)
     * @param value Value to store in local
     * @return StoreInst * - Store instruction returned by llvm::IRBuilder
     */
    StoreInst *storeValue(Local &local, Constant *value);

    /**
     * @brief Create an instruction for the future assignment of a local
     * 
     * @param local The local
     * @param value Value to be assigned
     */
    void createFuture(Local &local, Value *value);
    /**
     * @brief Create an instruction for the future assignment of a local
     * 
     * @param local The local
     * @param func Function to be called on local
     * @param args Arguments for function call
     */
    void createFuture(Local &local, Func &func, ValueVec args);


    /// STRUCT TYPES ///

    /**
     * @brief Declare a struct type.
     * 
     * @param name Name of the struct type
     * @param elements Types of the elements of the struct type
     * @return Struct & - Wrapped llvm::StructType
     */
    Struct::Ptr &declareStruct(std::string name, Ty::Vec elements);

    /**
     * @brief Allocate object of struct type.
     *      Automatically appends to Eisdrache::Func::locals.
     * 
     * @param wrap Wrapped llvm::StructType
     * @param name Name of the returned pointer
     * @return Local & - Wrapped alloca instruction
     */
    Local &allocateStruct(Struct::Ptr wrap, std::string name = "");
    /**
     * @brief Allocate object of struct type.
     *      Automatically appends to Eisdrache::Func::locals.
     * 
     * @param typeName Name of the struct type.
     * @param name Name of the returned pointer
     * @return Local & - Wrapped alloca instruction
     */
    Local &allocateStruct(std::string typeName, std::string name = "");

    /**
     * @brief Get the pointer to element at an index
     * 
     * @param parent Parent of the element
     * @param index Index of the element
     * @param name Name of the returned value
     * @return Local & - Wrapped llvm::Value
     */
    Local &getElementPtr(Local &parent, size_t index, std::string name = "");

    /**
     * @brief Get the value of an element at an index
     * 
     * @param parent Parent of the element
     * @param index Index of the element
     * @param name Name of the returned value
     * @return Local & - Wrapped llvm::Value 
     */
    Local &getElementVal(Local &parent, size_t index, std::string name = "");

    /// BUILDER ///

    /**
     * @brief Create a void return instruction.
     * 
     * @param next (optional) Next insertion point
     * @return ReturnInst * - Return Instruction returned from llvm::IRBuilder
     */
    ReturnInst *createRet(BasicBlock *next = nullptr);
    /**
     * @brief Create a return instruction with a value.
     * 
     * @param value Value to return
     * @param next (optional) Next insertion point
     * @return ReturnInst * - Return Instruction returned from llvm::IRBuilder
     */
    ReturnInst *createRet(Local &value, BasicBlock *next = nullptr);
    /**
     * @brief Create a return instruction with a constant.
     * 
     * @param value Value to return
     * @param next (optional) Next insertion point
     * @return ReturnInst * - Return Instruction returned from llvm::IRBuilder
     */
    ReturnInst *createRet(Constant *value, BasicBlock *next = nullptr);

    /**
     * @brief Create a block and set insert point to it.
     * 
     * @param name (optional) Name of the block
     * @param insert (optional) Start insertion at this block
     * @return BasicBlock * 
     */
    BasicBlock *createBlock(std::string name = "", bool insert = false);

    /**
     * @brief Set the current insertion block.
     * 
     * @param block The insertion block
     */
    void setBlock(BasicBlock *block);

    /**
     * @brief Create a binary operation.
     * 
     * @param op Operation
     * @param LHS Left-Hand-Side
     * @param RHS Right-Hand-Side
     * @param name (optional) Name of the result
     * @return Local & - Result 
     */
    Local &binaryOp(Op op, Local &LHS, Local &RHS, std::string name = ""); 

    /**
     * @brief Bitcast a pointer to a type.
     * 
     * @param ptr The original pointer 
     * @param to The destination type
     * @param name (optional) Name of the returned pointer
     * @return Local & - The returned pointer from the bitcast
     */
    Local &bitCast(Local &ptr, Ty::Ptr to, std::string name = "");

    /**
     * @brief Jump to block.
     * 
     * @param block 
     * @return BranchInst *
     */
    BranchInst *jump(BasicBlock *block);
    /**
     * @brief Jump to `then` if condition is true, else jump to `else´.
     * 
     * @param condition The condition
     * @param then The `then` block
     * @param else_ (optional) The `else` block
     * @return BranchInst *
     */
    BranchInst *jump(Local &condition, BasicBlock *then, BasicBlock *else_ = nullptr);

    /**
     * @brief Type cast a value.
     * 
     * @param local The value
     * @param to The destination type
     * @param name (optional) The name of the casted value
     * @return Local & 
     */
    Local &typeCast(Local &local, Ty::Ptr to, std::string name = "typecast");

    /**
     * @brief Get the pointer to an element of an array.
     * 
     * @param array The array
     * @param index The index of the element
     * @param name (optional) The name of the returned pointer
     * @return Local & 
     */
    Local &getArrayElement(Local &array, size_t index, std::string name = "");

    /**
     * @brief Check wether the given pointer is a nullptr and return result.
     * 
     * @param pointer The pointer 
     * @param name Name of the result
     * @return Local &,
     *          true: pointer == nullptr,
     *          false: pointer != nullptr
     */
    Local &compareToNull(Local &pointer, std::string name = "");

    /**
     * @brief Create an unary operation.
     * 
     * @param op The unary operation
     * @param expr The expression
     * @param name Name of the result
     * @return Local & 
     */
    Local &unaryOp(Op op, Local &expr, std::string name = "");

    /// GETTER ///

    /**
     * @brief Get the llvm::LLVMContext
     * 
     * @return LLVMContext * 
     */
    LLVMContext *getContext();
    
    /**
     * @brief Get the llvm::Module
     * 
     * @return Module * 
     */
    Module *getModule();
    
    /**
     * @brief Get the llvm::IRBuilder
     * 
     * @return IRBuilder<> * 
     */
    IRBuilder<> *getBuilder();
    
    /**
     * @brief Get the current wrapped parent llvm::Function
     * 
     * @return Func & 
     */
    Func &getCurrentParent();
    
    /**
     * @brief Get the vector of Eisdrache::Tys in this context
     * 
     * @return Ty::Vec & 
     */
    Ty::Vec &getTypes();
    
    /**
     * @brief Add a new Eisdrache::Ty to this context.
     *      If the type exists, it will return the pointer to it.
     *      If not, it will add the given type to the context.
     * 
     * @param ty Pointer to the Ty
     * @return Ty::Ptr - Pointer to the found type.
     */
    Ty::Ptr addTy(Ty::Ptr ty);

    /**
     * @brief Get the pointer to a function by its name.
     * 
     * @param name Name of the function
     * @return Func * - Pointer to the found function.  
     */
    Func *getFunc(std::string name);

    /**
     * @brief Set the current parent
     * 
     * @param func Pointer to the new parent
     */
    void setParent(Func *func);

private:
    Eisdrache(LLVMContext *context, Module *module, IRBuilder<> *builder, std::string targetTriple);

    static std::nullptr_t complain(std::string);

    LLVMContext *context;
    Module *module;
    IRBuilder<> *builder;

    Func *parent; // current parent function

    Func::Map functions;
    Struct::Map structs;
    Ty::Vec types;
};

} // namespace llvm
