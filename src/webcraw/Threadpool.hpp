#pragma once

#include <condition_variable>
#include <deque>
#include <functional>
#include <mutex>
#include <thread>
#include <unordered_set>
#include <vector>

class ThreadPool; // forward declare

class Worker
{
public:
    Worker(ThreadPool& s, int id)
        : pool(s)
        , id(id)
    {
    }
    void operator()();

private:
    ThreadPool& pool;
    int id;
};

class ThreadPool
{
public:
    ThreadPool(const size_t& threads, std::condition_variable& notifier);
    template <class F>
    void enqueue(F f)
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        tasks.push_back(std::function<void()>(f));
        lock.unlock();
        cond.notify_one();
    }
    ~ThreadPool();
    size_t getAmountFreeWorkers();

private:
    friend class Worker;

    std::vector<std::thread> workers;
    std::deque<std::function<void()>> tasks;

    std::mutex queue_mutex;
    std::condition_variable cond;
    std::condition_variable& notifier;
    std::unordered_set<int> freeWorkers;
    std::mutex workersLocker;
    bool stop;
};
