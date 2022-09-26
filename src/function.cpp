// Created by Fuechs on 2022-09-26

#include <iostream>

#include "include/function.hpp"

namespace fux {

    void ParamterDefinition::debugPrint() const {
        std::cout << mType.mName << " " << mName << std::endl;
    }

    void FunctionDefinition::debugPrint() const {
        std::cout << mName << "(\n";
        
        for (ParamterDefinition param : mParameters)
            param.debugPrint();
        
        std::cout << ")" << std::endl;
    }

}