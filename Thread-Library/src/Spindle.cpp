#include "../include/Spindle.h"

int Spindle::hwThreads = 0;
int Spindle::currentThreads = 0;
std::atomic<ll> Spindle::processCounter = 0;
std::atomic<bool> Spindle::flag = false;

Spindle::Spindle(Config* configuration = nullptr) {
    hwThreads = std::thread::hardware_concurrency() - 1;
    currentThreads = 0;
    processCounter = 0;
    flag = false;
}

Spindle::~Spindle() {
}

Spindle& Spindle::getInstance(Config* configuration){
    static Spindle spindle(configuration);
    return spindle; 
}

void Spindle::setFlag()
{
}

void Spindle::getExecutionState()
{
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
        { 
            int someValueReturnedByML = 0;
            createThreads(someValueReturnedByML);
        }
        break;
    case THREAD_MODE::CONSTANT:
        if ( threads <= 0 || threads > hwThreads)
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

bool Spindle::addProcess(void (*functPtr)()) {
    SCHEDULING scheduling = Config::getInstance().getSchedulingType();
    switch (scheduling)
    {
    case SCHEDULING::FCFS_SC :
        //assignFCFS(functPtr);
        break;
    case SCHEDULING::ML :
        assignML();
        break;
    case SCHEDULING::RR_SC :
        /* code */
        break;
    default:
        return false;
        break;
    }
    return true;
}

bool Spindle::assignFCFS(void (*funcPtr)()){
    if ( !flag )
        return false;
    idThreadMap.at( ( processCounter % currentThreads ) )->addToQueue(funcPtr,processCounter);
    processCounter++;
    return true;
}

bool Spindle::assignML()
{
    return false;
}

bool Spindle::createThreads(int threadCount){
    for(int i=0; i < threadCount; i++){
        auto p = std::make_shared<Thread>();
        idThreadMap.insert({i,p});
    }
    flag = true;
    return true;
}