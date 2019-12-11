#include "../include/Spindle.h"

int Spindle::hwThreads = 0;
int Spindle::currentThreads = 0;
std::atomic<ll> Spindle::processCounter(0);
std::atomic<bool> Spindle::flag(false);

Spindle::Spindle(Config* configuration = nullptr) {
    std::cout<<"Spindle init complete\n";
    hwThreads = std::thread::hardware_concurrency() - 1;
    currentThreads = 0;
    processCounter = 0;
    flag = false;
}

Spindle::~Spindle() {
}

Spindle& Spindle::getInstance(Config* configuration){
    static Spindle spindle(configuration);
    std::cout<<"spindle instance returned\n";
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
    std::cout<<"Init called with "<<threads<<" threads executing\n";
    switch (curMode)
    {
    case THREAD_MODE::POOL:
        std::cout<<"Pool Mode\n";
        createThreads(hwThreads);
        std::cout<<"Threads created\n";
        currentThreads = threads;
        break;
    case THREAD_MODE::SPINDLE:
        // Machine Learning Values come here
        { 
            int someValueReturnedByML = 0;
            std::cout<<"ML Mode\n";
            createThreads(someValueReturnedByML);
            std::cout<<"Threads created\n";
        }
        break;
    case THREAD_MODE::CONSTANT:
        if ( threads <= 0 || threads > hwThreads)
            return false;
        std::cout<<"Constant mode\n";
        createThreads(threads);
        std::cout<<"Threads Created\n";
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
        std::cout<<"FCFS running\n";
        assignFCFS(functPtr);        
        std::cout<<"func assigned\n";
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
    std::cout<<"assigning to FCFS\n";
    idThreadMap.at( ( processCounter % currentThreads ) )->addToQueue(funcPtr,processCounter);
    processCounter++;
    std::cout<<"added and incremented counter\n";
    return true;
}

bool Spindle::assignML()
{
    return false;
}

bool Spindle::createThreads(int threadCount){
    std::cout<<"Creating Threads\n";
    for(int i=0; i < threadCount; i++){
        auto p = std::make_shared<Thread>();
        idThreadMap.insert({i,p});
    }
    flag = true;
    std::cout<<"Threads Created\n";
    return true;
}