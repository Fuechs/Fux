// Created by Fuechs on 2022-09-25

#include <iostream>

#include "include/parser.hpp"

namespace fux {

    /*
    main(void): void {
        ...
    }

    */

    bool Parser::expectFunctionDefinition() {
        // TODO: implement whole function definition and add explanation

        // for error backtracking
        std::vector<Token>::iterator parseStart = mCurrentToken;
        
        std::optional<Token> possibleName = expectIdentifier();
        if (possibleName.has_value()) { // We have a name
            std::optional<Token> possibleOperator = expectOperator("(");

            if (possibleOperator.has_value()) { // We have a function
                while (expectOperator(")") == std::nullopt) {
                    std::optional<Token> possibleName = expectIdentifier();

                    if (possibleName.has_value()) {
                        if (expectOperator(":").has_value()) {
                            std::optional<Type> possibleType = expectType();

                            if (possibleType.has_value()) {
                                // parse parameter with type
                            }
                        } else {
                            // TODO: parse parameter without type
                            throw std::runtime_error("Automatic types not implemented yet.");
                        }

                        if (expectOperator(",") == std::nullopt
                        &&  expectOperator(")") == std::nullopt) {
                            throw std::runtime_error("Expected ',' to seperate parameters or ')' to indicate end of parameter defenition.");
                        }
                    } else {
                        throw std::runtime_error("Expected ')' to indicate no parameters.");
                    }
                }

                if (expectOperator(":").has_value()) {
                    std::optional<Type> possibleType = expectType();
                    if (possibleType.has_value()) {
                        // parse function with type
                    }
                } else {
                    // parse function without type
                }

                parseFunctionBody();

            } else {
                mCurrentToken = parseStart;
            }
        } else {
            mCurrentToken = parseStart;
        }

        return false;
    }

    void Parser::parse(std::vector<Token> &tokens) {
        mEndToken = tokens.end();
        mCurrentToken = tokens.begin();

        while (mCurrentToken != mEndToken) {
            if (expectFunctionDefinition()) {
                ++mCurrentToken;
            } else {
                std::cerr << "Unknown identifier '" << mCurrentToken->mText << "'.\n";
                mCurrentToken += 2;
            }
            
        }
    }

    std::optional<Token> Parser::expectIdentifier(const std::string &name) {
        if (mCurrentToken == mEndToken) { return std::nullopt; }
        if (mCurrentToken->mType != IDENTIFIER) { return std::nullopt; }
        if (!name.empty() && mCurrentToken->mText != name) { return std::nullopt; }

        Token returnToken = *mCurrentToken;
        ++mCurrentToken;
        return returnToken;
    }

    std::optional<Token> Parser::expectOperator(const std::string &name) {
        if (mCurrentToken == mEndToken) { return std::nullopt; }
        if (mCurrentToken->mType != OPERATOR) { return std::nullopt; }
        if (!name.empty() && mCurrentToken->mText != name) { return std::nullopt; }

        Token returnToken = *mCurrentToken;
        ++mCurrentToken;
        return returnToken;
    }

    std::optional<Type> Parser::expectType() {
        std::optional<Token> possibleType = expectIdentifier();
        if (!possibleType) { return std::nullopt; }
        
        std::map<std::string, Type>::iterator foundType = mTypes.find(possibleType->mText);
        if (foundType == mTypes.end()) { 
            --mCurrentToken;
            return std::nullopt; 
        }
        
        return foundType->second;
    }

    void Parser::parseFunctionBody() {

    }
    
}