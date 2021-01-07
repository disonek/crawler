#include <gtest/gtest.h>
using namespace testing;

#include "Crawler.hpp"
#include "SharedObjects.hpp"
#include "imgui/ImGuiLayer.hpp"

TEST(HighLevelTests, DISABLED_guiAndWebcrawlerRunningOnAnotherThreads)
{
    BasicProtectedQueue taskQueue;

    std::string url = "https://www.google.com/doodles";

    auto crawlerResult = std::async(
        std::launch::async, [&taskQueue, url] { webcrawler::crawlLinksFromUrlAndPushToTaskQueue(taskQueue, url); });

    auto guiResult = std::async(std::launch::async, [&taskQueue] {
        img::ImGuiLayer imGuiLayer{};
        imGuiLayer.guiThread(taskQueue);
    });
}

TEST(HighLevelTests, checkPushToTaskQueue)
{
    BasicProtectedQueue taskQueue;

    std::set<std::string> tasks = {"ala ma kota", "kot ma Ale"};
    taskQueue.push({"ala ma kota2", "kot ma Ale2"});

    auto guiResult = std::async(std::launch::async, [&taskQueue] {
        img::ImGuiLayer imGuiLayer{};
        imGuiLayer.guiThread(taskQueue);
    });

    auto crawlerResult = std::async(std::launch::async, [&taskQueue, tasks] { taskQueue.push(tasks); });
}