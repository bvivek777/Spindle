#include "../include/DataCollector.h"
#include <string>

DataCollector::DataCollector(Config* configuration) {
    config = configuration;
}

ll DataCollector::difference(std::string total, std::string free){
    try {
        // More robust parsing: find the number after the colon
        size_t totalPos = total.find(':');
        size_t freePos = free.find(':');
        
        if (totalPos == std::string::npos || freePos == std::string::npos) {
            return 0;
        }
        
        // Skip whitespace after colon
        totalPos = total.find_first_not_of(" \t", totalPos + 1);
        freePos = free.find_first_not_of(" \t", freePos + 1);
        
        if (totalPos == std::string::npos || freePos == std::string::npos) {
            return 0;
        }
        
        // Find the end of the number (whitespace or end of string)
        size_t totalEnd = total.find_first_of(" \t\n", totalPos);
        size_t freeEnd = free.find_first_of(" \t\n", freePos);
        
        std::string totalStr = (totalEnd == std::string::npos) ? 
            total.substr(totalPos) : total.substr(totalPos, totalEnd - totalPos);
        std::string freeStr = (freeEnd == std::string::npos) ? 
            free.substr(freePos) : free.substr(freePos, freeEnd - freePos);
        
        ll totalVal = std::stoll(totalStr);
        ll freeVal = std::stoll(freeStr);
        
        return totalVal - freeVal;
    } catch (const std::exception& e) {
        return 0;
    }
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
        file.open("../data/data.csv", std::ios::out | std::ios::trunc);
        if (!file.is_open()) {
            return false;
        }
        
        // Reserve string capacity for better performance
        std::string writeString;
        writeString.reserve(256); // Reserve space for typical line
        
        for (const ProcData& snapshot : snapshots) {
            if (snapshot.cpuUtilization.length() > 5) {
                writeString.clear();
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
    std::lock_guard<std::mutex> lock(mutex);
    try {
        stat.open("/proc/stat");
        if (!stat.is_open()) {
            return false;
        }
        
        std::string readLine;
        readLine.reserve(256); // Reserve capacity for better performance
        ProcData snapshot;
        int count = 2;  // Number of features to be read
        
        while( std::getline(stat,readLine) && count > 0 ) {
            if (readLine.length() < 3) continue;
            
            // Match original behavior: check if 2nd char is 'p' and 3rd is 'u' (matches "cpu" lines)
            // This handles both "cpu" and "cpu0", "cpu1", etc.
            if (readLine.length() > 2 && readLine[1] == 'p' && readLine[2] == 'u' && count > 1) {
                snapshot.cpuUtilization = readLine;
                count--;
            }
            // Match "ctxt" line (context switches)
            else if (readLine.length() > 3 && readLine[2] == 'x' && readLine[3] == 't') {
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
            
            // Use string comparison for better performance and readability
            if (readLine.substr(0, 9) == "SwapTotal") {
                snapshot.swapTotal = readLine;
                count--;
            }
            else if (readLine.substr(0, 8) == "SwapFree") {
                snapshot.swapFree = readLine;
                count--;
            }
            else if (readLine.substr(0, 8) == "MemTotal") {
                snapshot.memTotal = readLine;
                count--;
            }
            else if (readLine.substr(0, 7) == "MemFree") {
                snapshot.memFree = readLine;
                count--;
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

bool DataCollector::writeToFile(){
    // This method appears to be a placeholder - implement if needed
    return writeSnapshotToFile();
}

bool DataCollector::writeRumTimeToFile(){
    if (config->getInstance().getRunningMode() != RUN_MODE::COLLECT_DATA) {
        return false;
    }
    
    try {
        std::ofstream runtimeFile("../data/runtime.csv", std::ios::out | std::ios::trunc);
        if (!runtimeFile.is_open()) {
            return false;
        }
        
        // Write header
        runtimeFile << "FunctionId,Runtime\n";
        
        for (const RuntimeData& data : runTimeData) {
            runtimeFile << data.funcId << "," << data.runTime << "\n";
        }
        
        runtimeFile.close();
        return true;
    } catch (const std::exception& e) {
        return false;
    } catch (...) {
        return false;
    }
}