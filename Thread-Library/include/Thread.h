#ifndef __THREAD_CLASS__
#define __THREAD_CLASS__

#include <thread>
#include <functional>
#include <condition_variable>
#include <mutex>
#include "TsQueue.h"
#include "Config.h"
#include <chrono>
#include <atomic>
//#include "DataCollector.h"

/*
 * Create a constant set of states possible for the threads to be in
 */
enum THREAD_STATE { INIT, STOPPED, RUNNING, FINISHED };

class Thread {
    private:
        std::thread::id tid;
        std::thread thread;
        TsQueue<FunctionToId> *processPool;
        std::atomic<THREAD_STATE> threadState {INIT};        
        std::condition_variable queueConditionVariable;
        std::mutex queueMutex;
        std::atomic<bool> inScope;
        
        void processAssignedWork();   
    public:
        Thread();
        ~Thread();
        template<typename T>
        bool addToQueue(T* funcPtr, ll processId);
        bool notify();
        THREAD_STATE getRunningState();     
};

#endif
