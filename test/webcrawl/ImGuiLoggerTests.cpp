#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace testing;

#include "Crawler.hpp"
#include "OpenGLModule.hpp"
#include "SharedObjects.hpp"
#include "imgui/ImGuiLayer.hpp"
#include "mocks/ImGuiLoggerMock.hpp"
#include "mocks/OpenGLModuleMock.hpp"

class ImGuiLoggerTest : public ::testing::Test
{
public:
    ImGuiLoggerTest()
        : module{std::make_shared<StrictMock<OpenGLModuleMock>>()}
        , logger{std::make_shared<StrictMock<ImGuiLoggerMock>>()}
    {
    }

    ProtectedQueue taskQueue;
    std::shared_ptr<OpenGLModuleMock> module;
    std::shared_ptr<ImGuiLoggerMock> logger;
};

// Test 1: ImGuiLogger window close button - persistent state
TEST_F(ImGuiLoggerTest, loggerWindowClosePersistence)
{
    EXPECT_CALL(*module, initalize());
    // First frame: window open, returns false (don't close)
    EXPECT_CALL(*module, windowShouldClose()).WillOnce(Return(false));
    EXPECT_CALL(*module, startNewFrame());
    EXPECT_CALL(*module, createDockspace(_)); // Accept any bool& reference
    EXPECT_CALL(*logger, draw(StrEq("Webcreawler"), _, _))
        .WillOnce(DoAll(SetArgPointee<1>(false), Return(false))); // Simulate close button click
    EXPECT_CALL(*module, render());

    // Second frame: window should be closed, logger->draw() should not be called
    EXPECT_CALL(*module, windowShouldClose()).WillOnce(Return(true));
    EXPECT_CALL(*module, startNewFrame());
    EXPECT_CALL(*module, createDockspace(_)); // Accept any bool& reference
    // No expectation for logger->draw() - it should not be called when loggerWindowOpen is false
    EXPECT_CALL(*module, render());
    EXPECT_CALL(*logger, clear());
    EXPECT_CALL(*module, shutDown());

    img::ImGuiLayer imGuiLayer{module, logger};
    imGuiLayer.intialize();
    imGuiLayer.run(taskQueue);
}

// Test 2: ImGuiLogger stays open when p_open is not modified
TEST_F(ImGuiLoggerTest, loggerWindowStaysOpenByDefault)
{
    EXPECT_CALL(*module, initalize());
    EXPECT_CALL(*module, windowShouldClose()).WillOnce(Return(false)).WillOnce(Return(true));
    EXPECT_CALL(*module, startNewFrame()).Times(2);
    EXPECT_CALL(*module, createDockspace(_)); // Accept any bool& reference
    EXPECT_CALL(*logger, draw(StrEq("Webcreawler"), _, _))
        .WillOnce(Return(false)); // Draw returns false (no crawl request)
    EXPECT_CALL(*module, render()).Times(2);
    EXPECT_CALL(*logger, clear());
    EXPECT_CALL(*module, shutDown());

    img::ImGuiLayer imGuiLayer{module, logger};
    imGuiLayer.intialize();
    imGuiLayer.run(taskQueue);
}

// Test 3: Logs are properly consumed from queue
TEST_F(ImGuiLoggerTest, logsConsumedFromQueue)
{
    EXPECT_CALL(*module, initalize());
    EXPECT_CALL(*module, windowShouldClose()).WillOnce(Return(false)).WillOnce(Return(true));
    EXPECT_CALL(*module, startNewFrame()).Times(2);
    EXPECT_CALL(*module, createDockspace(_));
    EXPECT_CALL(*logger, draw(_, _, _)).WillOnce(Return(false));
    EXPECT_CALL(*module, render()).Times(2);

    // Expect the logs to be added
    EXPECT_CALL(*logger, addSimpleLog("Log message 1"));
    EXPECT_CALL(*logger, addSimpleLog("Log message 2"));
    EXPECT_CALL(*logger, clear());
    EXPECT_CALL(*module, shutDown());

    // Push response with logs
    taskQueue.pushResponse({"Log message 1", "Log message 2"});

    img::ImGuiLayer imGuiLayer{module, logger};
    imGuiLayer.intialize();
    imGuiLayer.run(taskQueue);
}

// Test 4: Missing logs don't crash (nullptr response)
TEST_F(ImGuiLoggerTest, handlesNulloptResponseGracefully)
{
    EXPECT_CALL(*module, initalize());
    EXPECT_CALL(*module, windowShouldClose()).WillOnce(Return(false)).WillOnce(Return(true));
    EXPECT_CALL(*module, startNewFrame()).Times(2);
    EXPECT_CALL(*module, createDockspace(_));
    EXPECT_CALL(*logger, draw(_, _, _)).WillOnce(Return(false));
    EXPECT_CALL(*module, render()).Times(2);
    EXPECT_CALL(*logger, clear());
    EXPECT_CALL(*module, shutDown());

    // Don't push any response - popResponse() returns nullopt
    img::ImGuiLayer imGuiLayer{module, logger};
    imGuiLayer.intialize();
    imGuiLayer.run(taskQueue);
}

// Test 5: Crawl request when logger->draw() returns true
TEST_F(ImGuiLoggerTest, crawlRequestFromLoggerDraw)
{
    EXPECT_CALL(*module, initalize());
    EXPECT_CALL(*module, windowShouldClose()).WillOnce(Return(false)).WillOnce(Return(true));
    EXPECT_CALL(*module, startNewFrame()).Times(2);
    EXPECT_CALL(*module, createDockspace(_));
    EXPECT_CALL(*logger, draw(StrEq("Webcreawler"), _, _))
        .WillOnce(Return(true));
    EXPECT_CALL(*module, render()).Times(2);
    EXPECT_CALL(*logger, clear());
    EXPECT_CALL(*module, shutDown());

    img::ImGuiLayer imGuiLayer{module, logger};
    imGuiLayer.intialize();
    imGuiLayer.run(taskQueue);
}

// Test 6: ImGuiLogger clear is called properly
TEST_F(ImGuiLoggerTest, loggerClearCalledDuringShutdown)
{
    InSequence seq;
    EXPECT_CALL(*logger, clear()).Times(1);
    EXPECT_CALL(*module, shutDown()).Times(1);

    img::ImGuiLayer imGuiLayer{module, logger};
    imGuiLayer.shutdown();
}
