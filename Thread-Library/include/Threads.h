#ifndef __THREADS__
#define __THREADS__

#include <thread>
#include <vector>
#include <atomic>
#include "TsQueue.h"
#include "Config.h"
#include <functional>

/*
 * Create a constant set of states possible for the threads to be in
 */
enum ThreadState { INIT, STOPPED, RUNNING, FINISHED };
/* 
 * Class to create and manage the execution of a thread
 * Since the threads created will be governed by the Learning Application
 * This class will only manage the execution of a single thread with multiple
 * functions to be run in parallel 
 */
class Threads {
private:
    static int maxThreads;
    static Threads *threads;   
    std::atomic<ThreadState> a {INIT};
    TsQueue<std::function<void(int function)>*> processPool;
    /* TsQueue<Threads> threadPool; */    
    Threads(/* args */);
public:
    ~Threads();
    static Threads *getInstance();
    int create();
    int addMoreThreads();
};
Threads *Threads::threads = 0;
#endif