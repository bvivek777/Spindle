#ifndef __SPINDLE__
#define __SPINDLE__

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
class Spindle {
private:
    static int maxThreads;  
    std::atomic<ThreadState> a {INIT};
    TsQueue<std::function<void(int function)>*> processPool;
    static Spindle* spindle;
    std::unordered_map<int, threadPtr> idThreadMap;
    int activeThreads;
    std::shared_ptr<std::atomic<bool>> flag;
    Spindle(/* args */);
    ~Spindle();
public:    
    int create();
    template<typename T>
    bool addProcess(T* functPtr);
    Spindle* getInstance();
    void setFlag();
    void getExecutionState();

};
#endif