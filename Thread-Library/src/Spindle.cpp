#include "Spindle.h"

Spindle::Spindle(/* args */) {
    threadPoolPtr = std::make_unique<PoolManager>();
}

Spindle::~Spindle() {
}

Spindle* Spindle::getInstance(){
    if ( !spindle )
        spindle = new Spindle;
    return spindle; 
}

template<typename T>
bool Spindle::AddProcess(T* funcPtr)
{
    return threadPoolPtr->addProcess(funcPtr);
}