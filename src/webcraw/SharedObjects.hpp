#pragma once

#include <future>
#include <mutex>
#include <queue>
#include <set>
#include <string>
#include <thread>
#include <utility>

struct BasicProtectedQueue
{
    std::mutex mutex;
    std::queue<std::set<std::string>> tasks;
};

struct TaskQueuePackagedTask
{
    std::deque<std::packaged_task<void(std::set<std::string>)>> tasks;
    std::mutex mutex;

    template <typename Func>
    std::future<void> postTaskForGuiThread(Func f)
    {
        std::packaged_task<void(std::set<std::string>)> task(f);
        std::future<void> res = task.get_future();
        std::lock_guard<std::mutex> lk(mutex);
        tasks.push_back(std::move(task));

        return res;
    }

    // std::packaged_task<void(std::set<std::string>)> task;
    // {
    //     std::lock_guard<std::mutex> lg(taskQueue.mutex);
    //     if(taskQueue.tasks.empty())
    //         continue;

    //     task = std::move(taskQueue.tasks.front());
    //     taskQueue.tasks.pop_front();
    // }
    // task("https://www.google.com/doodles");
};
;