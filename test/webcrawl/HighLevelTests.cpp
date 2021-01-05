#include <gtest/gtest.h>
using namespace testing;

#include "Crawler.hpp"
#include "ScopedTimer.hpp"
#include "SharedObjects.hpp"
#include "TraceTimer.hpp"
#include "imgui/ImGuiLayer.hpp"

TEST(HighLevelTests, guiAndWebcrawlerRunningOnAnotherThreads)
{
    BasicProtectedQueue taskQueue;

    std::string url = "https://blog.conan.io/2020/09/24/New-conan-training-series.html";

    auto crawlerResult =
        std::async(std::launch::async, [&taskQueue, url] { webcrawler::Crawler::CrawlerThread(taskQueue, url); });

    auto guiResult = std::async(std::launch::async, [&taskQueue] {
        img::ImGuiLayer imGuiLayer{};
        imGuiLayer.guiThread(taskQueue);
    });
}