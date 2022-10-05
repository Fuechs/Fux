/**
 * @file parser.hpp
 * @author fuechs 
 * @brief fux parser header
 * @version 0.1
 * @date 2022-09-27
 * 
 * @copyright Copyright (c) 2020-2022, Fuechs. All rights reserved.
 * 
 */

#pragma once

#include <string>
#include <iostream>

#include "../lexer/BaseLex.hpp"
#include "type.hpp"
#include "ast.hpp"

namespace fux {

    using namespace BaseLex;
    using std::string, std::cerr, std::endl;

    class ParserError {
    public:
        ParserError(string message, Token token = Token()) {
            cerr
            << "[Parser] Error occured at Line "
            << token.line << ":" << token.col
            << " : " << message << endl;
            exit(1);
        }
    };


    class Parser {
    public:
        Parser(TokenList tokens) : tokens(tokens) {
            types["void"] =     Type();
            types["i8"] =       Type(I8, "i8");
            types["u8"] =       Type(U8, "u8");
            types["char"] =     Type(U8, "char");
            types["i16"] =      Type(I16, "i16");
            types["u16"] =      Type(U16, "u16");
            types["wchar"] =    Type(U16, "wchar");
            types["i32"] =      Type(I32, "i32");
            types["u32"] =      Type(U32, "u32");
            types["float"] =    Type(FLOAT, "float");
            types["i64"] =      Type(I64, "i64");
            types["u64"] =      Type(U64, "u64");
        }

        AST parse();

    private:
        TokenList tokens;
        TokenList::iterator currentToken = tokens.begin();
        TypeMap types;

     /*
        Token expectTokenType(TokenType type);
       
        Token peek();

        bool matchTokenType(TokenType type);

        Type parseType();
    */
    };

}