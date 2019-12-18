#include "../include/Spindle.h"
#include <algorithm>
#include <iostream>

int Spindle::hwThreads = 0;
int Spindle::currentThreads = 0;
std::atomic<ll> Spindle::processCounter(0);
std::atomic<bool> Spindle::flag(false);

Spindle::Spindle(Config* configuration) {
    //std::cout<<"start - Spindle::Spindle()\n";
    config = configuration;
    hwThreads = std::thread::hardware_concurrency() - 1;
    functionThreadMapperCollection.clear();
    currentThreads = 0;
    processCounter = 0;
    flag = false;
    //std::cout<<"end - Spindle::Spindle()\n";
}

Spindle::~Spindle() {
    //std::cout<<"start - Spindle::~Spindle()\n";
    bool condition = true;
    threadPtr temp;
    while ( flag || condition ) {
        condition = false;
        for(int i = currentThreads-1; i >= 0 && flag; i--){
           temp = idThreadMap.find(i)->second;
           condition = condition | temp->isPending();
        }
    }
    for(int i = currentThreads-1; i >= 0; i--){
        temp = idThreadMap.find(i)->second;
        temp->wait();
    }
    //std::cout<<"end - Spindle::~Spindle()\n";
}

Spindle& Spindle::getInstance(Config* configuration){
    //std::cout<<"start - Spindle::getInstance()\n";
    static Spindle spindle(configuration);
    //std::cout<<"end - Spindle::getInstance()\n";
    return spindle; 
}

bool Spindle::init(int threads){
    //std::cout<<"start - Spindle::init()\n";
    THREAD_MODE curMode = config->getThreadMode();
    //std::cout<<"Spindle::init(), threads : "<<threads<<"\n";
    switch (curMode)
    {
    case THREAD_MODE::POOL:
        //std::cout<<"Spindle::init() POOL MODE, start createThreads()\n";
        createThreads(hwThreads);
        //std::cout<<"Spindle::init() POOL MODE, end createThreads()\n";
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
            //std::cout<<"ML Mode\n" << numThreads;
            createThreads(numThreads);
            currentThreads = numThreads;
            //std::cout<<"ML Mode Threads created\n";
        }
        break;
    case THREAD_MODE::CONSTANT:
        if ( threads <= 0 || threads > hwThreads)
            return false;
        //std::cout<<"Spindle::init() CONSTANT, start createThreads()\n";
        createThreads(threads);
        //std::cout<<"Spindle::init() CONSTANT, end createThreads()\n";
        currentThreads = threads;
        break;
    default:
        return false;
        break;
    }
    return true;
}

bool Spindle::addProcess(void (*functPtr)()) {
    if ( !flag ) {
        //std::cout<<"Spindle::addProcess() - Call init first\n";
        return false;
    }
    SCHEDULING scheduling = Config::getInstance().getSchedulingType();
    switch (scheduling)
    {
    case SCHEDULING::FCFS_SC :
        //std::cout<<"Spindle::addProcess(), start assignFCFS\n";
        assignFCFS(functPtr);        
        //std::cout<<"Spindle::addProcess(), end assignFCFS\n";
        break;
    case SCHEDULING::ML :
        //std::cout<<"Spindle::addProcess(), start assignML\n";
        assignML(functPtr);
        //std::cout<<"Spindle::addProcess(), end assignML\n";
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
    //std::cout<<" start - Spindle::assignFCFS()\n";
    if ( !flag ) {
        //std::cout<<"Spindle::assignFCFS() - Call init first\n";
        return false;
    }
    idThreadMap.at( ( processCounter % currentThreads ) )->addToQueue(funcPtr,processCounter);
    processCounter++;
    //std::cout<<" end - Spindle::assginFCFS(), processCount : "<<processCounter<<"\n";
    return true;
}

bool Spindle::assignML(void (*funcPtr)())
{
    //std::cout<<" start - Spindle::assignML()"<<"\n";
    if ( !flag ) {
        //std::cout<<"Spindle::assignML() - Call init first\n";
        return false;
    }
    int threadId = functionThreadMapperCollection[processCounter];
    idThreadMap[threadId - 1]->addToQueue(funcPtr, threadId); 
    processCounter++;
    flag = true;
    //std::cout<<" end - Spindle::assignML()"<<"\n";
    return true;
}

bool Spindle::createThreads(int threadC) {
    //std::cout<<" start - Spindle::createThreads() threadCount = "<<threadC<<"\n";
    currentThreads = threadC;
    for(int i=0; i < threadC; i++){
        auto p = std::make_shared<Thread>();
        idThreadMap.insert({i,p});
    }
    flag = true;
    //std::cout<<" end - Spindle::createThreads()\n";
    return true;
}

bool Spindle::done() {
    flag = false;
    return flag;
}