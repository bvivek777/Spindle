#ifndef __DATA_COLLECTOR__
#define __DATA_COLLECTOR__

#include <mutex>
#include <fstream>
#include <stdio.h>

class DataCollector {
private:
    std::mutex lock;
    std::ofstream file;    
    DataCollector();
public:
    ~DataCollector();
    bool write();
};
#endif