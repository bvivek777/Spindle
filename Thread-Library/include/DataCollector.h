#ifndef __DATA_COLLECTOR__
#define __DATA_COLLECTOR__

#include <mutex>
#include <fstream>
#include <vector>
#include <algorithm>
#include "Config.h"

typedef long long ll;

struct ProcData{
    std::string swapTotal;
    std::string swapFree;
    std::string memTotal;
    std::string memFree;
    std::string cpuUtilization;
    std::string contextSwitches;
};

struct RuntimeData {
    ll funcId;
    ll runTime;
    RuntimeData(){}
    RuntimeData(ll f, ll r){
        funcId = f;
        runTime = r;
    }
};

class DataCollector {
private:
    std::mutex mutex;
    std::vector<ProcData> snapshots;
    std::vector<RuntimeData> runTimeData;
    std::ofstream file;  
    std::ifstream stat;  
    DataCollector();
    ll difference(std::string, std::string);
public:
    ~DataCollector();
    bool captureCPUSnapshot();
    bool writeToFile();
    bool writeSnapshotToFile();
    bool captureRuntime(ll funcNum, ll runtime);
    bool writeRumTimeToFile();
};
#endif