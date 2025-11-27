#include "../include/Thread.h"

Thread::Thread() {
    processPool = new TsQueue<FunctionToId>();
    threadState = THREAD_STATE::INIT;
    inScope = true;
    processAssignedWork();
}

Thread::~Thread(){   
    inScope = false;
    queueConditionVariable.notify_all(); // Wake up waiting thread
    if (thread.joinable()) {
        thread.join();
    }
    delete processPool;
}

bool Thread::addToQueue(void (*funcPtr)(), ll processId) {
    if (funcPtr == nullptr) {
        return false;
    }
    FunctionToId funcId(funcPtr, processId);
    processPool->pushBack(funcId);
    queueConditionVariable.notify_one(); // Use notify_one instead of notify_all for better performance
    return true;
}

void Thread::processAssignedWork() {
    thread = std::thread([this] {
        FunctionToId func;
        std::chrono::high_resolution_clock::time_point startTime, endTime;
        std::chrono::duration<double> runtime;
        
        while( inScope ) {
            // Use condition variable to avoid busy waiting
            std::unique_lock<std::mutex> lock(queueMutex);
            queueConditionVariable.wait(lock, [this] {
                return !processPool->empty() || !inScope;
            });
            
            if (!inScope && processPool->empty()) {
                break;
            }
            
            lock.unlock();
            func = processPool->popFront();
            
            // Only process if we got a valid function
            if (func.isValid()) {
                try {
                    startTime = std::chrono::high_resolution_clock::now();
                    (func.funcPtr)();
                    endTime = std::chrono::high_resolution_clock::now();
                    runtime = std::chrono::duration_cast<std::chrono::duration<double>>(endTime - startTime);
                }
                catch(const std::exception& e) {
                    std::cerr << "Exception in thread execution: " << e.what() << '\n';
                }
            }
        }
    });
}

bool Thread::wait() {
    if ( processPool->empty() ) {
        inScope = false;
        queueConditionVariable.notify_all(); // Wake up thread to exit
        return true;
    }
    return false;
}

int Thread::isPending() {
    return processPool->size() > 0;
}

THREAD_STATE Thread::getRunningState(){
    return threadState;
}