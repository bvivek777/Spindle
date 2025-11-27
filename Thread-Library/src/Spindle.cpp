#include "../include/Spindle.h"
#include <algorithm>
#include <iostream>
#include <thread>
#include <chrono>
#include <sstream>

int Spindle::hwThreads = 0;
int Spindle::currentThreads = 0;
std::atomic<ll> Spindle::processCounter(0);
std::atomic<bool> Spindle::flag(false);

Spindle::Spindle(Config* configuration) {
    config = configuration;
    hwThreads = std::thread::hardware_concurrency() - 1;
    if (hwThreads <= 0) {
        hwThreads = 1; // Ensure at least one thread
    }
    functionThreadMapperCollection.clear();
    currentThreads = 0;
    processCounter = 0;
    flag = false;
}

Spindle::~Spindle() {
    // Set flag to stop accepting new work
    flag = false;
    
    // Wait for all threads to finish their current work and exit
    for(int i = 0; i < currentThreads; i++){
        auto it = idThreadMap.find(i);
        if (it != idThreadMap.end()) {
            it->second->wait();
        }
    }
    
    // Clear the map
    idThreadMap.clear();
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
            
            // Parse ML data more robustly
            if (!mlData.empty()) {
                std::istringstream iss(mlData);
                int value;
                while (iss >> value) {
                    functionThreadMapperCollection.push_back(value);
                }
            }
            
            if (functionThreadMapperCollection.empty()) {
                // Fallback to default if no ML data provided
                functionThreadMapperCollection.resize(threads, 1);
            }
            
            int numThreads = functionThreadMapperCollection.empty() ? 1 :
                *std::max_element(functionThreadMapperCollection.begin(), 
                    functionThreadMapperCollection.end());
            
            if (numThreads <= 0) {
                numThreads = 1;
            }
            
            if (!createThreads(numThreads)) {
                return false;
            }
            currentThreads = numThreads;
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
    if ( !flag || currentThreads == 0 ) {
        return false;
    }
    if (funcPtr == nullptr) {
        return false;
    }
    // Atomically get and increment the counter to avoid race conditions
    ll currentId = processCounter.fetch_add(1);
    int threadIndex = currentId % currentThreads;
    auto it = idThreadMap.find(threadIndex);
    if (it == idThreadMap.end()) {
        return false;
    }
    it->second->addToQueue(funcPtr, currentId);
    return true;
}

bool Spindle::assignML(void (*funcPtr)())
{
    if ( !flag ) {
        return false;
    }
    // Atomically get and increment the counter to avoid race conditions
    ll currentId = processCounter.fetch_add(1);
    if (functionThreadMapperCollection.empty() || 
        static_cast<size_t>(currentId) >= functionThreadMapperCollection.size()) {
        return false;
    }
    int threadId = functionThreadMapperCollection[currentId];
    if (threadId < 1 || threadId > currentThreads) {
        return false;
    }
    auto it = idThreadMap.find(threadId - 1);
    if (it == idThreadMap.end()) {
        return false;
    }
    it->second->addToQueue(funcPtr, currentId); 
    return true;
}

bool Spindle::createThreads(int threadC) {
    if (threadC <= 0 || threadC > hwThreads) {
        return false;
    }
    // Clear existing threads if any
    idThreadMap.clear();
    currentThreads = threadC;
    
    for(int i = 0; i < threadC; i++){
        try {
            auto p = std::make_shared<Thread>();
            idThreadMap.insert({i, p});
        } catch (const std::exception& e) {
            // Cleanup on failure
            idThreadMap.clear();
            currentThreads = 0;
            return false;
        }
    }
    flag = true;
    return true;
}

bool Spindle::done() {
    flag = false;
    return flag;
}