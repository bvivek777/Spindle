#ifndef __SPINDLE__
#define __SPINDLE__

#include <vector>
#include <memory>
#include <functional>
#include <unordered_map>
#include "TsQueue.h"
#include "ThreadConfig.h"
#include "Thread.h"
#include "sys/mman.h"
#include <utility>

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
    static std::atomic<ll> processCounter;

    std::unordered_map<int, threadPtr> idThreadMap;
    static std::atomic<bool> flag;

    Spindle() = default;
    Spindle(Config* config);
    ~Spindle();
        
    bool createThreads(int threadCount);
    bool assignFCFS(void (*funcPtr)());
    bool assignML();

public:

    bool init(int threads);

    bool addProcess(void (*functPtr)());

    static Spindle& getInstance(Config* configuration);
    void setFlag();
    void getExecutionState();
};
#endif