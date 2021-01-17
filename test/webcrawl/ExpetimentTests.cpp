#include <gtest/gtest.h>
using namespace testing;

#include <spdlog/spdlog.h>

#include <chrono>
#include <future>
#include <queue>
#include <set>
#include <string>
#include <thread>

using ExType = std::set<std::string>;

TEST(ExperimentTest, packaged_task)
{
    std::packaged_task<ExType()> task([] {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        ExType res = {"ala", "ma", "kota"};
        return res;
    });

    std::future<std::set<std::string>> fut = task.get_future();
    auto notNeeded = std::async(std::launch::async, std::move(task));

    ExType expectedResult = {"ala", "ma", "kota"};
    auto res = fut.get();

    EXPECT_TRUE(res == expectedResult);
}

TEST(ExperimentTest, promise)
{
    std::promise<ExType> promise;
    std::future<ExType> fut = promise.get_future();
    std::thread thread(
        [](std::promise<ExType>&& pro) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            pro.set_value({"ALA", "MA", "KOTA"});
        },
        std::move(promise));
    thread.detach();
    ExType expectedResult = {"ALA", "MA", "KOTA"};
    auto res = fut.get();
    EXPECT_TRUE(res == expectedResult);
}

struct QueuePackagedTask
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