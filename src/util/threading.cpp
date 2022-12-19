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
    : name(name), id(id), current(nullptr) {
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
        current = new thread(std::ref(*sf)); // we need the RootAST to be in the original SourceFile!
        debugPrint(true, "Detaching thread.");
        current->detach();
        delete &current;
        debugPrint(true, "Destroyed thread.");
    }

    ThreadManager::ThreadManager()
    : threads(ThreadList()), required(FileGroups()) {}

    ThreadManager::~ThreadManager() {
        for (Thread *thread : threads)
            delete thread;
        threads.clear();
    } 

    void ThreadManager::require(SourceFile *sf) {
        // compiler will optimize these out anyways
        size_t KiB3 = 3072; // Bytes (min)
        size_t KiB6 = 6144; // Bytes (max)
        
        if (required.size() == 0) {
            required.push_back({sf});
            return;
        }
        
        FileList &currentGroup = required.back();

        // get size of latest currentGroup 
        size_t currentSize = 0;
        for (SourceFile *sub : currentGroup)
            currentSize += sub->getFileSize();

        // if currentGroup isn't larger than 6 KiB
        // and sf isn't larger than 3KiB
        if (currentSize <= KiB6 && sf->getFileSize() <= KiB3)
            currentGroup.push_back(sf);
        else
            required.push_back({sf});
    }

    void ThreadManager::debugPrint() {
        if (!fux.options.debugMode)
            return;
            
        cout << getDebugText() << "ThreadManager:\n";
        for (Thread *thread : threads)
            thread->debugPrint();
    }

    void ThreadManager::createThreads() {
        for (FileList &fl : required) {
            stringstream name;
            for (SourceFile *sf : fl)
                name << sf->filePath << "; ";
            threads.push_back(new Thread(name.str(), threads.size()));
        }
    }

    void ThreadManager::runThreads() {
        for (size_t i = 0; i < required.size(); i++)
            for (SourceFile *sf : required[i])
                threads[i]->run(sf);
    }

}