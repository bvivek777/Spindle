#include "../include/DataCollector.h"
#include <string>

DataCollector::DataCollector(Config* configuration) {
    config = configuration;
}

ll DataCollector::difference(std::string total, std::string free){
    return std::stoll(total.substr(14,10)) - std::stoll(free.substr(14,10));
}

DataCollector::~DataCollector() {
    
}

bool DataCollector::captureRuntime (ll funcId, double runTime) {
    if ( config->getInstance().getRunningMode() != RUN_MODE::COLLECT_DATA )
        return false;
    RuntimeData r(funcId,runTime);
    runTimeData.push_back(r);
    return true;
}

bool DataCollector::writeSnapshotToFile(){
    try {
        std::remove("../data/data.csv");
        file.open("../data/data.csv");
        if (!file.is_open()) {
            return false;
        }
        std::string writeString;
        for (const ProcData& snapshot : snapshots) {
            if (snapshot.cpuUtilization.length() > 5) {
                writeString = snapshot.cpuUtilization.substr(5);
                std::replace(
                    writeString.begin(), 
                    writeString.end(),
                    ' ',
                    ','
                );
                writeString += ",";
                writeString += std::to_string(difference(snapshot.memTotal,snapshot.memFree));
                writeString += ",";
                writeString += std::to_string(difference(snapshot.swapTotal,snapshot.swapFree));
                writeString += "\n";
                file << writeString;
            }
        }
        file.close();
        return true;
    } catch (const std::exception& e) {
        if (file.is_open()) {
            file.close();
        }
        return false;
    } catch (...) {
        if (file.is_open()) {
            file.close();
        }
        return false;
    }
}

bool DataCollector::captureCPUSnapshot(){
    auto lock = std::unique_lock<std::mutex>(mutex);
    try {
        stat.open("/proc/stat");
        if (!stat.is_open()) {
            return false;
        }
        
        std::string readLine;
        ProcData snapshot;
        int count = 2;  // Number of features to be read
        
        while( std::getline(stat,readLine) && count > 0 ) {
            if (readLine.length() < 3) continue;
            
            if ( readLine.at(1) == 'p' && readLine.at(2) == 'u' && count > 1 ) {
                snapshot.cpuUtilization = readLine;
                count--;
            }
            else if (readLine.length() > 3 && readLine.at(2) == 'x' && readLine.at(3) == 't' ) {
                snapshot.contextSwitches = readLine;                   
                count--;
            }
        }
        stat.close();
        
        count = 4;  // 4 features need to be read
        stat.open("/proc/meminfo");
        if (!stat.is_open()) {
            return false;
        }
        
        while ( std::getline(stat,readLine) && count > 0 ) {
            if (readLine.length() < 5) continue;
            
            if ( readLine.at(0) == 'S' && readLine.at(1) == 'w' ) {
                if ( readLine.at(4) == 'T' ) {
                    snapshot.swapTotal = readLine;
                    count--;
                }
                else if ( readLine.at(4) == 'F' ) {
                    snapshot.swapFree = readLine;
                    count--;
                }
            }
            else if ( readLine.at(0) == 'M' && readLine.at(1) == 'e' ) {
                if (readLine.length() > 3) {
                    if ( readLine.at(3) == 'T' ) {
                        snapshot.memTotal = readLine;
                        count--;
                    }
                    else if ( readLine.at(3) == 'F' ) {
                        snapshot.memFree = readLine;
                        count--;
                    }
                }
            }
        }
        stat.close();
        snapshots.push_back(snapshot);
        return true;
    } catch (const std::exception& e) {
        if (stat.is_open()) {
            stat.close();
        }
        return false;
    } catch (...){
        if (stat.is_open()) {
            stat.close();
        }
        return false;
    }
}

bool DataCollector::ifLog(){
    return config->getInstance().getRunningMode() == RUN_MODE::COLLECT_DATA;
}