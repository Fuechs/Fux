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

    Thread::Thread(const string &name, size_t id) 
    : name(name), id(id), it(future<void>()) {
        debugPrint(true, "Initialized this thread.");
    }

    Thread::~Thread() { name.clear(); }

    void Thread::debugPrint(const bool sendMessage, const string message) {  
        if (!fux.options.debugMode)
            return;
            
        cout << getDebugText() << "Thread '" << name << "' (" << id << ")";
        if (sendMessage)
            cout << ": " << message;
        cout << "\n";
    }

    void Thread::run(SourceFile *sf) { 
        debugPrint(true, "Creating thread.");
        it = async(launch::async, ref(*sf));
        debugPrint(true, "Running thread.");
    }

    void Thread::finish() {
        it.get(); 
        debugPrint(true, "Finished thread.");
    }

    ThreadManager::ThreadManager()
    : threads(ThreadList()), required(FileList()) {}

    ThreadManager::~ThreadManager() {
        for (Thread *thread : threads)
            delete thread;
        threads.clear();
    } 

    void ThreadManager::require(SourceFile *sf) { required.push_back(sf); }

    void ThreadManager::debugPrint() {
        if (!fux.options.debugMode)
            return;
            
        cout << getDebugText() << "ThreadManager:\n";
        for (Thread *thread : threads)
            thread->debugPrint();
    }

    void ThreadManager::createThreads() {
        for (SourceFile *fl : required)
            threads.push_back(new Thread(fl->fileName, threads.size()));
    }

    void ThreadManager::runThreads() {
        for (size_t i = 0; i < required.size(); i++)
            threads.at(i)->run(required.at(i));
    }

    void ThreadManager::checkThreads() {
        for (Thread *t : threads)
            t->finish();
    }

}