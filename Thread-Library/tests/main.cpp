#include "../include/Spindle.h"
#include "../include/ThreadConfig.h"
#include <iostream>
#include <fstream>

#define _TRAINING_ true


void fileOperations() {
   /* std::ifstream file;
   std::string readLine;
   file.open("/home/bvivek2/Projects/permute/file.txt");
   while( std::getline(file,readLine) ){

   } */
}

void memoryOperations() {
   int *n = new int[100000];
   delete n;
}

void arithmeticOperations() {
   int a = 25;
   for(int i=0; i<100; i++){
      a++;
   }
}

void contextSwitch() {
   std::cout<< "fourth function\n";
}

int main(int argc, char* argv[])
{
   Config& config = Config::getInstance(RUN_MODE::PERFORMANCE, THREAD_MODE::CONSTANT);
   config.setThreadMode(THREAD_MODE::CONSTANT);
   config.setSchedulingType(SCHEDULING::FCFS_SC);
   Spindle& spindle = Spindle::getInstance(&config);
   spindle.init(2);
   spindle.addProcess(&contextSwitch);
   spindle.addProcess(&fileOperations);
   spindle.addProcess(&memoryOperations);
   spindle.done();
} 