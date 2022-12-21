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

    Thread::~Thread() { 
        name.clear(); 
        delete &it;
        debugPrint(true, "Deleted this thread.");
    }

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

    ThreadManager::ThreadManager() : threads(ThreadList()), required({{}}) {
        threadsMax = thread::hardware_concurrency() - 2;
        debugPrint("Max amount of threads: "+to_string(threadsMax+2)+" -> "+to_string(threadsMax));
        if (fux.options.debugMode)
            debugPrint("Limited to one (1) thread for debugging reasons.");
    }

    ThreadManager::~ThreadManager() {
        for (Thread *thread : threads)
            delete thread;
        threads.clear();
    }

    void ThreadManager::require(SourceFile *sf) { 
        for (FileList &fl : required)
            if (fl.size() < threadsMax) {
                fl.push_back(sf);
                return;
            }

        required.push_back({sf});
    }

    void ThreadManager::debugPrint(const string message) {
        if (!fux.options.debugMode)
            return;
            
        cout << getDebugText() << "ThreadManager";
        if (!message.empty())
            cout << ": " << message;
        cout << "\n";
    }

    void ThreadManager::createThreads() {
        if (fux.options.debugMode) {
            threads.push_back(new Thread("universal debug thread", threads.size()));
            return;
        }

        while (threads.size() > required[0].size()) { // delete unused threads
            delete threads.back();
            threads.pop_back();
        }

        for (SourceFile *sf : required[0]) { // max amount of threads required
            if (threads.size() >= required[0].size())
                return;
            threads.push_back(new Thread(sf->fileName, threads.size()));
        }
    }

    void ThreadManager::runThreads() {
        if (fux.options.debugMode) {
            for (FileList &fl : required)
                for (SourceFile *sf : fl) {
                    threads[0]->run(sf);
                    threads[0]->finish();
                }
            return;
        }

        for (FileList &fl : required) {
            for (size_t i = 0; i < fl.size(); i++)
                threads[i]->run(fl[i]);
            checkThreads();
        }
    }

    void ThreadManager::checkThreads() {
        for (Thread *thread : threads)
            thread->finish();
    }

}