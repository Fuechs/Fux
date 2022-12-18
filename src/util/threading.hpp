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
#include <vector>
#include <iostream>
#include <thread>

using namespace std;

namespace fuxThread {

    using pfunc = void (*)(void); // pointer to a function

    class Thread {
    public:
        Thread(const string &name = "unnamed");
        ~Thread();

        void debugPrint(const string message = "no message");
        void run(pfunc function);

    private:
        size_t id;
        string name;
        thread *current;
    };

    typedef vector<Thread *> ThreadList;

    extern ThreadList threads;

}