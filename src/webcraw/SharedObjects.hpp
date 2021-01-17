#pragma once

#include <future>
#include <mutex>
#include <queue>
#include <set>
#include <string>
#include <thread>
#include <utility>

template <typename TaskType = std::set<std::string>>
struct BasicProtectedQueue
{
    std::mutex mutex;
    std::queue<TaskType> tasks;

    template <typename T = TaskType>
    void push(T task)
    {
        std::lock_guard<std::mutex> lk(mutex);
        tasks.push(task);
    }
};
