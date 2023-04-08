/**
 * @file io.cpp
 * @author fuechs
 * @brief fux io util
 * @version 0.1
 * @date 2022-11-07
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs and Contributors. All rights reserved.
 * 
 */

#include "io.hpp"
#include "color.hpp"

const std::string readFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "could not open file '" << path << "'\n";
        exit(1);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

const std::string getDirectory(const std::string path) {
    std::string directory = "";
    const size_t lastSlashIdx = path.rfind('/');
    if (lastSlashIdx != std::string::npos)
        directory = path.substr(0, lastSlashIdx);
    return directory;
}

const std::string getFileName(const std::string &path) {
    return path.substr(path.find_last_of("/") + 1);
}

std::string toLower(std::string data) {
    std::transform(data.begin(), data.end(), data.begin(), 
        [](unsigned char c){ return std::tolower(c); });
    return data;
}

std::string escapeSequences(std::string value) {
    std::string buffer = "";
    for (size_t i = 0; i < value.size(); i++) {
        if (value.at(i) == '\\') {
            switch (value.at(++i)) {
                case 'a':   buffer += '\a'; break;  // alert
                case 'b':   buffer += '\b'; break;  // backspace
                case 'f':   buffer += '\f'; break;  // form feed
                case 'n':   buffer += '\n'; break;  // new line
                case 'r':   buffer += '\r'; break;  // carraige return  
                case 't':   buffer += '\t'; break;  // horizontal tab
                case 'v':   buffer += '\v'; break;  // vertical tab
                case '\'':  buffer += '\''; break;  // single quote
                case '\"':  buffer += '\"'; break;  // double quote
                case '\?':  buffer += '\?'; break;  // question mark
                case '\\':  buffer += '\\'; break;  // backslash
                default:    break;                  // remove invalid escape sequences
            }
        } else
            buffer += value.at(i);
    }

    return buffer;
}

std::string unescapeSequences(std::string value) {
    std::string buffer = "";
    for (size_t i = 0; i < value.size(); i++) 
        switch (value.at(i)) {
            case '\a':  buffer += "\\a"; break;
            case '\b':  buffer += "\\b"; break;
            case '\f':  buffer += "\\f"; break;
            case '\n':  buffer += "\\n"; break; 
            case '\r':  buffer += "\\r"; break;
            case '\t':  buffer += "\\t"; break;
            case '\v':  buffer += "\\v"; break;
            case '\'':  buffer += "\\'"; break;
            case '\"':  buffer += "\\\""; break;
            case '\?':  buffer += "\\?"; break;
            case '\\':  buffer += "\\\\"; break;
            default:    buffer += value.at(i); break;
        }
    return buffer;
}

template <typename Out>
void split(const std::string &s, char delim, Out result) {
    std::istringstream iss(s);
    std::string item;
    while (std::getline(iss, item, delim)) 
        *result++ = item;
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

std::string highlightedInsert(const std::string &dest, size_t pos, 
    std::string insert, size_t pos1, std::string insert1) {

        std::stringstream ret; 
        ret << CC::GRAY << dest.substr(0, pos) 
            << CC::WHITE << SC::BOLD << insert << SC::RESET;

        if (!insert1.empty())
            ret << CC::GRAY << dest.substr(pos, pos1 - pos) << CC::WHITE << SC::BOLD << insert1 << SC::RESET;
        else
            pos1 = pos;

        ret << CC::GRAY << dest.substr(pos1);
        return ret.str();
}