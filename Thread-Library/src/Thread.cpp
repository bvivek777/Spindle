#include "../include/Thread.h"

Thread::Thread() {
    //std::cout<<"Thread class Created\n";
    processPool = new TsQueue<FunctionToId>();
    threadState = THREAD_STATE::INIT;
    inScope = true;
    processAssignedWork();
}

Thread::~Thread(){      
   // inScope = false;  
    queueConditionVariable.notify_all();
    if(thread.joinable())
        thread.join();
    //std::cout<<"Thread class destructor\n";
    delete processPool;  
}

bool Thread::addToQueue(void (*funcPtr)(), ll processId)
{
    //std::cout<<"adding process to queue\n";
    {
        std::lock_guard<std::mutex> lckgd(queueMutex);
        if(threadState == THREAD_STATE::INIT)
            threadState == THREAD_STATE::RUNNING;
        FunctionToId funcId(funcPtr,processId);
        processPool->pushBack(funcId);
    }
    //std::cout<<"added to queue\n";
    queueConditionVariable.notify_all();
    return true;
}

void Thread::processAssignedWork() {
    //std::cout<< "Inside processAssignment"<<std::endl;
    //std::cout<<"Pool Size : "<<processPool->size()<<"\n";
    thread = std::thread([this]
    {
        for(;;) {
        std::unique_lock<std::mutex> lckgd(queueMutex);
        queueConditionVariable.wait(lckgd, [&] {return !processPool->empty() + !(threadState == THREAD_STATE::RUNNING);});
        //std::chrono::_V2::system_clock::time_point startTime, endTime;
        //std::cout<<"Entered thread exec\n";
        FunctionToId func;
        ll runTime;
        if( threadState != THREAD_STATE::RUNNING ) {
            threadState = THREAD_STATE::RUNNING;
                while(!processPool->empty()) {    
                    //std::cout<<"func in thread\n";            
                    //startTime = std::chrono::high_resolution_clock::now();
                    func = processPool->popBack();             
                    try
                    {
                        //std::cout<<"about to exec func:"<<func.id<<"  "<<"\n";
                        (func.funcPtr)();
                    }
                    catch(const std::exception& e)
                    {
                        std::cerr << e.what() << '\n';
                    }
                    //std::cout<<"func finished exec\n";
                    //endTime = std::chrono::high_resolution_clock::now();
                    //runTime = std::chrono::duration_cast<std::chrono::nanoseconds>(startTime-endTime).count();
                }
            threadState = THREAD_STATE::FINISHED;
        }
        }
    });
}

bool Thread::notify()
{
    return false;
}

THREAD_STATE Thread::getRunningState(){
    return threadState;
}