#include "../include/Spindle.h"
#include <algorithm>
#include <iostream>

int Spindle::hwThreads = 0;
int Spindle::currentThreads = 0;
std::atomic<ll> Spindle::processCounter(0);
std::atomic<bool> Spindle::flag(false);

Spindle::Spindle(Config* configuration) {
    //std::cout<<"Spindle init complete\n";
    config = configuration;
    hwThreads = std::thread::hardware_concurrency() - 1;
    functionThreadMapperCollection.clear();
    currentThreads = 0;
    processCounter = 0;
    flag = false;
}

Spindle::~Spindle() {
    //delete config;
}

Spindle& Spindle::getInstance(Config* configuration){
    static Spindle spindle(configuration);
    //std::cout<<"spindle instance returned\n";
    return spindle; 
}

void Spindle::setFlag()
{
}

void Spindle::getExecutionState() {

}

bool Spindle::init(int threads){
    THREAD_MODE curMode = config->getThreadMode();
    //std::cout<<"Init called with "<<threads<<" threads executing\n";
    switch (curMode)
    {
    case THREAD_MODE::POOL:
        //std::cout<<"Pool Mode\n";
        createThreads(hwThreads);
        //std::cout<<"Pool Threads created\n";
        currentThreads = threads;
        break;
    case THREAD_MODE::SPINDLE:
        // Machine Learning Values come here
        {
            functionThreadMapperCollection.clear();
            std::string mlData;
            std::getline(std::cin, mlData);
            for (auto& c : mlData)
            {
                functionThreadMapperCollection.push_back(std::atoi(&c));
            }
            int numThreads = *std::max_element(functionThreadMapperCollection.begin(), 
                functionThreadMapperCollection.end());
            std::cout<<"ML Mode\n" << numThreads;
            createThreads(numThreads);
            currentThreads = numThreads;
            //std::cout<<"ML Mode Threads created\n";
        }
        break;
    case THREAD_MODE::CONSTANT:
        if ( threads <= 0 || threads > hwThreads)
            return false;
        //std::cout<<"Constant mode\n";
        createThreads(threads);
        //std::cout<<"Const Threads Created\n";
        currentThreads = threads;
        break;
    default:
        return false;
        break;
    }
    return true;
}

bool Spindle::addProcess(void (*functPtr)()) {
    SCHEDULING scheduling = config->getSchedulingType();
    switch (scheduling)
    {
    case SCHEDULING::FCFS_SC :
        std::cout<<"FCFS running\n";
        assignFCFS(functPtr);        
        //std::cout<<"func assigned\n";
        break;
    case SCHEDULING::ML :
        std::cout<<"ML running\n";
        assignML(functPtr);
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
    //std::cout<<"assigning to FCFS\n";
    idThreadMap.at(( processCounter % currentThreads ) )->addToQueue(funcPtr,processCounter);
    processCounter++;
    //std::cout<<"added and incremented counter\n";
    return true;
}

bool Spindle::assignML(void (*funcPtr)())
{
    int threadId = functionThreadMapperCollection[processCounter];
    idThreadMap[threadId - 1]->addToQueue(funcPtr, threadId); 
    processCounter++;
    flag = true;
    return true;
}

bool Spindle::createThreads(int threadC){
    //std::cout<<"Creating Threads"<<threadC<<"\n";
    for(int i=0; i < threadC; i++){
        auto p = std::make_shared<Thread>();
        idThreadMap.insert({i,p});
    }
    flag = true;
    //std::cout<<"Threads Created\n";
    return true;
}