#include "PoolManager.h"

PoolManager::PoolManager(/* args */) {
    //@TODO determine the number of threads to create using the data from ML.
    //Creating 3 threads for now.
    for(int i = 0; i < 3; i++)
    {
        //Thread 
    }
}

PoolManager::~PoolManager() {
    //delete thread here
}

template<typename T>
bool PoolManager::addProcess(T* funcPtr)
{
    // based on the ML values assign this function to a thread in the Map
}