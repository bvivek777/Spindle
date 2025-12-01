#ifndef __DATA_COLLECTOR__
#define __DATA_COLLECTOR__

#include <algorithm>
#include <fstream>
#include <mutex>
#include <string>
#include <vector>
#include "ThreadConfig.h"

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
    double runTime;
    RuntimeData(){}
    RuntimeData(ll f, double r){
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
    Config* config;
    ll difference(std::string, std::string);
public:
    DataCollector(Config *config);
    DataCollector() = default;
    ~DataCollector();
    bool captureCPUSnapshot();
    bool writeToFile();
    bool writeSnapshotToFile();
    bool captureRuntime(ll funcNum, double runtime);
    bool writeRumTimeToFile();
    bool ifLog();
};
#endif