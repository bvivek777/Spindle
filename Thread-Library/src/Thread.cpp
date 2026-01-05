#include "../include/Thread.h"

Thread::Thread() {
    processPool = new TsQueue<FunctionToId>();
    threadState = THREAD_STATE::INIT;
    inScope = true;
    processAssignedWork();
}

Thread::~Thread(){   
    wait();
    delete processPool;
}

bool Thread::addToQueue(void (*funcPtr)(), ll processId) {
    if (funcPtr == nullptr) {
        return false;
    }
    FunctionToId funcId(funcPtr, processId);
    
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        processPool->pushBack(funcId);
    }
    queueConditionVariable.notify_one();
    return true;
}

void Thread::processAssignedWork() {
    thread = std::thread([this] {
        FunctionToId func;
        
        while( inScope ) {
            // Use condition variable to avoid busy waiting
            std::unique_lock<std::mutex> lock(queueMutex);
            queueConditionVariable.wait(lock, [this] {
                return !processPool->empty() || !inScope;
            });
            
            if (!inScope && processPool->empty()) {
                break;
            }
            
            // Pop from queue (TsQueue has its own mutex, so this is safe)
            // Unlock our mutex before popping to avoid holding multiple locks
            lock.unlock();
            func = processPool->popFront();
            
            // Only process if we got a valid function
            if (func.isValid()) {
                try {
                    (func.funcPtr)();
                }
                catch(const std::exception& e) {
                    std::cerr << "Exception in thread execution: " << e.what() << '\n';
                }
            }
        }
    });
}

bool Thread::wait() {
    // Set flag to stop accepting new work
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        inScope = false;
    }
    queueConditionVariable.notify_all(); // Wake up thread to exit
    
    // Wait for thread to finish
    if (thread.joinable()) {
        thread.join();
    }
    
    return true;
}

int Thread::isPending() {
    // Thread-safe check of queue size
    return static_cast<int>(processPool->size());
}

THREAD_STATE Thread::getRunningState(){
    return threadState;
}
