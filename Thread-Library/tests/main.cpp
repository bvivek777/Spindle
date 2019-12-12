#include "../include/Spindle.h"
#include "../include/ThreadConfig.h"
#include <iostream>
#include <time.h>
#include <unistd.h>

#define _TRAINING_ true

void print1(int i) {
   std::cout<<i<<" First function\n";
}

void print2() {
   std::cout<<"Second function : "<<rand()<<"\n";
   //std::chrono::milliseconds timespan(10000);
   //std::this_thread::sleep_for(timespan);
}

void print3() {
   std::cout<< "third function\n";
}

void print4() {
   std::cout<< "fourth function\n";
}

void print5() {
   std::cout<< "fifth function\n";
}


int main(int argc, char* argv[])
{
    Config& config = Config::getInstance(RUN_MODE::PERFORMANCE, THREAD_MODE::CONSTANT);
    //std::cout<<config.getThreadMode()<<"\n";
    config.setThreadMode(THREAD_MODE::CONSTANT);
    //std::cout<<config.getThreadMode()<<"\n";
    Spindle& spindle = Spindle::getInstance(&config);
    spindle.init(2);
    srand(time(0));
    /* for(int i=0; i<10; i++){
       spindle.addProcess(&print2);
    } */
    spindle.addProcess(&print2);
    spindle.addProcess(&print2);
    spindle.done();
    /* spindle.addProcess(&print1);
    spindle.addProcess(&print2);
    spindle.addProcess(&print3);
    spindle.addProcess(&print4);
    spindle.addProcess(&print5); */
} 