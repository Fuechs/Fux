/**
 * @file metadata.hpp
 * @author fuechs
 * @brief metadata
 * @version 0.1
 * @date 2023-04-07
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#pragma once

#include "../fux.hpp"

class Context { public: using Ptr = std::shared_ptr<Context>; };

struct Metadata {
    Metadata(Context::Ptr ctx = nullptr, std::string file = "", 
        size_t fstLine = 1, size_t lstLine = 1, size_t fstCol = 1, size_t lstCol = 1);
    ~Metadata();

    Metadata &operator=(const Metadata &copy);

    // get line from source code (line number, not index!)
            std::string &operator[](size_t line);
    const   std::string &operator[](size_t line) const;

    // copy whole position of other metadata (without file, source)
    void copyWhole(const Metadata &meta);

    // copy end position of other metadata (without file, source)
    void copyEnd(const Metadata &meta);

    std::string file;
    size_t fstLine, lstLine, fstCol, lstCol;
    Context::Ptr ctx;
};