#ifndef __THREAD_CLASS__
#define __THREAD_CLASS__

#include <atomic>
#include <condition_variable>
#include <cstddef>
#include <cstdint>
#include <deque>
#include <functional>
#include <mutex>
#include <thread>

#include "ThreadConfig.h"

/*
 * Create a constant set of states possible for the threads to be in
 */
enum THREAD_STATE { INIT, STOPPED, RUNNING, FINISHED };

using TaskId = std::uint64_t;

struct WorkItem {
    std::function<void()> task;
    TaskId id;
};

class Thread {
private:
    std::thread::id tid;
    std::thread worker;
    mutable std::mutex queueMutex;
    std::condition_variable queueConditionVariable;
    std::deque<WorkItem> workQueue;
    std::atomic<bool> stopping{false};
    std::atomic<THREAD_STATE> threadState{THREAD_STATE::INIT};

    void workerLoop();

public:
    Thread();
    ~Thread();

    Thread(const Thread&) = delete;
    Thread& operator=(const Thread&) = delete;
    Thread(Thread&&) = delete;
    Thread& operator=(Thread&&) = delete;

    void enqueue(WorkItem&& work);
    std::size_t pending() const;
    THREAD_STATE getRunningState() const;
};

#endif
