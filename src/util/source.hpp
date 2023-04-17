/**
 * @file source.hpp
 * @author fuechs
 * @brief fux source file header
 * @version 0.1
 * @date 2022-11-26
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#pragma once

#include "../fux.hpp"
#include "../frontend/parser/parser.hpp"
#include "../frontend/analyser/analyser.hpp"

class Source {
public:
    using Vec = vector<Source *>;
    using Groups = vector<Vec>;

    [[deprecated("This constructor is only for debugging purposes.")]]
    Source(const string &file, const string &source);
    Source(const string &filePath, const bool mainFile = false);
    
    ~Source();

    // get std::thread to run this->parse
    void operator()();

    // get source code at a line number
    string &operator[](size_t line);
    
    // parse file and save Root in root
    // will be called for every file that's referenced 
    void parse();

    // return file size in bytes
    // from https://stackoverflow.com/a/32286531
    size_t getFileSize();
    
    string fileName;
    string filePath;
    string fileDir;
    string fileContent;

    Root::Ptr root;
    
private:
    StringVec sourceCode;
    Parser *parser;
    FUX_AC(Analyser *analyser;)
    bool mainFile;
};