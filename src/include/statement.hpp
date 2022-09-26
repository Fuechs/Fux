// Created by Fuechs on 2022-09-26

#pragma once

#include <string>
#include <vector>

#include "types.hpp"

namespace fux {

    class Statement {
    public:
        std::string mName;
        Type mType;
        std::vector<Statement> mParameters;
    };

}