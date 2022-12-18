/**
 * @file threading.cpp
 * @author fuechs
 * @brief fux threading util
 * @version 0.1
 * @date 2022-12-18
 * 
 * @copyright Copyright (c) 2020-2022, Fuechs. All rights reserved.
 * 
 */

#include "threading.hpp"
#include "../fux.hpp"

namespace fuxThread {

    ThreadList threads = ThreadList();

    Thread::Thread(const string &name) : name(name), current(nullptr) {
        id = threads.size(); // id = index of this Thread
        threads.push_back(this);
    }

    Thread::~Thread() { 
        name.clear(); 
        threads.erase(threads.begin() + id);
    }

    void Thread::debugPrint(const string message) {  
        if (!fux.options.debugMode)
            return;
            
        cout << "Debug: Thread '" << name << "' (" << id << "): " << message << "\n";
    }

    void Thread::run(pfunc function) { 
        debugPrint("Creating thread.");
        current = new thread(function);
        debugPrint("Detaching thread.");
        current->detach();
        delete &current;
        debugPrint("Destroyed thread.");
    }

}