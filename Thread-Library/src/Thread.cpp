#include "Thread.h"

Thread::Thread()
: threadStatus(false)
{
    ProcessAssignedWork();
}

template<typename T>
bool Thread::AddToQueue(T* funcPtr)
{
    {
        std::lock_guard<std::mutex> lckgd(queueMutex);
        processPool.pushBack(funcPtr);
    }
    queueConditionVariable.notify_all();
}

void Thread::ProcessAssignedWork()
{
    thread = std::thread([this]
    {
        std::unique_lock<std::mutex> lckgd(queueMutex);
        queueConditionVariable.wait(lckgd, [&] {return !processPool.empty() + !threadStatus;});

        if(!threadStatus)
        {
            while(!processPool.empty())
            {
                auto func = processPool.popBack();
                (*func)();
            }
        }
    });
}