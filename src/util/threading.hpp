/**
 * @file threading.hpp
 * @author fuechs
 * @brief fux threading util header
 * @version 0.1
 * @date 2022-12-17
 * 
 * @copyright Copyright (c) 2020-2023, Fux programming language and authors. All rights reserved.
 * 
 */

#pragma once

#include "source.hpp"

namespace fuxThread {

    class Thread {
    public:
        Thread(const string &name = "unnamed", size_t id = 0);
        ~Thread();

        void debugPrint(const string message);
        // create thread and run sf->parse    
        void run(SourceFile *sf);

        // wait until thread is finished
        void finish();

        void setName(const string &name); 

    private:
        size_t id;
        string name;
        future<void> it;
    };

    typedef vector<Thread *> ThreadList;

    class ThreadManager {
    public:
        ThreadManager();
        ~ThreadManager();
        
        // adds a requirement to parse the SourceFile 'sf'
        void require(SourceFile *sf);
        // prints information about all managed threads
        void debugPrint(const string message);
        // create threads that are required
        // delete threads that are unused
        void createThreads(); 
        // parse all required SourceFiles with the threads
        void runThreads();
       
        
    private:
        thread *master;
        ThreadList threads;
        FileGroups required;
        size_t threadsMax;
        
        // wait until all threads are finished
        void checkThreads(); 
    };

}