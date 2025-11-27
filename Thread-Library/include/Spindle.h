#ifndef __SPINDLE__
#define __SPINDLE__

#include <atomic>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <type_traits>
#include <utility>
#include <vector>

#include "Thread.h"
#include "ThreadConfig.h"

/*
 * Class to create and manage the execution of thread workers.
 */
class Spindle {
private:
    Config* config;
    std::vector<std::unique_ptr<Thread>> workers;
    std::vector<std::size_t> trainingPlan;
    std::atomic<TaskId> processCounter;
    std::atomic<bool> initialized;
    mutable std::mutex workerMutex;

    explicit Spindle(Config* config);

    std::size_t resolveThreadCount(int requestedThreads) const;
    std::size_t selectWorker(TaskId taskId);
    std::size_t findLeastLoadedWorker() const;
    void dispatchWork(std::function<void()> job, TaskId taskId);

public:
    ~Spindle();

    static Spindle& getInstance(Config* configuration);

    bool init(int threads);
    bool addProcess(void (*functPtr)());
    void setTrainingPlan(const std::vector<std::size_t>& planner);
    bool isInitialized() const;

    template <typename Func, typename... Args>
    auto submit(Func&& func, Args&&... args)
        -> std::future<std::invoke_result_t<Func, Args...>>;
};

template <typename Func, typename... Args>
auto Spindle::submit(Func&& func, Args&&... args)
    -> std::future<std::invoke_result_t<Func, Args...>> {
    using Result = std::invoke_result_t<Func, Args...>;
    auto bound = std::bind(std::forward<Func>(func), std::forward<Args>(args)...);
    auto task = std::make_shared<std::packaged_task<Result()>>(std::move(bound));
    auto future = task->get_future();
    TaskId taskId = processCounter.fetch_add(1, std::memory_order_relaxed);
    dispatchWork([task]() { (*task)(); }, taskId);
    return future;
}
#endif