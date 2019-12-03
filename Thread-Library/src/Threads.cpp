#include "Threads.h"

Threads::Threads(/* args */) {
    maxThreads = std::thread::hardware_concurrency();
}

Threads::~Threads() {
    delete threads;
}

Threads* Threads::getInstance() {
    if ( threads )
        threads = new Threads;
    return threads;
}