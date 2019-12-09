#include "DataCollector.h"

DataCollector::DataCollector(/* args */) {
    
}

ll DataCollector::difference(std::string total, std::string free){
    return std::stoll(total.substr(14,10)) - std::stoll(free.substr(14,10));
}

DataCollector::~DataCollector() {
    try {
        std::remove("../data/data.csv");
        file.open("../data/data.csv");
        std::string writeString;
        for ( ProcData snapshot : snapshots ) {
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
        }
        file.close();
    } catch (...) {
        file.close();
        // @TODO log error
    }
}

bool DataCollector::captureCPUSnapshot(){
    auto lock = std::unique_lock<std::mutex>(mutex);
    try {
        stat.open("/proc/stat");
        std::string readLine;
        ProcData snapshot;
        int count = 2;                                                  // Number of features to be read;
        while( std::getline(stat,readLine) && count > 0 ) {
            if ( readLine.at(1) == 'p' && readLine.at(2) == 'u' && count > 1 ) {
                snapshot.cpuUtilization = readLine;
                count--;                                              // CPU usage comes first so read all cpu information
            }
            else if ( readLine.at(2) = 'x' && readLine.at(3) == 't' ) {
                snapshot.contextSwitches = readLine;                   
                count--;                                              // context switches comes next, so assign count to 0
            }
        }
        stat.close();
        count = 4;                                                      // 4 features need to be read
        stat.open("/proc/meminfo");
        while ( std::getline(stat,readLine) && count > 0 ) {
            if ( readLine.at(0) == 'S' && readLine.at(1) == 'w' ) {
                if (  readLine.at(4) == 'T' ) {
                    snapshot.swapTotal = readLine;
                    count--;                                                      // Reduce required features by 1                      
                }
                else if ( readLine.at(4) == 'F' ) {
                    snapshot.swapFree = readLine;
                    count--;                                                      // Reduce required features by 1
                }
            }
            else if ( readLine.at(0) == 'M' && readLine.at(1) == 'e' ) {
                if (  readLine.at(3) == 'T' ) {
                    snapshot.memTotal = readLine;
                    count--;                                                      // Reduce required features by 1
                }
                else if ( readLine.at(3) == 'F' ) {
                    snapshot.memFree = readLine;
                    count--;                                                      // Reduce required features by 1
                }
            }
        }
        stat.close();
        snapshots.push_back(snapshot);
        return true;
    } catch (...){
        // @TODO log error
        stat.close();
        return false;
    }
}