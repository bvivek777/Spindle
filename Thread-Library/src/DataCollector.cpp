#include "DataCollector.h"

DataCollector::DataCollector(/* args */) {
    file.open("../data/data.csv",std::ios::out);
}

DataCollector::~DataCollector() {
    file.close();
}