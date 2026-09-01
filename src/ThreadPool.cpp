#include "ThreadPool.h"

namespace imaging {

ThreadPool::ThreadPool(std::size_t workerCount) {
    if (workerCount == 0) {
        throw std::invalid_argument("Thread pool worker count must be greater than zero.");
    }

    workers_.reserve(workerCount);
    for (std::size_t index = 0; index < workerCount; ++index) {
        workers_.emplace_back(&ThreadPool::workerLoop, this);
    }
}

ThreadPool::~ThreadPool() {
    {
        std::lock_guard<std::mutex> lock(queueMutex_);
        stopping_ = true;
    }

    taskAvailable_.notify_all();
    for (std::thread& worker : workers_) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}

std::size_t ThreadPool::workerCount() const noexcept {
    return workers_.size();
}

void ThreadPool::workerLoop() {
    while (true) {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(queueMutex_);
            taskAvailable_.wait(lock, [this] { return stopping_ || !tasks_.empty(); });

            if (stopping_ && tasks_.empty()) {
                return;
            }

            task = std::move(tasks_.front());
            tasks_.pop();
        }

        task();
    }
}

}  // namespace imaging
