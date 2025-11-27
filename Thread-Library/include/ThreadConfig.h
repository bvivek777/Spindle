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

enum SCHEDULING {
    FCFS_SC,
    ML,
    RR_SC
};

enum ORDER {
    AUTO,
    DISTRIBUTE
};

class Config {
private:
    RUN_MODE runMode{RUN_MODE::PERFORMANCE};
    THREAD_MODE threadMode{THREAD_MODE::POOL};
    SCHEDULING scheduling{SCHEDULING::FCFS_SC};
    ORDER order{ORDER::AUTO};

    ~Config();
    Config() = default;
    Config(RUN_MODE rm, THREAD_MODE tm);

public:
    static Config& getInstance(RUN_MODE rm, THREAD_MODE tm);
    static Config& getInstance();
    bool setRunningMode(RUN_MODE rm);
    bool setThreadMode(THREAD_MODE tm);
    bool setSchedulingType(SCHEDULING sc);
    bool setOrder(ORDER newOrder);
    RUN_MODE getRunningMode();
    THREAD_MODE getThreadMode();
    SCHEDULING getSchedulingType();
    ORDER getOrder();
};
#endif