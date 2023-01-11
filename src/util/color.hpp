/**
 * @file color.hpp
 * @author fuechs
 * @brief fux color header - print in color
 * @version 0.1
 * @date 2022-11-05
 * 
 * @copyright Copyright (c) 2020-2023, Fux programming language and authors. All rights reserved.
 * 
 */

#pragma once

#include <ostream>
#include <string>

namespace {

    enum class ColorCode {
        BLACK = 30,
        RED = 31,
        GREEN = 32,
        YELLOW = 33,
        BLUE = 34,
        MAGENTA = 35,
        CYAN = 36,
        WHITE = 37,
            
        DEFAULT = 39,
        
        GRAY = 90,
        BRIGHT_RED = 91,
        BRIGHT_GREEN = 92,
        BRIGHT_YELLOW = 93,
        BRIGHT_BLUE = 94,
        BRIGHT_MAGENTA = 95,
        BRIGHT_CYAN = 96,
        BRIGHT_WHITE = 97,
    };

    enum class BackColor {
        BLACK = 40,
        RED = 41,
        GREEN = 42,
        YELLOW = 43,
        BLUE = 44,
        MAGENTA = 45,
        CYAN = 46,
        WHITE = 47,
        
        DEFAULT = 49,
        
        GRAY = 100,
        BRIGHT_RED = 101,
        BRIGHT_GREEN = 102,
        BRIGHT_YELLOW = 103,
        BRIGHT_BLUE = 104,
        BRIGHT_MAGENTA = 105,
        BRIGHT_CYAN = 106,
        BRIGHT_WHITE = 107, 
    };

    enum class StyleCode {
        RESET,
        BOLD,
        FAINT,
        ITALIC,
        UNDERLINE,
        SLOW_BLINK,
        RAPID_BLINK,
        SWAP_COLOR,
        CONCEAL,
        STRIKETHROUGH,
    };

    using CC = ColorCode;
    using BC = BackColor;
    using SC = StyleCode;

    std::ostream &operator<<(std::ostream &os, CC code) {
        return os << "\033[" << static_cast<int>(code) << "m";
    }

    std::ostream &operator<<(std::ostream &os, BC code) {
        return os << "\033[" << static_cast<int>(code) << "m";
    }

    std::ostream &operator<<(std::ostream &os, SC code) {
        return os << "\033[" << static_cast<int>(code) << "m";
    }

    // yellow string "Debug: "
    std::string debugText = "\033[33mDebug\033[39m: ";
}