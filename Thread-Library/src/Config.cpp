#include "Config.h"

Config::Config(RUN_MODE rm, THREAD_MODE tm) {
    runMode = rm;
    threadMode = tm;
}

Config::~Config() {
}

RUN_MODE Config::getRunningMode (){
    return runMode;
}

THREAD_MODE Config::getThreadMode(){
    return threadMode;
}

bool Config::setRunningMode(RUN_MODE rm){
    runMode = rm;
    return true;
}

bool Config::setThreadMode(THREAD_MODE tm){
    threadMode = tm;
    return true;
}

Config& Config::getInstance(RUN_MODE rm, THREAD_MODE tm){
    static Config config(rm, tm);
    return config;
}

Config& Config::getInstance(){
    static Config config;
    return config;
}