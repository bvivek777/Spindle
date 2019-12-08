#include "include/PoolManager.h"
#include <iostream>
#include <Windows.h>

//#define _TRAINING_ true

void print()
{
    std::cout<< "Hello World";
}

void DoSomeWorkFor2Secs()
{
    Sleep(2000);
}

int main(int argc, char* argv[])
{
    std::unique_ptr<PoolManager> plManager = std::make_unique<PoolManager>(plManager->getInstance());
    plManager->AddProcess(&print);
    plManager->AddProcess(&DoSomeWorkFor2Secs);
}