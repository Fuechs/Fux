/**
 * @file preprocessor.cpp
 * @author fuechs
 * @brief fux preprocessor
 * @version 0.1
 * @date 2022-10-05
 * 
 * @copyright Copyright (c) 2020-2022, Fuechs. All rights reserved.
 * 
 */

#include "preprocessor.hpp"

namespace fux {

    TokenList PreProcessor::process() {

        TokenList modified;

        while (currentToken != source.end()) {

            if (currentToken->type == HASH) {
                size_t thisLine = currentToken++->line;
                expectTokenType(KEY_GET);
                expectTokenType(LESSTHAN);
                string data = "";
                while (currentToken->type != GREATERTHAN) {
                    data += currentToken++->value;
                    if (currentToken->line > thisLine)
                        PreProcessorError(
                            "Unterminated #get, expected '>' to end source.",
                            *currentToken
                        );
                } ++currentToken;
                cout << "Would be getting " << data << " here.\n";
            } else
                modified.push_back(*currentToken++);

        }

        return modified;

    }

    Token PreProcessor::expectTokenType(TokenType type) {
        if (currentToken->type != type)
            PreProcessorError(
                "Unexpected Token '"
                +currentToken->value
                +"' ("+TokenTypeString[currentToken->type]+") "
                +"expected '"+TokenTypeValue[type]+"' ("+TokenTypeString[type]+") instead.",
                *currentToken
            );

        return *currentToken++;
    }

}