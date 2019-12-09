#include "Thread.h"
#include "TsQueue.h"

Thread::Thread() {
    processPool = new TsQueue<FunctionToId>();
    threadState = THREAD_STATE::INIT;
    inScope = true;
}

Thread::~Thread(){
    inScope = false;
    delete processPool;
    thread.join();
}

template<typename T>
bool Thread::addToQueue(T* funcPtr, ll processId)
{
    {
        std::lock_guard<std::mutex> lckgd(queueMutex);
        FunctionToId funcId(funcPtr,processId);
        processPool->pushBack(funcId);
    }
    queueConditionVariable.notify_all();
    return true;
}

void Thread::processAssignedWork()
{   
    if ( !processPool->size() )
        return;
    thread = std::thread([this]
    {
        std::unique_lock<std::mutex> lckgd(queueMutex);
        queueConditionVariable.wait(lckgd, [&] {return !processPool->empty() + (threadState == THREAD_STATE::INIT);});
        std::chrono::_V2::system_clock::time_point startTime, endTime;
        FunctionToId *func;
        ll runTime;
        if( threadState == THREAD_STATE::INIT ) {
            threadState = THREAD_STATE::RUNNING;
            while ( inScope ) {
                while(!processPool->empty()) {                
                    startTime = std::chrono::high_resolution_clock::now();
                    func = processPool->popBack();
                    (*func->funcPtr)();
                    endTime = std::chrono::high_resolution_clock::now();
                    runTime = std::chrono::duration_cast<std::chrono::nanoseconds>(startTime-endTime).count();
                }
            }
            threadState = THREAD_STATE::FINISHED;
        }
    });
}

THREAD_STATE Thread::getRunningState(){
    return threadState;
}