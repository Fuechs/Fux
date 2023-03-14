/**
 * @file eisdrache.cpp
 * @author fuechs
 * @brief Eisdrache class implementation
 * @version 0.1
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

#include "eisdrache.hpp"

#ifdef FUX_BACKEND

namespace llvm {

/// EISDRACHE TY ///

Eisdrache::Ty::Ty(Eisdrache *eisdrache, Type *llvmTy) {
    this->eisdrache = eisdrache;
    structTy = nullptr;
    ptrDepth = 0;
    isSigned = true;
    isFloat = true;
    switch (llvmTy->getTypeID()) {
        case Type::IntegerTyID:
            bit = llvmTy->getIntegerBitWidth();
            isSigned = false;
            isFloat = false;
            break;
        case Type::HalfTyID:    bit = 16; break;
        case Type::FloatTyID:   bit = 32; break;
        case Type::DoubleTyID:  bit = 64; break;
        case Type::PointerTyID: 
            bit = 0; 
            ptrDepth = 1; 
            isSigned = false;
            isFloat = false;
            break;
        case Type::StructTyID:
            Eisdrache::complain("Eisdrache::Ty::Ty(): Can not construct Eisdrache::Ty from a llvm::Type with ID: Type::StructTyID.");
        case Type::FunctionTyID:
            Eisdrache::complain("Eisdrache::Ty::Ty(): Can not construct Eisdrache::Ty from a llvm::Type with ID: Type::FunctionTyID.");
        case Type::ArrayTyID:
            Eisdrache::complain("Eisdrache::Ty::Ty(): Can not construct Eisdrache::Ty from a llvm::Type with ID: Type::ArrayTyID.");
        default:
            break;
    }
}

Eisdrache::Ty::Ty(Eisdrache *eisdrache, size_t bit, size_t ptrDepth, bool isFloat, bool isSigned)
: eisdrache(eisdrache), bit(bit), ptrDepth(ptrDepth), 
    isFloat(isFloat), isSigned(isSigned), structTy(nullptr) {}

Eisdrache::Ty::Ty(Eisdrache *eisdrache, Struct *structTy, size_t ptrDepth)
: eisdrache(eisdrache), bit(0), ptrDepth(ptrDepth), 
    isFloat(false), isSigned(false), structTy(structTy) {}

Eisdrache::Ty &Eisdrache::Ty::operator=(const Ty &copy) {
    bit = copy.bit;
    ptrDepth = copy.ptrDepth;
    isFloat = copy.isFloat;
    isSigned = copy.isSigned;
    structTy = copy.structTy;
    eisdrache = copy.eisdrache;
    return *this;
}

bool Eisdrache::Ty::operator==(const Ty &comp) const {
    return (bit == comp.bit 
        && ptrDepth == comp.ptrDepth 
        && isFloat == comp.isFloat
        && isSigned == comp.isSigned
        && structTy == comp.structTy
        && eisdrache == comp.eisdrache);
}

bool Eisdrache::Ty::operator==(const Type *comp) const {
    return (bit == comp->getIntegerBitWidth() 
        && (bool) ptrDepth == comp->isPointerTy()
        && isFloat == comp->isFloatingPointTy());
}

Eisdrache::Ty *Eisdrache::Ty::operator*() const {
    return eisdrache->addTy(new Ty(eisdrache, bit, ptrDepth > 0 ? ptrDepth - 1 : 0, isFloat));
}

Type *Eisdrache::Ty::getTy() const {
    if (ptrDepth > 0)
        return PointerType::get(*eisdrache->getContext(), 0);
    
    if (structTy)
        return **structTy;

    if (isFloat) 
        switch (bit) {
            case 16:    return Type::getHalfTy(*eisdrache->getContext());
            case 32:    return Type::getFloatTy(*eisdrache->getContext());
            case 64:    return Type::getDoubleTy(*eisdrache->getContext());
            default:    return complain("Eisdrache::Ty::getTy(): Invalid amount of bits ("+std::to_string(bit)+") for floating point type (expected 16|32|64).");
        }

    if (bit == 0)
        return Type::getVoidTy(*eisdrache->getContext());
    
    return Type::getIntNTy(*eisdrache->getContext(), bit);
}

Eisdrache::Ty *Eisdrache::Ty::getPtrTo() const { return eisdrache->addTy(new Ty(eisdrache, bit, ptrDepth + 1, isFloat)); }

Eisdrache::Struct &Eisdrache::Ty::getStructTy() const {
    if (!structTy)
        Eisdrache::complain("Eisdrache::Ty::getStructTy(): Tried to get struct type, but there is none.");
    return *structTy;
}

size_t Eisdrache::Ty::getBit() const { return bit; }

bool Eisdrache::Ty::isFloatTy() const { return isFloat; }

bool Eisdrache::Ty::isSignedTy() const { return isSigned; }

bool Eisdrache::Ty::isPtrTy() const { return ptrDepth > 0; }

bool Eisdrache::Ty::isValidRHS(const Ty *comp) const {
    return bit == comp->bit
        && isFloat == comp->isFloat
        && isPtrTy() == comp->isPtrTy();
}

/// EISDRACHE LOCAL ///

Eisdrache::Local::Local(Eisdrache *eisdrache, Constant *constant) 
: eisdrache(eisdrache), v_ptr(constant), future(nullptr) {
    type = eisdrache->addTy(new Ty(eisdrache, constant->getType()));
}

Eisdrache::Local::Local(Eisdrache *eisdrache, Ty *type, Value *ptr, Value *future, ValueVec future_args)
: eisdrache(eisdrache), type(type), v_ptr(ptr), future(future), future_args(future_args) {}

Eisdrache::Local& Eisdrache::Local::operator=(const Local &copy) {
    v_ptr = copy.v_ptr;
    type = copy.type;
    future = copy.future;
    eisdrache = copy.eisdrache;
    return *this;
}

bool Eisdrache::Local::operator==(const Local &comp) const { return v_ptr == comp.v_ptr; }

bool Eisdrache::Local::operator==(const Value *comp) const { return v_ptr == comp; }

AllocaInst *Eisdrache::Local::operator*() {
    invokeFuture();
    if (!isAlloca())
        return complain("Eisdrache::Local::operator*(): Tried to get AllocaInst * of Value * (%"+v_ptr->getName().str()+").");
    return a_ptr;
}

void Eisdrache::Local::setPtr(Value *ptr) { v_ptr = ptr; }

void Eisdrache::Local::setFuture(Value *future) { this->future = future; }

void Eisdrache::Local::setFutureArgs(ValueVec args) { future_args = args; }

void Eisdrache::Local::setTy(Ty *ty) { type = ty; }

AllocaInst *Eisdrache::Local::getAllocaPtr() { return operator*(); }

Value *Eisdrache::Local::getValuePtr() { 
    invokeFuture();
    return v_ptr; 
}

Eisdrache::Ty *Eisdrache::Local::getTy() { return type; }

std::string Eisdrache::Local::getName() const { 
    if (!v_ptr || !v_ptr->hasName())
        return "unnamed";
    return v_ptr->getName().str();
}

bool Eisdrache::Local::isAlloca() { return isa<AllocaInst>(v_ptr); }

Eisdrache::Local &Eisdrache::Local::loadValue(bool force, std::string name) {
    if ((!force && !isAlloca()) || !type->isPtrTy())
        return *this;

    if (isAlloca())
        invokeFuture();

    Ty *loadTy = **type;
    LoadInst *load = eisdrache->getBuilder()->CreateLoad(loadTy->getTy(), 
        v_ptr, name.empty() ? v_ptr->getName().str()+"_load" : name);
    return eisdrache->getCurrentParent().addLocal(Local(eisdrache, loadTy, load));
}

void Eisdrache::Local::invokeFuture() {
    if (!future)
        return;
        
    if (Function *func = dyn_cast<Function>(future)) {
        if (func->getReturnType()->isVoidTy()) {
            eisdrache->getBuilder()->CreateCall(func, future_args);
            future = nullptr;
            future_args.clear();
            return;
        } else 
            future = eisdrache->getBuilder()->CreateCall(func, future_args, getName()+"_future");
    } 

    eisdrache->getBuilder()->CreateStore(future, v_ptr);
    future = nullptr;
    future_args.clear();
}

/// EISDRACHE FUNC ///

Eisdrache::Func::Func() {
    func = nullptr;
    type = nullptr;
    parameters = Local::Vec();
    locals = Local::Map();
    eisdrache = nullptr;
}

Eisdrache::Func::Func(Eisdrache *eisdrache, Ty *type, std::string name, Ty::Map parameters, bool entry) {
    this->eisdrache = eisdrache;
    this->type = type;
    this->locals = Local::Map();
    this->parameters = Local::Vec();

    std::vector<std::string> paramNames;
    std::vector<Type *> paramTypes;
    for (Ty::Map::value_type &param : parameters) {
        paramNames.push_back(param.first);
        paramTypes.push_back(param.second->getTy());
        this->parameters.push_back(Local(eisdrache, param.second));
    }

    FunctionType *FT = FunctionType::get(type->getTy(), paramTypes, false);
    func = Function::Create(FT, Function::ExternalLinkage, name, *eisdrache->getModule());

    for (size_t i = 0; i < func->arg_size(); i++) {  
        func->getArg(i)->setName(paramNames[i]);
        this->parameters[i].setPtr(func->getArg(i));
    }

    if (entry) {
        BasicBlock *entry = BasicBlock::Create(*eisdrache->getContext(), "entry", func);
        eisdrache->setBlock(entry);
    } else
        llvm::verifyFunction(*func);
}

Eisdrache::Func::~Func() { locals.clear(); }

Eisdrache::Func &Eisdrache::Func::operator=(const Func &copy) {
    func = copy.func;
    type = copy.type;
    parameters = copy.parameters;
    locals = copy.locals;
    eisdrache = copy.eisdrache;
    return *this;
}

bool Eisdrache::Func::operator==(const Func &comp) const { return func == comp.func; }

bool Eisdrache::Func::operator==(const Function *comp) const { return func == comp; }

Eisdrache::Local &Eisdrache::Func::operator[](std::string symbol) { return locals.at(symbol); }

Function *Eisdrache::Func::operator*() { return func; }

Eisdrache::Local &Eisdrache::Func::arg(size_t index) { return parameters[index]; }

Eisdrache::Local &Eisdrache::Func::call(ValueVec args, std::string name) { 
    Value *ret = eisdrache->getBuilder()->CreateCall(func, args, name); 
    return eisdrache->getCurrentParent().addLocal(Local(eisdrache, type, ret));
}
 
Eisdrache::Local &Eisdrache::Func::addLocal(Local local) { 
    std::string symbol = "";
    if (local.getName() == "unnamed" || locals.contains(local.getName()))
        symbol = local.getName()+std::to_string(locals.size());
    else
        symbol = local.getName();
    locals[symbol] = local;
    return locals[symbol];
}

Eisdrache::Ty *Eisdrache::Func::getTy() { return type; }

/// EISDRACHE STRUCT ///

Eisdrache::Struct::Struct() {
    name = "";
    type = nullptr;
    elements = Ty::Vec();
    eisdrache = nullptr;
}

Eisdrache::Struct::Struct(Eisdrache *eisdrache, std::string name, Ty::Vec elements) {
    TypeVec elementTypes = TypeVec();
    for (Ty *&e : elements)
        elementTypes.push_back(e->getTy());
    this->name = name;
    this->type = StructType::create(elementTypes, name);
    this->elements = elements;
    this->eisdrache = eisdrache;
}

Eisdrache::Struct::~Struct() { name.clear(); }

Eisdrache::Struct &Eisdrache::Struct::operator=(const Struct &copy) {
    name = copy.name;
    type = copy.type;
    elements = copy.elements;
    eisdrache = copy.eisdrache;
    return *this;
}

bool Eisdrache::Struct::operator==(const Struct &comp) const { return type == comp.type; }

bool Eisdrache::Struct::operator==(const Type *comp) const { return type == comp; }

Eisdrache::Ty *Eisdrache::Struct::operator[](size_t index) { return elements.at(index); }

StructType *Eisdrache::Struct::operator*() { return type; }

Eisdrache::Local &Eisdrache::Struct::allocate(std::string name) {
    return eisdrache->allocateStruct(*this, name);
} 

Eisdrache::Ty *Eisdrache::Struct::getPtrTy() { return eisdrache->addTy(new Ty(eisdrache, this, 1)); }

Eisdrache::Func *Eisdrache::Struct::createMemberFunc(Ty *type, std::string name, Ty::Map args) {
    Ty::Map processed = {{"this", getPtrTy()}};
    for (Ty::Map::value_type &x : args)
        processed.push_back(x);
    return &eisdrache->declareFunction(type, this->name+"_"+name, processed, true);
}

/// EISDRACHE ARRAY ///

Eisdrache::Array::Array(Eisdrache *eisdrache, Ty *elementTy, std::string name) {
    this->eisdrache = eisdrache;
    this->name = name;
    this->elementTy = elementTy;
    this->bufferTy = elementTy->getPtrTo();
    this->self = &eisdrache->declareStruct(name, {
        bufferTy,                   // TYPE* buffer
        eisdrache->getSizeTy(),     // i64 size
        eisdrache->getSizeTy(),     // i64 max
        eisdrache->getSizeTy(),     // i64 factor
    });

    Func *malloc = nullptr;
    if (!(malloc = eisdrache->getFunc("malloc")))
        malloc = &eisdrache->declareFunction(eisdrache->getUnsignedPtrTy(8), "malloc", 
            {eisdrache->getSizeTy()});

    Func *free = nullptr;
    if (!(free = eisdrache->getFunc("free")))
        free = &eisdrache->declareFunction(eisdrache->getVoidTy(), "free",
            {eisdrache->getUnsignedPtrTy(8)});

    Func *memcpy = nullptr;
    if (!(memcpy = eisdrache->getFunc("memcpy")))
        memcpy = &eisdrache->declareFunction(eisdrache->getUnsignedPtrTy(8), "memcpy",
            {eisdrache->getUnsignedPtrTy(8), 
                eisdrache->getUnsignedPtrTy(8), 
                eisdrache->getSizeTy()});

    { // get_buffer
    get_buffer = self->createMemberFunc(bufferTy, "get_buffer");
    Local &buffer = eisdrache->getElementVal(get_buffer->arg(0), 0, "buffer");
    eisdrache->createRet(buffer);
    }

    { // set_buffer
    set_buffer = self->createMemberFunc(eisdrache->getVoidTy(), "set_buffer",
        {{"buffer", bufferTy}});
    Local &buffer_ptr = eisdrache->getElementPtr(set_buffer->arg(0), 0, "buffer_ptr");
    eisdrache->storeValue(buffer_ptr, set_buffer->arg(1));
    eisdrache->createRet();
    }

    { // get_size
    get_size = self->createMemberFunc(eisdrache->getSizeTy(), "get_size");
    Local &size = eisdrache->getElementVal(get_size->arg(0), 1, "size");
    eisdrache->createRet(size);
    }

    { // set_size
    set_size = self->createMemberFunc(eisdrache->getVoidTy(), "set_size",
        {{"size", eisdrache->getSizeTy()}});
    Local &size_ptr = eisdrache->getElementPtr(set_size->arg(0), 1, "size_ptr");
    eisdrache->storeValue(size_ptr, set_size->arg(1));
    eisdrache->createRet();
    }

    { // get_max
    get_max = self->createMemberFunc(eisdrache->getSizeTy(), "get_max");
    Local &max = eisdrache->getElementVal(get_max->arg(0), 2, "max");
    eisdrache->createRet(max);
    }

    { // set_max
    set_max = self->createMemberFunc(eisdrache->getVoidTy(), "set_max",
        {{"max", eisdrache->getSizeTy()}});
    Local &max_ptr = eisdrache->getElementPtr(set_max->arg(0), 1, "max_ptr");
    eisdrache->storeValue(max_ptr, set_max->arg(1));
    eisdrache->createRet();
    }
    
    { // get_factor
    get_factor = self->createMemberFunc(eisdrache->getSizeTy(), "get_factor");
    Local &factor = eisdrache->getElementVal(get_factor->arg(0), 3, "factor");
    eisdrache->createRet(factor);
    }

    { // set_factor
    set_factor = self->createMemberFunc(eisdrache->getVoidTy(), "set_factor",
        {{"factor", eisdrache->getSizeTy()}});
    Local &factor_ptr = eisdrache->getElementPtr(set_factor->arg(0), 1, "factor_ptr");
    eisdrache->storeValue(factor_ptr, set_factor->arg(1));
    eisdrache->createRet();
    }

    { // constructor
    constructor = self->createMemberFunc(eisdrache->getVoidTy(), "constructor");
    (**constructor)->setCallingConv(CallingConv::Fast);
    (**constructor)->setDoesNotThrow();
    set_buffer->call({constructor->arg(0).getValuePtr(), eisdrache->getNullPtr(bufferTy)});
    set_size->call({constructor->arg(0).getValuePtr(), eisdrache->getInt(64, 0)});
    set_max->call({constructor->arg(0).getValuePtr(), eisdrache->getInt(64, 0)});
    set_factor->call({constructor->arg(0).getValuePtr(), eisdrache->getInt(64, 16)});
    eisdrache->createRet();
    }

    { // destructor
    destructor = self->createMemberFunc(eisdrache->getVoidTy(), "destructor");
    (**destructor)->setCallingConv(CallingConv::Fast);
    (**destructor)->setDoesNotThrow();
    BasicBlock *free_begin = eisdrache->createBlock("free_begin");
    BasicBlock *free_close = eisdrache->createBlock("free_close");
    Local &buffer = get_buffer->call({destructor->arg(0).getValuePtr()}, "buffer");
    eisdrache->jump(eisdrache->compareToNull(buffer, "cond"), free_close, free_begin);
    eisdrache->setBlock(free_begin);
    Local &buffer_cast = eisdrache->bitCast(buffer, eisdrache->getUnsignedPtrTy(8), "buffer_cast");
    free->call({buffer.getValuePtr()});
    eisdrache->jump(free_close);
    eisdrache->setBlock(free_close);
    eisdrache->createRet();
    }

    { // resize
    resize = self->createMemberFunc(eisdrache->getVoidTy(), "resize",
        {{"new_size", eisdrache->getSizeTy()}});
    BasicBlock *copy = eisdrache->createBlock("copy");
    BasicBlock *empty = eisdrache->createBlock("empty");
    BasicBlock *end = eisdrache->createBlock("end"); 
    
    Local byteSize = Local(eisdrache, eisdrache->getInt(64, elementTy->getBit() / 8));
    Local &bytes = eisdrache->binaryOp(MUL, resize->arg(1), byteSize, "bytes");
    Local &new_buffer = malloc->call({bytes.getValuePtr()}, "new_buffer");
    Local &buffer = get_buffer->call({resize->arg(0).getValuePtr()}, "buffer");
    Local &size = get_size->call({resize->arg(0).getValuePtr()}, "size");
    eisdrache->jump(eisdrache->compareToNull(buffer, "cond"), empty, copy);
    
    eisdrache->setBlock(copy);
    memcpy->call({new_buffer.getValuePtr(), buffer.getValuePtr(), size.getValuePtr()});
    free->call({buffer.getValuePtr()});
    eisdrache->jump(end);

    eisdrache->setBlock(empty);
    eisdrache->storeValue(new_buffer, eisdrache->getNullPtr(bufferTy));
    eisdrache->jump(end);
    
    eisdrache->setBlock(end);
    set_buffer->call({resize->arg(0).getValuePtr(), new_buffer.getValuePtr()});
    Local &max_ptr = eisdrache->getElementPtr(resize->arg(0), 3, "max_ptr");
    eisdrache->storeValue(max_ptr, resize->arg(1));
    eisdrache->createRet();
    }
}

Eisdrache::Array::~Array() { name.clear(); }

Eisdrache::Local &Eisdrache::Array::allocate(std::string name) {
    return eisdrache->allocateStruct(*self, name);
}

Eisdrache::Local &Eisdrache::Array::call(Member callee, ValueVec args, std::string name) {
    switch (callee) {
        case GET_BUFFER:    return get_buffer->call(args, name);
        case SET_BUFFER:    return set_buffer->call(args, name);
        case GET_SIZE:      return get_size->call(args, name);
        case SET_SIZE:      return set_size->call(args, name);
        case GET_MAX:       return get_max->call(args, name);
        case SET_MAX:       return set_max->call(args, name);
        case GET_FACTOR:    return get_factor->call(args, name);
        case SET_FACTOR:    return set_factor->call(args, name);
        case CONSTRUCTOR:   return constructor->call(args, name);
        case DESTRUCTOR:    return destructor->call(args, name);
        case RESIZE:        return resize->call(args, name);
        default:            
            Eisdrache::complain("Eisdrache::Array::call(): Callee not implemented.");
            return eisdrache->getCurrentParent().arg(0); // silence warning
    }
}

/// EISDRACHE WRAPPER ///

Eisdrache::~Eisdrache() {
    delete builder;
    functions.clear();
    structs.clear();
    types.clear();
}

void Eisdrache::initialize() {
    PassRegistry *registry = PassRegistry::getPassRegistry();
    InitializeAllTargetInfos();
    InitializeAllTargets();
    InitializeAllTargetMCs();
    InitializeNativeTarget();
    initializeTarget(*registry);
}

Eisdrache *Eisdrache::create(std::string moduleID, std::string targetTriple) {
    LLVMContext *context = new LLVMContext();
    return new Eisdrache(context, new Module(moduleID, *context), new IRBuilder<>(*context), targetTriple);
}

Eisdrache *Eisdrache::create(LLVMContext *context, std::string moduleID, std::string targetTriple) {
    return new Eisdrache(context, new Module(moduleID, *context), new IRBuilder<>(*context), targetTriple);
}

Eisdrache *Eisdrache::create(Module *module, std::string targetTriple) {
    return new Eisdrache(&module->getContext(), module, new IRBuilder<>(module->getContext()), targetTriple);
}

Eisdrache *Eisdrache::create(Module *module, IRBuilder<> *builder, std::string targetTriple) {
    return new Eisdrache(&module->getContext(), module, builder, targetTriple);
};

void Eisdrache::dump(raw_fd_ostream &os) { module->print(os, nullptr); }

/// TYPES ///

Eisdrache::Ty *Eisdrache::getVoidTy() { return addTy(new Ty(this)); }

Eisdrache::Ty *Eisdrache::getBoolTy() { return addTy(new Ty(this, 1)); }

Eisdrache::Ty *Eisdrache::getSizeTy() { return addTy(new Ty(this, 64)); }

Eisdrache::Ty *Eisdrache::getSignedTy(size_t bit) { return addTy(new Ty(this, bit, 0, false, true)); }

Eisdrache::Ty *Eisdrache::getSignedPtrTy(size_t bit) { return addTy(new Ty(this, bit, 1, false, true));}

Eisdrache::Ty *Eisdrache::getSignedPtrPtrTy(size_t bit) { return addTy(new Ty(this, bit, 2, false, true)); }

Eisdrache::Ty *Eisdrache::getUnsignedTy(size_t bit) { return addTy(new Ty(this, bit, 0, false)); }

Eisdrache::Ty *Eisdrache::getUnsignedPtrTy(size_t bit) { return addTy(new Ty(this, bit, 1, false)); }

Eisdrache::Ty *Eisdrache::getUnsignedPtrPtrTy(size_t bit) { return addTy(new Ty(this, bit, 2, false)); }

Eisdrache::Ty *Eisdrache::getFloatTy(size_t bit) { return addTy(new Ty(this, bit, 0, true, true)); }

Eisdrache::Ty *Eisdrache::getFloatPtrTy(size_t bit) { return addTy(new Ty(this, bit, 1, true, true)); }

Eisdrache::Ty *Eisdrache::getFloatPtrPtrTy(size_t bit) { return addTy(new Ty(this, bit, 2, true, true)); };

/// VALUES ///

ConstantInt *Eisdrache::getBool(bool value) { return builder->getInt1(value); }

ConstantInt *Eisdrache::getInt(size_t bit, uint64_t value) { return builder->getIntN(bit, value); }

Value *Eisdrache::getNegative(ConstantInt *value) { 
    return builder->CreateSub(builder->getIntN(value->getBitWidth(), 0), value, "eisdrache_negate_");
}

ConstantFP *Eisdrache::getFloat(double value) { return ConstantFP::get(*context, APFloat(value)); }

Constant *Eisdrache::getLiteral(std::string value, std::string name) { return builder->CreateGlobalStringPtr(value, name); }

ConstantPointerNull *Eisdrache::getNullPtr(Ty *ptrTy) { return ConstantPointerNull::get(dyn_cast<PointerType>(ptrTy->getTy())); }

/// FUNCTIONS ///

Eisdrache::Func &Eisdrache::declareFunction(Ty *type, std::string name, Ty::Vec parameters) {   
    Ty::Map parsedParams = Ty::Map();
    for (Ty *&param : parameters)
        parsedParams.push_back({std::to_string(parsedParams.size()), param});
    functions[name] = Func(this, type, name, parsedParams);
    parent = &functions.at(name);
    return *parent;
}

Eisdrache::Func &Eisdrache::declareFunction(Ty *type, std::string name, Ty::Map parameters, bool entry) {
    functions[name] = Func(this, type, name, parameters, entry);
    parent = &functions.at(name);
    return *parent;
}

Eisdrache::Func &Eisdrache::getWrap(Function *function) {
    for (Func::Map::value_type &wrap : functions)
        if (wrap.second == function)
            return wrap.second;
    complain("Could not find Eisdrache::Func of @" + function->getName().str() + "().");
    return functions.end()->second;
}

bool Eisdrache::verifyFunc(Func &wrap) { 
    return llvm::verifyFunction(**wrap); 
}

Eisdrache::Local &Eisdrache::callFunction(Func &wrap, ValueVec args, std::string name) { 
    return wrap.call(args, name);
}

Eisdrache::Local &Eisdrache::callFunction(std::string callee, ValueVec args, std::string name) { 
    return functions.at(callee).call(args, name);
}
 
/// LOCALS ///

Eisdrache::Local &Eisdrache::declareLocal(Ty *type, std::string name, Value *value, ValueVec future_args) {
    AllocaInst *alloca = builder->CreateAlloca(type->getTy(), nullptr, name);
    return parent->addLocal(Local(this, type->getPtrTo(), alloca, value, future_args));
}

Eisdrache::Local &Eisdrache::loadLocal(Local &local, std::string name) { return local.loadValue(); }

StoreInst *Eisdrache::storeValue(Local &local, Local &value) {
    if (!local.getTy()->isPtrTy())
        return Eisdrache::complain("Eisdrache::storeValue(): Local is not a pointer (%"+local.getName()+").");

    Ty *storeType = **local.getTy();

    if (storeType != value.getTy() 
    && ((!local.getTy()->isFloatTy() 
    && !value.getTy()->isFloatTy())
    || value.getTy()->isPtrTy()))
        return Eisdrache::complain("Eisdrache::storeValue(): Value type does not match type of local.");
    
    return builder->CreateStore(value.getValuePtr(), local.getValuePtr());
} 

StoreInst *Eisdrache::storeValue(Local &local, Constant *value) {
    if (!local.getTy()->isPtrTy())
        return Eisdrache::complain("Eisdrache::storeValue(): Local is not a pointer.");
    
    return builder->CreateStore(value, local.getValuePtr());
}

void Eisdrache::createFuture(Local &local, Value *value) { local.setFuture(value); }

void Eisdrache::createFuture(Local &local, Func &func, ValueVec args) {
    local.setFuture(*func);
    local.setFutureArgs(args);
}

/// STRUCT TYPES ///

Eisdrache::Struct &Eisdrache::declareStruct(std::string name, Ty::Vec elements) {
    structs[name] = Struct(this, name, elements);
    return structs.at(name);
}

Eisdrache::Local &Eisdrache::allocateStruct(Struct &wrap, std::string name) {
    AllocaInst *alloca = builder->CreateAlloca(*wrap, nullptr, name);
    return parent->addLocal(Local(this, addTy(new Ty(this, &wrap, 1)), alloca));
}

Eisdrache::Local &Eisdrache::allocateStruct(std::string typeName, std::string name) {
    Struct &ref = structs.at(typeName);
    AllocaInst *alloca = builder->CreateAlloca(*ref, nullptr, name);
    return parent->addLocal(Local(this, ref.getPtrTy(), alloca));
}

Eisdrache::Local &Eisdrache::getElementPtr(Local &parent, size_t index, std::string name) {
    Struct &ref = parent.getTy()->getStructTy();
    Value *gep = builder->CreateGEP(*ref, parent.getValuePtr(), 
        {getInt(32, 0), getInt(32, index)}, name);
    return this->parent->addLocal(Local(this, ref[index]->getPtrTo(), gep));
}

Eisdrache::Local &Eisdrache::getElementVal(Local &parent, size_t index, std::string name) {
    Local &ptr = getElementPtr(parent, index, name+"_ptr");
    return ptr.loadValue(true, name);
}

/// BUILDER ///

ReturnInst *Eisdrache::createRet(BasicBlock *next) {
    ReturnInst *inst = builder->CreateRetVoid();
    if (next)
        builder->SetInsertPoint(next);
    return inst;
}

ReturnInst *Eisdrache::createRet(Local &value, BasicBlock *next) {
    ReturnInst *inst = builder->CreateRet(value.loadValue().getValuePtr());
    if (next)
        builder->SetInsertPoint(next);
    return inst;
}

ReturnInst *Eisdrache::createRet(Constant *value, BasicBlock *next) {
    ReturnInst *inst = builder->CreateRet(value);
    if (next)
        builder->SetInsertPoint(next);
    return inst;
}

BasicBlock *Eisdrache::createBlock(std::string name, bool insert) {
    BasicBlock *BB = BasicBlock::Create(*context, name, **parent);
    if (insert)
        setBlock(BB);
    return BB;
}

void Eisdrache::setBlock(BasicBlock *block) {
    builder->SetInsertPoint(block);
}

Eisdrache::Local &Eisdrache::binaryOp(Op op, Local &LHS, Local &RHS, std::string name) {
    Local &l = LHS.loadValue(false, LHS.getName()+"_lhs_load");
    Local &r = RHS.loadValue(false, RHS.getName()+"_rhs_load");
    Ty *ty = l.getTy();
    Local bop = Local(this, ty); 
    if (!ty->isValidRHS(r.getTy()))
        Eisdrache::complain("Eisdrache::binaryOp(): LHS and RHS types differ.");

    switch (op) {
        case ADD:
            if (name.empty()) name = "addtmp";
            if (ty->isFloatTy()) 
                bop.setPtr(builder->CreateFAdd(l.getValuePtr(), r.getValuePtr(), name));
            else
                bop.setPtr(builder->CreateAdd(l.getValuePtr(), r.getValuePtr(), name));
            break;
        case SUB:
            if (name.empty()) name = "subtmp";
            if (ty->isFloatTy())
                bop.setPtr(builder->CreateFSub(l.getValuePtr(), r.getValuePtr(), name));
            else
                bop.setPtr(builder->CreateSub(l.getValuePtr(), r.getValuePtr(), name));
            break;
        case MUL:
            if (name.empty()) name = "multmp";
            if (ty->isFloatTy())
                bop.setPtr(builder->CreateFMul(l.getValuePtr(), r.getValuePtr(), name));
            else
                bop.setPtr(builder->CreateMul(l.getValuePtr(), r.getValuePtr(), name));
            break;
        case DIV:
            if (name.empty()) name = "divtmp";
            if (ty->isFloatTy())
                bop.setPtr(builder->CreateFDiv(l.getValuePtr(), r.getValuePtr(), name));
            else if (ty->isSignedTy())
                bop.setPtr(builder->CreateSDiv(l.getValuePtr(), r.getValuePtr(), name));
            else
                bop.setPtr(builder->CreateUDiv(l.getValuePtr(), r.getValuePtr(), name));
            break;
        case OR:    
            bop.setPtr(builder->CreateOr(l.getValuePtr(), r.getValuePtr(), name.empty() ? "ortmp" : name)); 
            break;
        case XOR:   
            bop.setPtr(builder->CreateXor(l.getValuePtr(), r.getValuePtr(), name.empty() ? "xortmp" : name)); 
            break;
        case AND:   
            bop.setPtr(builder->CreateAnd(l.getValuePtr(), r.getValuePtr(), name.empty() ? "andtmp" : name)); 
            break;
        case LSH:   
            bop.setPtr(builder->CreateShl(l.getValuePtr(), r.getValuePtr(), name.empty() ? "lshtmp" : name)); 
            break;
        case RSH:   
            bop.setPtr(builder->CreateLShr(l.getValuePtr(), r.getValuePtr(), name.empty() ? "rshtmp" : name)); 
            break;
        case EQU:
            if (name.empty()) name = "equtmp";
            if (ty->isFloatTy())
                bop.setPtr(builder->CreateFCmpOEQ(l.getValuePtr(), r.getValuePtr(), name)); 
            else
                bop.setPtr(builder->CreateICmpEQ(l.getValuePtr(), r.getValuePtr(), name));
            break;
        case NEQ:
            if (name.empty()) name = "neqtmp";
            if (ty->isFloatTy())
                bop.setPtr(builder->CreateFCmpONE(l.getValuePtr(), r.getValuePtr(), name));
            else
                bop.setPtr(builder->CreateICmpNE(l.getValuePtr(), r.getValuePtr(), name));
            break;
        case LES:
            if (name.empty()) name = "lestmp";
            if (ty->isFloatTy())
                bop.setPtr(builder->CreateFCmpOLT(l.getValuePtr(), r.getValuePtr(), name));
            else if (ty->isSignedTy())
                bop.setPtr(builder->CreateICmpSLT(l.getValuePtr(), r.getValuePtr(), name));
            else
                bop.setPtr(builder->CreateICmpULT(l.getValuePtr(), r.getValuePtr(), name));
            break;
        case LTE:
            if (name.empty()) name = "ltetmp";
            if (ty->isFloatTy())
                bop.setPtr(builder->CreateFCmpOLE(l.getValuePtr(), r.getValuePtr(), name));
            else if (ty->isSignedTy())
                bop.setPtr(builder->CreateICmpSLE(l.getValuePtr(), r.getValuePtr(), name));
            else
                bop.setPtr(builder->CreateICmpULE(l.getValuePtr(), r.getValuePtr(), name));
            break;
        case GRE:
            if (name.empty()) name = "gretmp";
            if (ty->isFloatTy())
                bop.setPtr(builder->CreateFCmpOGT(l.getValuePtr(), r.getValuePtr(), name));
            else if (ty->isSignedTy())
                bop.setPtr(builder->CreateICmpSGT(l.getValuePtr(), r.getValuePtr(), name));
            else
                bop.setPtr(builder->CreateICmpUGT(l.getValuePtr(), r.getValuePtr(), name));
            break;
        case GTE:
            if (name.empty()) name = "gtetmp";
            if (ty->isFloatTy())
                bop.setPtr(builder->CreateFCmpOGE(l.getValuePtr(), r.getValuePtr(), name));
            else if (ty->isSignedTy())
                bop.setPtr(builder->CreateICmpSGE(l.getValuePtr(), r.getValuePtr(), name));
            else
                bop.setPtr(builder->CreateICmpUGE(l.getValuePtr(), r.getValuePtr(), name));
            break;
        default:
            Eisdrache::complain("Eisdrache::binaryOp(): Operation (ID "+std::to_string(op)+") not implemented.");
    }

    return parent->addLocal(bop);
}

Eisdrache::Local &Eisdrache::bitCast(Local &ptr, Ty *to, std::string name) {
    Value *cast = builder->CreateBitCast(ptr.getValuePtr(), to->getTy(), name);
    return parent->addLocal(Local(this, to, cast));
}

BranchInst *Eisdrache::jump(BasicBlock *next) {
    return builder->CreateBr(next);
}

BranchInst *Eisdrache::jump(Local &condition, BasicBlock *then, BasicBlock *else_) {
    return builder->CreateCondBr(condition.loadValue().getValuePtr(), then, else_);
}

Eisdrache::Local &Eisdrache::typeCast(Local &value, Ty *to, std::string name) {
    if (*value.getTy() == *to)
        complain("Eisdrache::typeCast(): Redundant type cast.");

    Value *v = value.loadValue().getValuePtr();
    Ty *from = value.getTy();
    Local &cast = parent->addLocal(Local(this, to));
    
    if (from->isFloatTy()) {
        if (to->isFloatTy()) {                                                      // FLOAT -> FLOAT
            if (from->getBit() < to->getBit())
                cast.setPtr(builder->CreateFPExt(v, to->getTy(), name));
            else
                cast.setPtr(builder->CreateFPTrunc(v, to->getTy(), name));
        } else if (to->isSignedTy())                                                // FLOAT -> SIGNED
            cast.setPtr(builder->CreateFPToSI(v, to->getTy(), name));
        else if (to->isPtrTy())                                                     // FLOAT -> POINTER
            complain("Eisdrache::typeCast(): Invalid type cast (Float -> Pointer).");
        else                                                                        // FLOAT -> UNSIGNED
            cast.setPtr(builder->CreateFPToUI(v, to->getTy(), name));
    } else if (from->isSignedTy()) {
        if (to->isFloatTy())                                                        // SIGNED -> FLOAT
            cast.setPtr(builder->CreateSIToFP(v, to->getTy(), name));
        else if (to->isPtrTy())                                                     // SIGNED -> POINTER
            cast.setPtr(builder->CreateIntToPtr(v, to->getTy(), name));
        else if (to->isSignedTy()) {                                                // SIGNED -> SIGNED
            if (from->getBit() < to->getBit())
                cast.setPtr(builder->CreateSExt(v, to->getTy(), name));
            else
                cast.setPtr(builder->CreateTrunc(v, to->getTy(), name));
        } else {                                                                    // SIGNED -> UNSIGNED
            if (from->getBit() < to->getBit())
                cast.setPtr(builder->CreateZExt(v, to->getTy(), name));
            else
                cast.setPtr(builder->CreateTrunc(v, to->getTy(), name));
        }
    } else if (from->isPtrTy()) {
        if (to->isFloatTy())                                                        // POINTER -> FLOAT
            complain("Eisdrache::typeCast(): Invalid type cast (Pointer -> Float).");
        else if (to->isPtrTy())                                                     // POINTER -> POINTER
            return bitCast(value, to, name);
        else 
            cast.setPtr(builder->CreatePtrToInt(v, to->getTy(), name));             // POINTER -> INTEGER
    } else {
        if (to->isFloatTy())                                                        // UNSIGNED -> FLOAT
            cast.setPtr(builder->CreateUIToFP(v, to->getTy(), name));
        else if (to->isPtrTy()) {                                                   // UNSIGNED -> POINTER
            cast.setPtr(builder->CreateIntToPtr(v, to->getTy(), name));
        } else {                                                                    // UNSIGNED -> INTEGER
            if (from->getBit() < to->getBit()) 
                cast.setPtr(builder->CreateZExt(v, to->getTy(), name));
            else
                cast.setPtr(builder->CreateTrunc(v, to->getTy(), name));
        }
    }

    return cast;
}

Eisdrache::Local &Eisdrache::getArrayElement(Local &array, size_t index, std::string name) {
    Value *ptr = builder->CreateGEP(array.getTy()->getTy(), array.getValuePtr(),
        {getInt(32, index)}, name);
    return parent->addLocal(Local(this, array.getTy(), ptr));
}

Eisdrache::Local &Eisdrache::compareToNull(Local &pointer, std::string name) {
    if (!pointer.getTy()->isPtrTy())
        complain("Eisdrache::compareToNull(): Local is not a pointer.");
    Value *cond = builder->CreateICmpEQ(pointer.getValuePtr(), getNullPtr(pointer.getTy()), name);
    return parent->addLocal(Local(this, getBoolTy(), cond));
}

/// GETTER ///

LLVMContext *Eisdrache::getContext() { return context; }

Module *Eisdrache::getModule() { return module; }

IRBuilder<> *Eisdrache::getBuilder() { return builder; }

Eisdrache::Func &Eisdrache::getCurrentParent() { return *parent; }

Eisdrache::Ty::Vec &Eisdrache::getTypes() { return types; }

Eisdrache::Ty *Eisdrache::addTy(Ty *ty) {
    for (Ty *&cty : types)
        if (*cty == *ty)
            return cty;
    types.push_back(ty);
    return types.back();
}

Eisdrache::Func *Eisdrache::getFunc(std::string name) {
    return (functions.contains(name) ? &functions[name] : nullptr);
}

void Eisdrache::setParent(Func *func) { parent = func; }

/// PRIVATE ///

Eisdrache::Eisdrache(LLVMContext *context, Module *module, IRBuilder<> *builder, std::string targetTriple) {
    this->context = context;
    this->module = module;
    this->builder = builder;
    parent = nullptr;
    functions = Func::Map();
    structs = Struct::Map();

    TargetOptions targetOptions = TargetOptions();
    targetOptions.FloatABIType = FloatABI::Hard;
    TargetMachine *targetMachine = nullptr;

    if (targetTriple.empty()) {
        EngineBuilder engineBuilder = EngineBuilder();
        engineBuilder.setTargetOptions(targetOptions);
        targetMachine = engineBuilder.selectTarget();
    } else {
        std::string error;
        const Target *target = TargetRegistry::lookupTarget(targetTriple, error);
        if (!target) 
            complain("TargetRegistry::lookupTargt() failed: " + error);
        targetMachine = target->createTargetMachine(targetTriple, "generic", "", targetOptions, None);
    }

    module->setTargetTriple(targetMachine->getTargetTriple().str());
    module->setDataLayout(targetMachine->createDataLayout());
}

std::nullptr_t Eisdrache::complain(std::string message) {
    std::cerr << "\033[31mError\033[0m: " << message << "\n"; 
    exit(1);
    return nullptr; // for warnings
} 

} // namespace llvm

#endif