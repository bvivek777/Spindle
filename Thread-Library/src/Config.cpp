#include "Config.h"

enum RUN_MODE {
    LEARN,
    PERFORMANCE,
    COLLECT_DATA
};

enum THREAD_MODE {
    POOL,
    SPINDLE,
    CONSTANT
};

Config::Config() {
}

Config::~Config() {
}
