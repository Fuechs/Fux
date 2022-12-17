/**
 * @file threading.hpp
 * @author fuechs
 * @brief fux threading util header
 * @version 0.1
 * @date 2022-12-17
 * 
 * @copyright Copyright (c) 2020-2022, Fuechs. All rights reserved.
 * 
 */

#pragma once

#include <string>

using namespace std;

namespace fux {

using pfunc = void (*)(void); // pointer to a function

class Thread {
public:
    Thread(const string &name = "unnamed");
    ~Thread();

    void debugPrint();
    void run(pfunc function);

private:
    thread *current;
};


typedef vector<Thread> ThreadList;


}