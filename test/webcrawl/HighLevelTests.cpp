#include <gtest/gtest.h>
using namespace testing;

#include "Crawler.hpp"
#include "OpenGLModule.hpp"
#include "SharedObjects.hpp"
#include "imgui/ImGuiLayer.hpp"
#include "mocks/ImGuiLoggerMock.hpp"
#include "mocks/OpenGLModuleMock.hpp"

class HighLevelTest : public ::testing::Test
{
public:
    HighLevelTest()
        : module{std::make_shared<StrictMock<OpenGLModuleMock>>()}
        , logger{std::make_shared<StrictMock<ImGuiLoggerMock>>()}
    {
    }

    ProtectedQueue taskQueue;
    std::shared_ptr<OpenGLModuleMock> module;
    std::shared_ptr<ImGuiLoggerMock> logger;

    void singleGuiLoopRun()
    {
        EXPECT_CALL(*module, initalize());
        EXPECT_CALL(*module, windowShouldClose()).WillOnce(Return(false)).WillOnce(Return(true));
        EXPECT_CALL(*module, startNewFrame());
        EXPECT_CALL(*module, createDockspace(_));
        EXPECT_CALL(*module, render());
        EXPECT_CALL(*logger, draw(_, _));
        EXPECT_CALL(*module, shutDown());
    }
};

// TEST(mainExample, defaultUsage)
TEST(mainExample, DISABLED_defaultUsageAsync)
{
    ProtectedQueue taskQueue;

    webcrawler::Crawler crawler;
    auto crawlerResult = std::async(std::launch::async, [&taskQueue, &crawler] { crawler.run(taskQueue); });

    auto guiResult = std::async(std::launch::async, [&taskQueue] {
        img::ImGuiLayer imGuiLayer{std::make_shared<img::OpenGLModule>(), std::make_shared<img::ImGuiLogger>()};
        imGuiLayer.intialize();
        imGuiLayer.run(taskQueue);
    });
}

TEST(mainExample, DISABLED_defaultUsageThread)
{
    ProtectedQueue taskQueue;

    webcrawler::Crawler crawler;
    img::ImGuiLayer imGuiLayer{std::make_shared<img::OpenGLModule>(), std::make_shared<img::ImGuiLogger>()};
    std::thread crawlerThread(&webcrawler::Crawler::run, &crawler, std::ref(taskQueue));

    std::thread imGuiLayerThread([&taskQueue, &imGuiLayer]() {
        imGuiLayer.intialize();
        imGuiLayer.run(taskQueue);
    });

    crawlerThread.join();
    imGuiLayerThread.join();
}

TEST_F(HighLevelTest, oneIterationRunLoop)
{
    singleGuiLoopRun();
    auto guiResult =
        std::async(std::launch::async, [&taskQueue = this->taskQueue, &module = this->module, &logger = this->logger] {
            img::ImGuiLayer imGuiLayer{module, logger};
            imGuiLayer.intialize();
            imGuiLayer.run(taskQueue);
        });
}

TEST_F(HighLevelTest, pushedTasksLoggedInGuiThread)
{
    singleGuiLoopRun();
    std::set<std::string> tasks = {"ala ma kota", "kot ma Ale"};
    taskQueue.pushResponse({"ala ma kota2", "kot ma Ale2"});

    auto guiResult =
        std::async(std::launch::async, [&taskQueue = this->taskQueue, &module = this->module, &logger = this->logger] {
            img::ImGuiLayer imGuiLayer{module, logger};
            imGuiLayer.intialize();
            imGuiLayer.run(taskQueue);
        });

    EXPECT_CALL(*logger, addSimpleLog("ala ma kota2"));
    EXPECT_CALL(*logger, addSimpleLog("kot ma Ale2"));

    auto crawlerResult =
        std::async(std::launch::async, [&taskQueue = this->taskQueue, tasks] { taskQueue.pushResponse(tasks); });
}