// Created by Fuechs on 2022-09-25

#pragma once

#include <vector>
#include <string>

#include "types.hpp"

namespace fux {

    class ParamterDefinition {
    public:
        ParamterDefinition(std::string name = "", Type type = Type("void", VOID))
        : mName(name), mType(type) {}

        std::string mName; 
        Type mType;

        void debugPrint() const;
    };
    
    class FunctionDefinition {
    public:
        FunctionDefinition(std::string name = "", std::vector<ParamterDefinition> parameters = {})
        : mName(name), mParameters(parameters) {}

        std::string mName;
        std::vector<ParamterDefinition> mParameters;

        void debugPrint() const;
    };

}