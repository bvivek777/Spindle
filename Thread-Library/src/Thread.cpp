#include "../include/Thread.h"

#include <iostream>
#include <utility>

Thread::Thread() {
    worker = std::thread([this]() { workerLoop(); });
}

Thread::~Thread() {
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        stopping = true;
    }
    queueConditionVariable.notify_all();
    if (worker.joinable()) {
        worker.join();
    }
    threadState = THREAD_STATE::STOPPED;
}

void Thread::enqueue(WorkItem&& work) {
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        workQueue.emplace_back(std::move(work));
    }
    queueConditionVariable.notify_one();
}

std::size_t Thread::pending() const {
    std::lock_guard<std::mutex> lock(queueMutex);
    return workQueue.size();
}

THREAD_STATE Thread::getRunningState() const {
    return threadState.load();
}

void Thread::workerLoop() {
    tid = std::this_thread::get_id();
    for (;;) {
        WorkItem work;
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            queueConditionVariable.wait(
                lock,
                [this]() { return stopping.load() || !workQueue.empty(); });

            if (stopping.load() && workQueue.empty()) {
                threadState = THREAD_STATE::STOPPED;
                return;
            }

            work = std::move(workQueue.front());
            workQueue.pop_front();
            threadState = THREAD_STATE::RUNNING;
        }

        try {
            if (work.task) {
                work.task();
            }
        } catch (const std::exception& ex) {
            std::cerr << "Thread worker exception: " << ex.what() << std::endl;
        } catch (...) {
            std::cerr << "Thread worker encountered unknown exception" << std::endl;
        }

        if (!stopping.load()) {
            std::lock_guard<std::mutex> lock(queueMutex);
            threadState = workQueue.empty() ? THREAD_STATE::FINISHED : THREAD_STATE::RUNNING;
        }
    }
}