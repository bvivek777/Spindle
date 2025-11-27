#include "../include/Spindle.h"

#include <algorithm>
#include <stdexcept>
#include <thread>

Spindle::Spindle(Config* configuration)
    : config(configuration),
      processCounter(0),
      initialized(false) {}

Spindle::~Spindle() = default;

Spindle& Spindle::getInstance(Config* configuration) {
    static Spindle spindle(configuration);
    if (configuration != nullptr) {
        spindle.config = configuration;
    }
    return spindle;
}

bool Spindle::init(int threads) {
    if (config == nullptr) {
        throw std::logic_error("Config must be provided before initializing Spindle");
    }

    const std::size_t workerCount = resolveThreadCount(threads);
    if (workerCount == 0) {
        throw std::invalid_argument("Cannot initialize Spindle with zero worker threads");
    }

    std::lock_guard<std::mutex> lock(workerMutex);
    workers.clear();
    workers.reserve(workerCount);
    for (std::size_t i = 0; i < workerCount; ++i) {
        workers.push_back(std::make_unique<Thread>());
    }

    processCounter.store(0, std::memory_order_relaxed);
    initialized.store(true, std::memory_order_release);
    return true;
}

bool Spindle::addProcess(void (*functPtr)()) {
    if (functPtr == nullptr) {
        return false;
    }
    submit(functPtr);
    return true;
}

void Spindle::setTrainingPlan(const std::vector<std::size_t>& planner) {
    std::lock_guard<std::mutex> lock(workerMutex);
    trainingPlan.clear();
    trainingPlan.reserve(planner.size());
    for (auto threadIndex : planner) {
        trainingPlan.push_back(threadIndex == 0 ? 0 : threadIndex - 1);
    }
}

bool Spindle::isInitialized() const {
    return initialized.load(std::memory_order_acquire);
}

std::size_t Spindle::resolveThreadCount(int requestedThreads) const {
    const std::size_t hw = std::max(1u, std::thread::hardware_concurrency());
    const THREAD_MODE mode = config->getThreadMode();

    switch (mode) {
        case THREAD_MODE::POOL:
            return hw;
        case THREAD_MODE::SPINDLE: {
            std::lock_guard<std::mutex> lock(workerMutex);
            if (!trainingPlan.empty()) {
                const auto maxEntry =
                    *std::max_element(trainingPlan.begin(), trainingPlan.end());
                const std::size_t requiredThreads =
                    std::max<std::size_t>(static_cast<std::size_t>(1), maxEntry + 1);
                return std::min<std::size_t>(requiredThreads, hw);
            }
            return hw;
        }
        case THREAD_MODE::CONSTANT:
            if (requestedThreads <= 0) {
                throw std::invalid_argument("Requested thread count must be positive in CONSTANT mode");
            }
            return std::min<std::size_t>(requestedThreads, hw);
        default:
            return hw;
    }
}

std::size_t Spindle::selectWorker(TaskId taskId) {
    if (workers.empty()) {
        throw std::logic_error("Spindle workers are not initialized");
    }

    const SCHEDULING scheduling = config->getSchedulingType();

    switch (scheduling) {
        case SCHEDULING::ML:
            if (!trainingPlan.empty()) {
                const std::size_t planIndex = static_cast<std::size_t>(taskId % trainingPlan.size());
                return trainingPlan[planIndex] % workers.size();
            }
            [[fallthrough]];
        case SCHEDULING::RR_SC:
            return static_cast<std::size_t>(taskId % workers.size());
        case SCHEDULING::FCFS_SC:
        default:
            return findLeastLoadedWorker();
    }
}

std::size_t Spindle::findLeastLoadedWorker() const {
    if (workers.empty()) {
        throw std::logic_error("No workers available");
    }

    std::size_t leastLoadedIndex = 0;
    std::size_t leastPending = workers.front()->pending();

    for (std::size_t i = 1; i < workers.size(); ++i) {
        const std::size_t pending = workers[i]->pending();
        if (pending < leastPending) {
            leastPending = pending;
            leastLoadedIndex = i;
        }
    }

    return leastLoadedIndex;
}

void Spindle::dispatchWork(std::function<void()> job, TaskId taskId) {
    if (!isInitialized()) {
        throw std::logic_error("Spindle must be initialized before dispatching work");
    }
    const std::size_t index = selectWorker(taskId);
    workers.at(index)->enqueue(WorkItem{std::move(job), taskId});
}