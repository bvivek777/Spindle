#include "../include/Spindle.h"
#include "../include/ThreadConfig.h"

#include <atomic>
#include <future>
#include <iostream>
#include <vector>

namespace {

int computeValue(int base, int delta) {
    return base + delta;
}

}  // namespace

int main(int, char**) {
    Config& config = Config::getInstance(RUN_MODE::PERFORMANCE, THREAD_MODE::POOL);
    config.setSchedulingType(SCHEDULING::FCFS_SC);

    Spindle& spindle = Spindle::getInstance(&config);
    spindle.init(0);

    std::atomic<int> counter{0};
    std::vector<std::future<int>> futures;

    for (int i = 0; i < 8; ++i) {
        futures.push_back(spindle.submit([&counter, i]() {
            counter.fetch_add(1, std::memory_order_relaxed);
            return computeValue(10, i);
        }));
    }

    spindle.addProcess([]() { std::cout << "Spindle is ready for general use\n"; });

    int resultSum = 0;
    for (auto& fut : futures) {
        resultSum += fut.get();
    }

    constexpr int expectedSum = 8 * 10 + (7 * 8) / 2;

    if (counter.load() != 8 || resultSum != expectedSum) {
        std::cerr << "Thread scheduling failed\n";
        return 1;
    }

    return 0;
}