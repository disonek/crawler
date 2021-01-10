#include <gtest/gtest.h>
using namespace testing;

#include "Crawler.hpp"
#include "OpenGLModule.hpp"
#include "SharedObjects.hpp"
#include "imgui/ImGuiLayer.hpp"
#include "mocks/ImGuiLoggerMock.hpp"
#include "mocks/OpenGLModuleMock.hpp"

TEST(HighLevelTests, DISABLED_defaultUsage)
{
    BasicProtectedQueue taskQueue;

    std::string url = "https://www.google.com/doodles";

    auto crawlerResult = std::async(
        std::launch::async, [&taskQueue, url] { webcrawler::crawlLinksFromUrlAndPushToTaskQueue(taskQueue, url); });

    auto guiResult = std::async(std::launch::async, [&taskQueue] {
        img::ImGuiLayer imGuiLayer{std::make_shared<img::OpenGLModule>(), std::make_shared<img::ImGuiLogger>()};
        imGuiLayer.guiThread(taskQueue);
    });
}

TEST(HighLevelTests, oneTimeRenderLoopRun)
{
    BasicProtectedQueue taskQueue;
    std::shared_ptr<OpenGLModuleMock> module = std::make_shared<StrictMock<OpenGLModuleMock>>();
    std::shared_ptr<ImGuiLoggerMock> logger = std::make_shared<StrictMock<ImGuiLoggerMock>>();

    EXPECT_CALL(*module, initalize());
    EXPECT_CALL(*module, windowShouldClose()).WillOnce(Return(false)).WillOnce(Return(true));
    EXPECT_CALL(*module, startNewFrame());
    EXPECT_CALL(*module, createDockspace(_));
    EXPECT_CALL(*module, render());
    EXPECT_CALL(*logger, draw(_, _));
    EXPECT_CALL(*module, shutDown());

    auto guiResult = std::async(std::launch::async, [&taskQueue, &module, &logger] {
        img::ImGuiLayer imGuiLayer{module, logger};
        imGuiLayer.guiThread(taskQueue);
    });
}

TEST(HighLevelTests, DISABLED_toDoChekTaskPush)
{
    BasicProtectedQueue taskQueue;
    std::shared_ptr<OpenGLModuleMock> module = std::make_shared<StrictMock<OpenGLModuleMock>>();
    std::shared_ptr<ImGuiLoggerMock> logger = std::make_shared<StrictMock<ImGuiLoggerMock>>();

    EXPECT_CALL(*module, initalize());
    EXPECT_CALL(*module, windowShouldClose()).WillOnce(Return(false)).WillOnce(Return(true));
    EXPECT_CALL(*module, startNewFrame());
    EXPECT_CALL(*module, createDockspace(_));
    EXPECT_CALL(*module, render());
    EXPECT_CALL(*logger, draw(_, _));
    EXPECT_CALL(*module, shutDown());

    std::set<std::string> tasks = {"ala ma kota", "kot ma Ale"};
    taskQueue.push({"ala ma kota2", "kot ma Ale2"});

    auto guiResult = std::async(std::launch::async, [&taskQueue, &module, &logger] {
        img::ImGuiLayer imGuiLayer{module, logger};
        imGuiLayer.guiThread(taskQueue);
    });

    auto crawlerResult = std::async(std::launch::async, [&taskQueue, tasks] { taskQueue.push(tasks); });
}