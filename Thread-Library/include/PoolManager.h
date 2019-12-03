#ifndef __POOL_MANAGER__
#define __POOL_MANAGER__

#include "Threads.h"


class PoolManager
{
private:
    // Don't delete pointer, as the singleton class is deleted in the destructor
    static Threads *threads;    
    PoolManager(/* args */);
public:
    ~PoolManager();
};

#endif