#include "Thread.h"
#include "TsQueue.h"

Thread::Thread()
: threadStatus(false)
{
    processPool = new TsQueue<FunctionToId>();
    processAssignedWork();
}

template<typename T>
bool Thread::addToQueue(T* funcPtr, ll processId)
{
    {
        std::lock_guard<std::mutex> lckgd(queueMutex);
        //processPool.pushBack(funcPtr);
    }
    queueConditionVariable.notify_all();
}

void Thread::processAssignedWork()
{
    thread = std::thread([this]
    {
        std::unique_lock<std::mutex> lckgd(queueMutex);
        queueConditionVariable.wait(lckgd, [&] {return !processPool->empty() + !threadStatus;});
        time startTime, endTime;
        FunctionToId *func;
        ll *id;
        if(!threadStatus) {
            threadStatus = true;
            while(!processPool->empty()) {                
                startTime = std::chrono::high_resolution_clock::now();
                func = processPool->popBack();
                (*func->funcPtr)();
                endTime = std::chrono::high_resolution_clock::now();
                //if ( con )
            }
        }
        threadStatus = false;
    });
}