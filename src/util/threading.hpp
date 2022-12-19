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

#include "source.hpp"

// FIXME: Segmentation fault when trying to access data from SourceFile after thread got detached. 
// Possible solution: Wait until all threads are finished, while still detaching them.
// Still need to figure out how to do that.

using namespace std;

namespace fuxThread {

    class Thread {
    public:
        Thread(const string &name = "unnamed", size_t id = 0);
        ~Thread();

        void debugPrint(const bool sendMessage = false, const string message = "no message");
        // create thread and run sf->parse    
        void run(SourceFile *sf);

    private:
        size_t id;
        string name;
        thread *current;
    };

    typedef vector<Thread *> ThreadList;

    class ThreadManager {
    public:
        ThreadManager();
        ~ThreadManager();

        void operator()(size_t n);
        
        // adds an requirement to parse the SourceFile 'sf'
        void require(SourceFile *sf);
        // prints information about all managed threads
        void debugPrint();
        // create threads that are required
        // delete threads that are unused
        void createThreads(); 
        // parse all required SourceFiles with the threads
        void runThreads();
        
    private:
        thread *master;
        ThreadList threads;
        // respective group of files for each thread
        // max size is 6 KiB each (text data)
        // min size is 3 KiB    "    "
        FileGroups required;

        void runHelper(size_t n);
        
    };

}