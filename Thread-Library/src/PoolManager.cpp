#include "PoolManager.h"

PoolManager::PoolManager(/* args */) {
    //maxThreads = std::thread::hardware_concurrency();
    threadsPtr = std::make_unique<Threads>();
}

PoolManager::~PoolManager() {
}

PoolManager* PoolManager::getInstance(){
    if ( !poolManager )
        poolManager = new PoolManager;
    return poolManager; 
}

template<typename T>
bool PoolManager::AddProcess(T* funcPtr)
{
    return threadsPtr->addProcess(funcPtr);
}