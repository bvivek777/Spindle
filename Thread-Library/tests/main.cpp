#include "../include/Spindle.h"
#include "../include/ThreadConfig.h"
#include <iostream>


#define _TRAINING_ true

void print1() {
   std::cout<< "First function\n";
}

void print2() {
   std::cout<< "Second function\n";
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
    int i = 1;
    /* while ( i ){
       spindle.addProcess(&print1);
       std::cin>>i;
    } */
    spindle.addProcess(&print1);
    spindle.addProcess(&print2);
    spindle.addProcess(&print3);
    spindle.addProcess(&print4);
    spindle.addProcess(&print5);
} 