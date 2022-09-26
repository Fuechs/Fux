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
        
        optional<Token> possibleName = expectIdentifier();
        if (possibleName.has_value()) { // We have a name
            optional<Token> possibleOperator = expectOperator("(");
            if (possibleOperator.has_value()) { // We have a function
                FunctionDefinition func;
                while (!expectOperator(")").has_value()) {
                    optional<Token> possibleName = expectIdentifier();
                    ParamterDefinition param;

                    if (possibleName.has_value()) {
                        param.mName = possibleName->mText;

                        if (expectOperator(":").has_value()) {
                            optional<Type> possibleType = expectType();

                            if (possibleType.has_value()) {
                                // parse parameter with type
                                param.mType = possibleType->mName;
                                func.mParameters.push_back(param);
                            }
                        } else {
                            // TODO: parse parameter without type
                            throw std::runtime_error("Automatic types not implemented yet.");
                        }

                        if (expectOperator(")").has_value()) {
                            break;
                        }

                        if (!expectOperator(",").has_value()) {
                            throw std::runtime_error("Expected ',' to seperate parameters.");
                        }
                    } else {
                        throw std::runtime_error("Expected ')' to indicate no parameters.");
                    }
                }

                if (expectOperator(":").has_value()) {
                    optional<Type> possibleType = expectType();
                    if (possibleType.has_value()) {
                        // parse function with type
                        func.mName = possibleName->mText;
                    }
                } else {
                    // TODO: parse function without type
                    throw std::runtime_error("Automatic types not implemented yet.");
                }

                parseFunctionBody();

                mFunctions[func.mName] = func;

                return true;
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
                break;
            }
            
        }
    }

    optional<Token> Parser::expectIdentifier(const std::string &name) {
        if (mCurrentToken == mEndToken) { return std::nullopt; }
        if (mCurrentToken->mType != IDENTIFIER) { return std::nullopt; }
        if (!name.empty() && mCurrentToken->mText != name) { return std::nullopt; }

        Token returnToken = *mCurrentToken;
        ++mCurrentToken;
        return returnToken;
    }

    optional<Token> Parser::expectOperator(const std::string &name) {
        if (mCurrentToken == mEndToken) { return std::nullopt; }
        if (mCurrentToken->mType != OPERATOR) { return std::nullopt; }
        if (!name.empty() && mCurrentToken->mText != name) { return std::nullopt; }

        Token returnToken = *mCurrentToken;
        ++mCurrentToken;
        return returnToken;
    }

    optional<Type> Parser::expectType() {
        std::optional<Token> possibleType = expectIdentifier();
        if (!possibleType) { return std::nullopt; }
        
        std::map<std::string, Type>::iterator foundType = mTypes.find(possibleType->mText);
        if (foundType == mTypes.end()) { 
            --mCurrentToken;
            return std::nullopt; 
        }

        return foundType->second;
    }

    bool Parser::parseFunctionBody() {
        if (!expectOperator("{").has_value())
            return false;

        parseOneStatement();

        if (!expectOperator("}").has_value())
            throw std::runtime_error("Unbalanceed '{'.");
        
        return true;
    }

    void Parser::debugPrint() const {
        for (auto funcPair : mFunctions)
            funcPair.second.debugPrint(); 
    }

    optional<Statement> Parser::parseOneStatement() {
        std::vector<Token>::iterator startToken = mCurrentToken;
        optional<Token> possibleName = expectIdentifier();
        if (!possibleName.has_value()) {
            mCurrentToken = startToken;
            return std::nullopt;
        }

        optional<Token> possibleOperator = expectOperator(":");
        if (possibleName.has_value()) {
            optional<Type> possibleType = expectType();
            if (!possibleType.has_value()) {
                mCurrentToken = startToken;
                return std::nullopt;
            }
            // parse variable with type
        } 
        
        // continue to parse variable
        
    }
    
}