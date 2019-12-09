#include "Spindle.h"

Spindle::Spindle(Config* configuration = nullptr) {
    hwThreads = std::thread::hardware_concurrency();
    currentThreads = 0;
}

Spindle::~Spindle() {
}

Spindle& Spindle::getInstance(Config* configuration){
    static Spindle spindle(configuration);
    return spindle; 
}

bool Spindle::init(int threads){
    THREAD_MODE curMode = Config::getInstance().getThreadMode();
    switch (curMode)
    {
    case THREAD_MODE::POOL:
        createThreads(hwThreads);
        currentThreads = threads;
        break;
    case THREAD_MODE::SPINDLE:
        // Machine Learning Values come here
        int someValueReturnedByML = 0;
        createThreads(someValueReturnedByML);
        break;
    case THREAD_MODE::CONSTANT:
        if ( threads <= 0 )
            return false;
        createThreads(threads);
        currentThreads = threads;
        break;
    default:
        return false;
        break;
    }
    return true;
}

template<typename T>
bool Spindle::addProcess(T* funcPtr) {
    if (currentThreads   == 0)
        return false;
    return true;
}

bool Spindle::createThreads(int threadCount){
    for(int i=0; i < threadCount; i++){
        auto p = std::make_shared<Thread>(new Thread());
        idThreadMap.insert({i,p});
    }
    return true;
}