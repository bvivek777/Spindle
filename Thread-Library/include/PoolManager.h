#ifndef __POOL_MANAGER__
#define __POOL_MANAGER__

#include "Threads.h"


class PoolManager {
private:
    int maxThreads;
    std::unique_ptr<Threads> threadsPtr;
    static std::atomic<std::vector<Threads>> threadPool; 
    static PoolManager* poolManager;  
    PoolManager(/* args */);
public:
    ~PoolManager();
    static PoolManager* getInstance();
    template<typename T>
    bool AddProcess(T* funcPtr);
};

PoolManager *PoolManager::poolManager = nullptr;
#endif