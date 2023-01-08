/**
 * @file threading.cpp
 * @author fuechs
 * @brief fux threading util
 * @version 0.1
 * @date 2022-12-18
 * 
 * @copyright Copyright (c) 2020-2023, Fuechs. All rights reserved.
 * 
 */

#include "threading.hpp"
#include "../fux.hpp"

namespace fuxThread {

    Thread::Thread(const string &name, size_t id) 
    : name(name), id(id), it(future<void>()) {
        debugPrint("Initialized this thread.");
    }

    Thread::~Thread() { 
        name.clear(); 
        delete &it;
        debugPrint("Deleted this thread.");
    }

    void Thread::run(SourceFile *sf) { 
        debugPrint("Creating thread.");
        it = async(launch::async, ref(*sf));
        debugPrint("Running thread.");
    }

    void Thread::finish() {
        it.get(); 
        debugPrint("Finished thread.");
    }

    void Thread::setName(const string &name) {
        this->name = name;
    }

    ThreadManager::ThreadManager() : threads(ThreadList()), required({{}}) {
        threadsMax = thread::hardware_concurrency() - 2;
        debugPrint("Max amount of threads: "+to_string(threadsMax+2)+" -> "+to_string(threadsMax));
        if (fux.options.debugMode || !fux.options.threading)
            debugPrint("Limited to one (1) thread because of compiler flags.");
    }

    ThreadManager::~ThreadManager() {
        for (Thread *thread : threads)
            delete thread;
        threads.clear();
    }

    void ThreadManager::require(SourceFile *sf) { 
        if (required.back().size() < threadsMax) {
            required.back().push_back(sf);
            return;
        }

        required.push_back({sf});
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

        while (threads.size() < required[0].size()) // create required threads
            threads.push_back(new Thread(required[0][threads.size()]->fileName, threads.size()));
    }

    void ThreadManager::runThreads() {
        if (fux.options.debugMode || !fux.options.threading) {
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