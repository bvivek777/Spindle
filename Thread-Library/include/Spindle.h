#include "PoolManager.h"


class Spindle 
{
private:
    std::unique_ptr<PoolManager> threadPoolPtr;
    static Spindle* spindle;  
    Spindle(/* args */);
public:
    ~Spindle();
    static Spindle* getInstance();
    template<typename T>
    bool AddProcess(T* funcPtr);
};

Spindle *Spindle::spindle = nullptr;