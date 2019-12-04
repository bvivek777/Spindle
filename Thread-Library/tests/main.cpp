#include "include/PoolManager.h"
#include <iostream>

void print()
{
    std::cout<< "Hello World";
}

int main(int argc, char* argv[])
{
    std::unique_ptr<PoolManager> plManager = std::make_unique<PoolManager>(plManager->getInstance());
    plManager->AddProcess(&print);

}