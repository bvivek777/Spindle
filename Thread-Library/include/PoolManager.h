#ifndef __POOL_MANAGER__
#define __POOL_MANAGER__

#include <thread>
#include <vector>
#include <atomic>
#include <memory>
#include <functional>
#include "TsQueue.h"
#include "Config.h"
#include "Thread.h"

/*
 * Create a constant set of states possible for the threads to be in
 */
enum ThreadState { INIT, STOPPED, RUNNING, FINISHED };
enum ScheduleType { RR, FCFS };

typedef std::shared_ptr<Thread> threadPtr;
/* 
 * Class to create and manage the execution of a thread
 * Since the threads created will be governed by the Learning Application
 * This class will only manage the execution of a single thread with multiple
 * functions to be run in parallel 
 */
class PoolManager {
private:
    //static int maxThreads;
   // static PoolManager *threads;   
    std::atomic<ThreadState> a {INIT};
    TsQueue<std::function<void(int function)>*> processPool;

    // @ToDo create the key of this map based on some unique value which
    // will be decided based on the type of operations the thread performs.
    std::unordered_map<int, threadPtr> idThreadMap;
    int activeThreads;
    std::shared_ptr<std::atomic<bool>> flag;
public:    
    PoolManager(/* args */);
    ~PoolManager();
    int create();

    template<typename T>
    bool addProcess(T* functPtr);
    
    void setFlag();
    void getExecutionState();

};
#endif