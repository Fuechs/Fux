/**
 * @file preprocessor.hpp
 * @author fuechs (fuechsss@gmail.com)
 * @brief fux preprocessor header
 * @version 0.1
 * @date 2022-10-05
 * 
 * @copyright Copyright (c) 2020-2022, Fuechs.
 *            All rights reserved.
 *            BSD 3-Clause License
 * 
 */

#pragma once

#include "BaseLex.hpp"

namespace fux {

    using namespace BaseLex;

    class PreProcessorError {
    public:
        PreProcessorError(string message, Token token = Token()) {
            std::cerr
            << "[PreProcessor] Error occured at Line "
            << token.line << ":" << token.col
            << " : " << message << std::endl;
            exit(1);
        }
    };

    class PreProcessor {
    public:
        PreProcessor(TokenList source = {}) : source(source) {}


        /**
         * @brief preprocesses (token) source
         * 
         * @return TokenList : modified source
         */
        TokenList process();

    private:
        TokenList source;
        TokenList::iterator currentToken = source.begin();

        Token expectTokenType(TokenType type);

    };

}