#include "Thread.h"

Thread::Thread()
{
    thread = std::thread();
    tid = thread.get_id;
}

template<typename T>
bool Thread::AddToQueue(T* funcPtr)
{
    processPool.pushBack(funcPtr);
}