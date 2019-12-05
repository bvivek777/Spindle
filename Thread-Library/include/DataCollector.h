#ifndef __DATA_COLLECTOR__
#define __DATA_COLLECTOR__

#include <mutex>
#include <fstream>

class DataCollector {
private:
    std::mutex lock;
    std::ofstream file;    
    DataCollector();
public:
    ~DataCollector();
};
#endif