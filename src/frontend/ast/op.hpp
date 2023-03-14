/**
 * @file op.hpp
 * @author fuechs
 * @brief fux operation type header
 * @version 0.1
 * @date 2023-01-26
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#pragma once

#include "../lexer/token.hpp"

enum class BinaryOp {
    LMOV = LSHIFT,      // left pipe / move <<
    RMOV = RSHIFT,      // right pipe / move >>

    ASG = EQUALS,               // normal assignment =
    CONASG = TRIPLE_EQUALS,     // constant assignment ===
    ADDASG = PLUS_EQUALS,       // addition assignment +=
    SUBASG = MINUS_EQUALS,      // subtraction assignment -=
    MULASG = ASTERISK_EQUALS,   // multiplication assignment *=
    DIVASG = SLASH_EQUALS,      // division assignment /=
    POWASG = CARET_EQUALS,      // power equals ^=
    MODASG = PERCENT_EQUALS,    // modulus equals %=
    BORASG = OR_EQUALS,         // bitwise or equals |=
    BANDASG = AND_EQUALS,       // bitwise and equals &=
    LSHASG = LSH_EQUALS,        // left bitwise shift equals <|=
    RSHASG = RSH_EQUALS,        // right bitwise shift equals |>=
    SWAPASG = SWAP,             // swap assignment <>

    EQUAL = EQUALS_EQUALS,  // equalitiy ==
    UNEQUAL = NOT_EQUALS,   // unequalitiy !=
    LESST = LESSTHAN,       // less than <
    LTE = LTEQUALS,         // less than equals <=
    GREATERT = GREATERTHAN, // greater than >
    GTE = GTEQUALS,         // greater than equals >= 

    LOR = OR,           // logical or ||                    
    LAND = AND,         // logical and &&
    ADD = PLUS,         // addition +
    SUB = MINUS,        // subtraction - 
    MUL = ASTERISK,     // multiplication *
    DIV = SLASH,        // division /
    POW = CARET,        // power ^
    MOD = PERCENT,      // modulus %
    BOR = BIT_OR,       // bitwise or |
    BXOR = BIT_XOR,     // bitwise xor ><
    BAND = BIT_AND,     // bitwise and &
    LSH = BIT_LSHIFT,   // bitwise left shift <|
    RSH = BIT_RSHIFT,   // bitwise right shift |>

    IDX = ARRAY_BRACKET, // index expression <expr>[<expr>] 
};

string BinaryOpValue(BinaryOp &op);

enum class UnaryOp {
    POS = PLUS,             // positive +
    NEG = MINUS,            // negative -
    DEREF = ASTERISK,       // dereference *
    LOGNOT = EXCLAMATION,   // logical not !
    EXIST = QUESTION,       // existence ?
    BNOT = BIT_NOT,         // bitwise not ~
    ADDR = BIT_AND,         // address &
    PINC,                   // prefix increment ++x
    SINC,                   // suffix increment x++
    PDEC,                   // prefix decrement --x
    SDEC,                   // suffix decrement x--
};

string UnaryOpValue(UnaryOp &op);

enum class Inbuilts {
    RETURN = KEY_RETURN,    // return <expr>;
    THROW = KEY_THROW,      // throw <expr>;
    EXIT = KEY_EXIT,        // exit <expr>;
    PUTS = KEY_PUTS,        // puts <expr>;
    PUTCH = KEY_PUTCH,      // putch <expr>;
    READ = KEY_READ,        // read <address>;
    RESERVE = KEY_RESERVE,  // reserve <type>, <size>, <address>;
    FREE = KEY_FREE,        // free <address>;
};

string InbuiltsValue(Inbuilts &inbuilt);