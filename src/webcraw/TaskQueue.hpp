#pragma once

#include <deque>
#include <future>
#include <mutex>
#include <set>
#include <string>
#include <thread>
#include <utility>

struct TaskQueue
{
    std::deque<std::packaged_task<std::set<std::string>(std::string)>> tasks;
    std::mutex mutex;

    template <typename Func>
    std::future<std::set<std::string>> postTaskForGuiThread(Func f)
    {
        std::packaged_task<std::set<std::string>(std::string)> task(f);
        std::future<std::set<std::string>> res = task.get_future();
        std::lock_guard<std::mutex> lk(mutex);
        tasks.push_back(std::move(task));

        return res;
    }
};