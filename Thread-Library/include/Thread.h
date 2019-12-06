#include <thread>
#include <functional>
#include <condition_variable>
#include <mutex>
#include "TsQueue.h"

class Thread
{
    private:
        std::thread::id tid;
        std::thread thread;
        TsQueue<std::function<void()>> processPool;
        std::condition_variable queueConditionVariable;
        std::mutex queueMutex;
        bool threadStatus;

    public:
        Thread();
        template<typename T>
        bool AddToQueue(T* funcPtr);
        void ProcessAssignedWork();
};