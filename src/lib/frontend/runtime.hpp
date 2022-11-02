/**
 * @file runtime.hpp
 * @author fuechs
 * @brief fux runtime header
 * @version 0.1
 * @date 2022-10-30
 * 
 * @copyright Copyright (c) 2020-2022, Fuechs. All rights reserved.
 * 
 */

#pragma once

#include "../../include.hpp"
#include "list.hpp"

class Interpreter {
public:
    template<class T>
    static void freeList(List<T> &lst) {
        for (unsigned int i = 0; i < lst.size(); i++)
            lst.get(i).free();
        lst.free();
    }

    template<class T>
    static void freeList(list<T> &lst) {
        for (T &item : lst)
            item.free();
        lst.clear();
    }
};

int bootstrap(int argc, char **argv);
void rtError(string message);
void printVersion();
string toLower(string s);
bool allIntegers(string intString);
void execRuntime(string fileName);
int help();

extern options cOptions;

#define opt(s) (strcmp(argv[i], s) == 0)