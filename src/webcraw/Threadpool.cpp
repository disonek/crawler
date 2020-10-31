#include "Threadpool.hpp"

#include <chrono>
#include <condition_variable>
#include <deque>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

void Worker::operator()()
{
    std::function<void()> task;
    while(true)
    {
        std::unique_lock<std::mutex> locker(pool.queue_mutex);
        // worker is free again, add to free workers
        std::unique_lock<std::mutex> workLocker(pool.workersLocker);
        pool.freeWorkers.insert(id);
        workLocker.unlock();
        pool.cond.wait(locker, [this]() { return !pool.tasks.empty() || pool.stop; });
        if(pool.stop)
        {
            locker.unlock();
            return;
        }
        // worker is busy, so remove from freeWorkers
        workLocker.lock();
        pool.freeWorkers.erase(id);
        workLocker.unlock();
        task = pool.tasks.front();
        pool.tasks.pop_front();
        locker.unlock();
        task();
        // notify condition_variable
        pool.notifier.notify_one();
    }
}

ThreadPool::ThreadPool(const size_t& threads, std::condition_variable& notifier)
    : notifier(notifier)
    , stop(false)
{
    for(size_t i = 0; i < threads; ++i)
        workers.emplace_back(Worker(*this, i + 1));
}

ThreadPool::~ThreadPool()
{
    stop = true; // stop all threads
    cond.notify_all();
    for(auto& thread : workers)
        thread.join();
}

size_t ThreadPool::getAmountFreeWorkers()
{
    std::lock_guard<std::mutex> workLocker(workersLocker);
    return freeWorkers.size();
}
