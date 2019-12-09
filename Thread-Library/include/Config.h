#ifndef __CONFIG__
#define __CONFIG__

enum RUN_MODE {
    PERFORMANCE,
    COLLECT_DATA
};

enum THREAD_MODE {
    POOL,
    SPINDLE,
    CONSTANT
};

class Config {
private:
    RUN_MODE runMode;
    THREAD_MODE threadMode; 
    ~Config();
    Config(RUN_MODE rm, THREAD_MODE tm);
public:
    static Config& getInstance(RUN_MODE rm, THREAD_MODE tm);
    bool setRunningMode(RUN_MODE rm);
    bool setThreadMode(THREAD_MODE tm);
    RUN_MODE getRunningMode();
    THREAD_MODE getThreadMode();
};
#endif