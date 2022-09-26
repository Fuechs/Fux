// Created by Fuechs on 2022-09-25

#pragma once

#include <optional>
#include <map>
#include <vector>
#include <string>

#include "tokenizer.hpp"
#include "types.hpp"
#include "function.hpp"

namespace fux {

    using std::optional;

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

        void debugPrint() const;
    
    private:
        std::vector<Token>::iterator mEndToken;
        std::vector<Token>::iterator mCurrentToken;
        std::map<std::string, Type> mTypes;
        std::map<std::string, FunctionDefinition> mFunctions;

        /**
         * @brief expects a type 
         * 
         * @return std::optional<Type> : optional token that is detected
         */
        optional<Type> expectType();

        /**
         * @brief expects an identifier with value of name
         * 
         * @param name : identifier value to expect 
         * @return std::optional<Token> : optional token that is detected
         * 
         * ! ~ empty string (name) means match any identifier
         */
        optional<Token> expectIdentifier(const std::string &name = std::string());

        /**
         * @brief expects an operator with value of name
         * 
         * @param name : operator value to expect
         * @return std::optional<Token> : optional token that is detected
         * 
         * ! ~ empty string (name) means match any operator
         */
        optional<Token> expectOperator(const std::string &name = std::string());
    
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