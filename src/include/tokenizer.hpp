// Created by Fuechs on 2022-09-25

#pragma once

#include <vector>
#include <string>

namespace fux {

    enum TokenType {
        WHITESPACE,         // No token has this type; Normal case
        IDENTIFIER,     
        INTEGER_LITERAL,
        FLOAT_LITERAL,
        STRING_LITERAL,
        OPERATOR,
        STRING_ESCAPE_SEQUENCE,
        POTENTIAL_FLOAT,
        POTENTIAL_COMMENT,
        COMMENT,
    };

    static const char *sTokenTypeStrings[] = {
        "WHITESPACE",       
        "IDENTIFIER",     
        "INTEGER_LITERAL",
        "FLOAT_LITERAL",
        "STRING_LITERAL",
        "OPERATOR",
        "STRING_ESCAPE_SEQUENCE",
        "POTENTIAL_FLOAT",
        "POTENTIAL_COMMENT",
        "COMMENT"
    };

    /**
     * @brief An individual token
     * Examples: '+', '1', 'main', '(', '}'
     */
    class Token {
    public: 
        // whitespace is normal case for token type
        enum TokenType mType {WHITESPACE};
        std::string mText; // original literal value
        /**
         * @brief Exact position of the token for error tracking
         * ~ mStartOffset:  Position of first char in token
         * ~ mEndOffset:    Position of last char in token
         * ~ mLineNumber:   Position of token itself in the source
         */
        // size_t mStartOffset {0};
        // size_t mEndOffset {0};
        size_t mLineNumber {0};

        /**
         * @brief prints information about this token
         * 
         */
        void debugPrint() const;
    };

    class Tokenizer {
    public:
        /**
         * @brief Tokenizes the program source
         *
         * @param inProgram : Program source
         * @return std::vector<Token> : Vector of all tokens collected
         */
        std::vector<Token> parse(const std::string &inProgram);
    
    private:
        /**
         * @brief Appends token to vector of tokens and resets current token
         * 
         * @param token : Token to append
         * @param tokens : Vector to append to
         */
        void endToken(Token &token, std::vector<Token> &tokens);
    };
    
}