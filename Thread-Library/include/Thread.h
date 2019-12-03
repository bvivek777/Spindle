#ifndef __THREADS__
#define __THREADS__

#include <thread>
#include <vector>
#include <atomic>
#include "TsQueue.h"
#include "Config.h"
#include <memory>
#include <functional>

/*
 * Create a constant set of states possible for the threads to be in
 */
enum ThreadState { INIT, STOPPED, RUNNING, FINISHED };
enum ScheduleType { RR, FCFS };
/* 
 * Class to create and manage the execution of a thread
 * Since the threads created will be governed by the Learning Application
 * This class will only manage the execution of a single thread with multiple
 * functions to be run in parallel 
 */
class Thread {
private:
    //static int maxThreads;
   // static Threads *threads;   
    std::atomic<ThreadState> a {INIT};
    TsQueue<std::function<void(int function)>*> processPool;
    std::unique_ptr<std::thread> thread;
    std::shared_ptr<std::atomic<bool>> flag;
public:    
    Thread(/* args */);
    ~Thread();
    int create();
    int addProcess();
    void setFlag();
    void getExecutionState();

};
//Threads *Threads::threads = 0;
#endif