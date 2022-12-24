/**
 * @file color.hpp
 * @author fuechs
 * @brief fux color header - print in color
 * @version 0.1
 * @date 2022-11-05
 * 
 * @copyright Copyright (c) 2020-2022, Fuechs. All rights reserved.
 * 
 */

#pragma once

#include <ostream>
#include <string>

namespace {

    enum ColorCode {
        RED = 31,
        GREEN = 32,
        YELLOW = 33,
        MAGENTA = 35,
        DEFAULT = 39,
    };

    std::ostream &operator<<(std::ostream &os, ColorCode code) {
        return os << "\033[" << static_cast<int>(code) << "m";
    }

    enum StyleCode {
        SLIM = 0,
        BOLD = 1,
    };

    std::ostream &operator<<(std::ostream &os, StyleCode code) {
        return os << "\033[" << static_cast<int>(code) << "m";
    }

    // return yellow string "Debug: "
    std::string getDebugText() {
        return std::string("\033[33mDebug\033[39m: ");
    }

}