#ifndef __SPINDLE__
#define __SPINDLE__

#include <vector>
#include <atomic>
#include <memory>
#include <functional>
#include "TsQueue.h"
#include "Config.h"
#include "Thread.h"
#include "sys/mman.h"
#include <utility>

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
    static int hwThreads;
    static int currentThreads;

    std::atomic<ThreadState> a {INIT};
    std::unordered_map<int, threadPtr> idThreadMap;
    std::shared_ptr<std::atomic<bool>> flag;

    Spindle(Config* config);
    ~Spindle();
        
    bool createThreads(int threadCount);

public:

    template<typename T>
    bool addProcess(T* functPtr);

    template<typename T>
    bool addProcess(T* functPtr,int threadCount);

    static Spindle& getInstance(Config* configuration);
    void setFlag();
    void getExecutionState();
};
#endif