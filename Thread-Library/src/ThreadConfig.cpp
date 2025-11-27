#include "../include/ThreadConfig.h"

Config::Config(RUN_MODE rm, THREAD_MODE tm) : runMode(rm), threadMode(tm) {}

Config::~Config() = default;

RUN_MODE Config::getRunningMode() {
    return runMode;
}

THREAD_MODE Config::getThreadMode() {
    return threadMode;
}

bool Config::setRunningMode(RUN_MODE rm) {
    runMode = rm;
    return true;
}

bool Config::setThreadMode(THREAD_MODE tm) {
    threadMode = tm;
    return true;
}

Config& Config::getInstance(RUN_MODE rm, THREAD_MODE tm) {
    Config& config = getInstance();
    config.setRunningMode(rm);
    config.setThreadMode(tm);
    return config;
}

Config& Config::getInstance() {
    static Config config;
    return config;
}

SCHEDULING Config::getSchedulingType() {
    return scheduling;
}

bool Config::setSchedulingType(SCHEDULING sc) {
    scheduling = sc;
    return true;
}

ORDER Config::getOrder() {
    return order;
}

bool Config::setOrder(ORDER o) {
    order = o;
    return true;
}