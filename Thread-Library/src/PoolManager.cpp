#include "PoolManager.h"

PoolManager::PoolManager(/* args */) {
    maxThreads = std::thread::hardware_concurrency();
}

PoolManager::~PoolManager() {
}

PoolManager* PoolManager::getInstance(){
    if ( !poolManager )
        poolManager = new PoolManager;
    return poolManager; 
}