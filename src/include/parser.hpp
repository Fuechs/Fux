// Created by Fuechs on 2022-09-25

#pragma once

#include <optional>
#include <map>

#include "tokenizer.hpp"

namespace fux {

    enum BUILTIN_TYPE {
        VOID,       // void, nothing, nada
        BOOL,       // boolean
        I8,         // 8-bit integer
        U8,         // unsigned 8-bit integer
        I16,        // 16-bit integer
        U16,        // unsigned 16-bit integer
        I32,        // 32-bit integer
        U32,        // unsigned 32-bit integer
        FLOAT,      // 32-bit floating point integer
        I64,        // 64-bit integer
        U64,        // unsigned 64-bit integer
        DOUBLE,     // 64-bit floating point integer
        STR,        // string
        STRUCT,     // struct
        ENUM,       // TODO: enum
    };

    /**
     * @brief Individual type in fux
     * Examples: i32, bool, str
     */
    class Type {
    public:
        Type(const std::string name = "", enum BUILTIN_TYPE type = VOID) 
        : mName(name), mType(type) {}

        std::string mName;
        enum BUILTIN_TYPE mType;
        std::vector<Type> mFields; // for STRUCT only
    };

    class Parser {
    public:
        Parser() {
            mTypes["void"] =    Type("void", VOID);
            mTypes["bool"] =    Type("bool", BOOL);
            mTypes["i8"] =      Type("i8", I8);
            mTypes["u8"] =      Type("u8", U8);
            mTypes["char"] =    Type("char", I8);
            mTypes["i16"] =     Type("i16", I16);
            mTypes["u16"] =     Type("u16", U16);
            mTypes["wchar"] =   Type("wchar", I16);
            mTypes["i32"] =     Type("i32", I32);
            mTypes["u32"] =     Type("u32", U32);
            mTypes["float"] =   Type("float", FLOAT);
            mTypes["i64"] =     Type("i64", I64);
            mTypes["u64"] =     Type("u64", U64);
            mTypes["double"] =  Type("u32", DOUBLE);
        }

        /**
         * @brief parses tokens into an ast
         * 
         * @param tokens : vector of tokens to parse
         */
        void parse(std::vector<Token> &tokens);
    
    private:
        std::vector<Token>::iterator mEndToken;
        std::vector<Token>::iterator mCurrentToken;
        std::map<std::string, Type> mTypes;

        /**
         * @brief expects a type 
         * 
         * @return std::optional<Type> : optional token that is detected
         */
        std::optional<Type> expectType();

        /**
         * @brief expects an identifier with value of name
         * 
         * @param name : identifier value to expect 
         * @return std::optional<Token> : optional token that is detected
         * 
         * ! empty string (name) means match any identifier
         */
        std::optional<Token> expectIdentifier(const std::string &name = std::string());

        /**
         * @brief expects an operator with value of name
         * 
         * @param name : operator value to expect
         * @return std::optional<Token> : optional token that is detected
         * 
         * ! empty string (name) means match any operator
         */
        std::optional<Token> expectOperator(const std::string &name = std::string());
    
        /**
         * @brief expects a function definition
         * 
         * @return true : function definition detected
         * @return false : no function definition
         */
        bool expectFunctionDefinition();

        /**
         * @brief parses function body
         * 
         */
        void parseFunctionBody();
    };

}