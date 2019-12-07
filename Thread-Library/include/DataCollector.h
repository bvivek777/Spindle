#ifndef __DATA_COLLECTOR__
#define __DATA_COLLECTOR__

#include <mutex>
#include <fstream>
#include <vector>
#include <algorithm>

typedef long long ll;

struct ProcData{
    std::string swapTotal;
    std::string swapFree;
    std::string memTotal;
    std::string memFree;
    std::string cpuUtilization;
    std::string contextSwitches;
};

class DataCollector {
private:
    std::mutex mutex;
    std::vector<ProcData> snapshots;
    std::ofstream file;  
    std::ifstream stat;  
    DataCollector();
    ll difference(std::string, std::string);
public:
    ~DataCollector();
    bool captureSnapshot();
    bool write();
};
#endif