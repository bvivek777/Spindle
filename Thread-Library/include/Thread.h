#ifndef __THREAD_CLASS__
#define __THREAD_CLASS__

#include <thread>
#include <functional>
#include <condition_variable>
#include <mutex>
#include "TsQueue.h"
#include "Config.h"
#include <chrono>

typedef std::chrono::_V2::system_clock::time_point time;
typedef unsigned long long int ll;

class Thread {
    private:
        std::thread::id tid;
        std::thread thread;
        TsQueue<std::function<void()>> processPool;
        TsQueue<ll> processIds;
        std::condition_variable queueConditionVariable;
        std::mutex queueMutex;
        bool threadStatus;
    public:
        Thread();
        template<typename T>
        bool addToQueue(T* funcPtr, ll processId);
        void processAssignedWork();        
};

#endif
