#include "Thread.h"

Thread::Thread()
: threadStatus(false)
{
    processAssignedWork();
}

template<typename T>
bool Thread::addToQueue(T* funcPtr)
{
    {
        std::lock_guard<std::mutex> lckgd(queueMutex);
        processPool.pushBack(funcPtr);
    }
    queueConditionVariable.notify_all();
}

void Thread::processAssignedWork()
{
    thread = std::thread([this]
    {
        std::unique_lock<std::mutex> lckgd(queueMutex);
        queueConditionVariable.wait(lckgd, [&] {return !processPool.empty() + !threadStatus;});

        if(!threadStatus) {
            while(!processPool.empty()) {
                auto func = processPool.popBack();
                (*func)();
            }
        }
    });
}