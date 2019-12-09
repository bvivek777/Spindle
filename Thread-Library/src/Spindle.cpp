#include "Spindle.h"

Spindle::Spindle(Config* configuration = nullptr) {
    hwThreads = std::thread::hardware_concurrency();
}

Spindle::~Spindle() {
}

Spindle& Spindle::getInstance(Config* configuration){
    static Spindle spindle(configuration);
    return spindle; 
}

template<typename T>
bool Spindle::addProcess(T* funcPtr) {
    THREAD_MODE curMode = Config::getInstance().getThreadMode();
    switch (curMode)
    {
    case THREAD_MODE::POOL:
        addProcess(funcPtr,hwThreads);
        /* code */
        break;
    case THREAD_MODE::SPINDLE:
        // Machine Learning Values come here
        int someValueReturnedByML = 0;
        createThreads(someValueReturnedByML);

        /* code */
        break;
    case THREAD_MODE::CONSTANT:
        addProcess(funcPtr,hwThreads);
        break;
    default:
        return false;
        break;
    }
    return true;
}

template<typename T>
bool Spindle::addProcess(T* funcPtr, int threadCount){
    createThreads(hwThreads);
    return true;
}

bool Spindle::createThreads(int threadCount){
    for(int i=0; i < threadCount; i++){
        auto p = std::make_shared<Thread>(new Thread());
        idThreadMap.insert({i,p});
    }
    return true;
}