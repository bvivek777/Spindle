#ifndef __POOL_MANAGER__
#define __POOL_MANAGER__

#include "Thread.h"


class PoolManager {
private:
    int maxThreads;
    static std::atomic<std::vector<Thread>> threadPool; 
    static PoolManager* poolManager;  
    PoolManager(/* args */);
public:
    ~PoolManager();
    static PoolManager* getInstance();
};

PoolManager *PoolManager::poolManager = 0;
#endif