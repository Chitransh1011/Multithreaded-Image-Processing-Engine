#pragma once

#include <condition_variable>
#include <cstddef>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>

namespace imaging {

class ThreadPool {
public:
    explicit ThreadPool(std::size_t workerCount);
    ~ThreadPool();

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;
    ThreadPool(ThreadPool&&) = delete;
    ThreadPool& operator=(ThreadPool&&) = delete;

    template <typename Function>
    auto submit(Function&& function) -> std::future<std::invoke_result_t<Function>> {
        using Result = std::invoke_result_t<Function>;

        auto task = std::make_shared<std::packaged_task<Result()>>(
            std::forward<Function>(function));
        std::future<Result> result = task->get_future();

        {
            std::lock_guard<std::mutex> lock(queueMutex_);
            if (stopping_) {
                throw std::runtime_error("Cannot submit work to a stopped thread pool.");
            }

            tasks_.emplace([task] { (*task)(); });
        }

        taskAvailable_.notify_one();
        return result;
    }

    std::size_t workerCount() const noexcept;

private:
    void workerLoop();

    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> tasks_;
    std::mutex queueMutex_;
    std::condition_variable taskAvailable_;
    bool stopping_ = false;
};

}  // namespace imaging
