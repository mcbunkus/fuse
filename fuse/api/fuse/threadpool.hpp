#pragma once

#include <cstddef>
#include <functional>
#include <future>
#include <iostream>
#include <mutex>
#include <ostream>
#include <queue>
#include <stdexcept>
#include <thread>
#include <vector>

namespace fuse
{

/**
 * ThreadPool is the simplest possible implementation of.. a thread pool. Create
 * a ThreadPool and submit tasks to it, so you don't have to create/drop threads
 * yourself. Here's a pool of them sitting here ready to go.
 */
class ThreadPool
{

    std::vector<std::thread> m_workers;
    std::queue<std::function<void()>> m_tasks;

    std::mutex m_queueMutex;
    std::condition_variable m_condition;
    std::atomic_bool m_stop;

    // multiple worker's are spawned in the constructor, they then watch the
    // queue for incoming tasks, and then execute them once there's one
    // available
    void worker()
    {
        while (!m_stop)
        {
            std::function<void()> task;
            {
                std::unique_lock<std::mutex> lock{m_queueMutex};
                m_condition.wait(
                    lock,
                    [this] { return this->m_stop || !this->m_tasks.empty(); });

                if (m_stop && m_tasks.empty())
                {
                    return;
                }

                task = std::move(m_tasks.front());
                m_tasks.pop();
            }
            task();
        }
    }

  public:
    ThreadPool(size_t numThreads = std::thread::hardware_concurrency())
        : m_stop{false}
    {
        for (size_t i = 0; i < numThreads; ++i)
        {
            m_workers.emplace_back([this] { this->worker(); });
        }
    }

    ~ThreadPool()
    {
        m_stop = true;
        m_condition.notify_all();
        for (std::thread &worker : m_workers)
        {
            if (worker.joinable())
            {
                worker.join();
            }
        }
    }

    // love incomprehensible C++ template incantations, essentially all this
    // does is put your method/function in a queue, waiting until a thread is
    // available. The thread will then pick up your task and execute it
    template <class F, class... Args>
    auto enqueue(F &&f, Args &&...args) -> std::future<decltype(f(args...))>
    {
        using returnType = decltype(f(args...));

        auto task = std::make_shared<std::packaged_task<returnType()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...));

        std::future<returnType> res = task->get_future();
        {
            std::unique_lock<std::mutex> lock{m_queueMutex};
            if (m_stop)
            {
                throw std::runtime_error("enqueue on stopped ThreadPool");
            }
            m_tasks.emplace([task]() { (*task)(); });
        }

        m_condition.notify_one();
        return res;
    }
};

} // namespace fuse
