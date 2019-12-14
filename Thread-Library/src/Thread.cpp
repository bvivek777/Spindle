#include "../include/Thread.h"

Thread::Thread() {
    //std::cout<<"start - Thread::Thread()\n";
    processPool = new TsQueue<FunctionToId>();
    threadState = THREAD_STATE::INIT;
    inScope = true;
    processAssignedWork();
    //std::cout<<"end - Thread::Thread()\n";
}

Thread::~Thread(){   
    //std::cout<<"start - Thread::~Thread()\n";      
    inScope = false;
    thread.join();
    delete processPool;
    //std::cout<<"end - Thread::~Thread()\n";  
}

bool Thread::addToQueue(void (*funcPtr)(), ll processId) {
    //std::cout<<"start - Thread::addToQueue\n";
    FunctionToId funcId(funcPtr,processId);
    processPool->pushBack(funcId);
    queueConditionVariable.notify_all();
    //std::cout<<"end - Thread::addToQueue\n";
    return true;
}

void Thread::processAssignedWork() {
    //std::cout<< "start - Thread::processAssignedWork - poolSize : "<<processPool->size()<<std::endl;
    thread = std::thread([this] {
        FunctionToId func;
        std::chrono::_V2::system_clock::time_point startTime, endTime;
        std::chrono::duration<double> runtime;
        while( inScope ) {
            while(processPool->size() == 0);
            /* 
             * std::unique_lock<std::mutex> lckgd(queueMutex);
             * queueConditionVariable.wait(lckgd, [&] {return processPool->size()!=0 ;});            
             */
            //std::cout<<"Thread::processAssignedWork - in new thread_poolSize : "<<processPool->size()<<"\n";
            func = processPool->popFront();
            //std::cout<<"Thread::processAssignedWork - function_id : "<<func.id<<"\n";
            try {
                startTime = std::chrono::high_resolution_clock::now();
                (func.funcPtr)();
                endTime = std::chrono::high_resolution_clock::now();
                runtime = std::chrono::duration_cast<std::chrono::duration<double>>(endTime - startTime);
                std::cout<<" runtime for func :"<<func.id<<" - "<<runtime.count()<<"\n";
            }
            catch(const std::exception& e) {
                std::cerr << e.what() << '\n';
            }            
        }
    });
    //std::cout<< "end - Thread::processAssignedWork - poolSize : "<<processPool->size()<<std::endl;
}

bool Thread::wait() {
    if ( processPool->size() == 0 ) {
        inScope = false;
        return true;
    }
    return false;
}

int Thread::isPending() {
    return processPool->size() > 0;
}

THREAD_STATE Thread::getRunningState(){
    return threadState;
}