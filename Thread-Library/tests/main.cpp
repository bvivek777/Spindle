#include "../include/Spindle.h"
#include "../include/ThreadConfig.h"
#include <iostream>


#define _TRAINING_ true

void print()
{
    std::cout<< "Hello World";
}

void DoSomeWorkFor2Secs()
{
    //Sleep(2000);
}

int main(int argc, char* argv[])
{
    Config& config = Config::getInstance(RUN_MODE::PERFORMANCE, THREAD_MODE::POOL);

    Spindle& spindle = Spindle::getInstance(&config);
    spindle.addProcess(&print);
    spindle.addProcess(&DoSomeWorkFor2Secs);
} 