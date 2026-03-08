#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace testing;

#include "Crawler.hpp"
#include "OpenGLModule.hpp"
#include "SharedObjects.hpp"
#include "imgui/ImGuiLayer.hpp"
#include "mocks/ImGuiLoggerMock.hpp"
#include "mocks/OpenGLModuleMock.hpp"

class OpenGLModuleTest : public ::testing::Test
{
public:
    OpenGLModuleTest()
        : module{std::make_shared<StrictMock<OpenGLModuleMock>>()}
        , logger{std::make_shared<StrictMock<ImGuiLoggerMock>>()}
    {
    }

    ProtectedQueue taskQueue;
    std::shared_ptr<OpenGLModuleMock> module;
    std::shared_ptr<ImGuiLoggerMock> logger;
};

// Test 1: Window initialization on ImGuiLayer startup
TEST_F(OpenGLModuleTest, windowInitialization)
{
    EXPECT_CALL(*module, initalize()).Times(1);
    EXPECT_CALL(*module, windowShouldClose()).WillOnce(Return(true)); // Exit immediately
    EXPECT_CALL(*module, startNewFrame());
    EXPECT_CALL(*module, createDockspace(_));
    EXPECT_CALL(*module, render());
    EXPECT_CALL(*logger, clear());
    EXPECT_CALL(*module, shutDown());

    img::ImGuiLayer imGuiLayer{module, logger};
    imGuiLayer.intialize();
    imGuiLayer.run(taskQueue);
}

// Test 2: Window close signal properly detected
TEST_F(OpenGLModuleTest, windowCloseDetection)
{
    EXPECT_CALL(*module, initalize());
    EXPECT_CALL(*module, windowShouldClose()).WillOnce(Return(false)).WillOnce(Return(true));
    EXPECT_CALL(*module, startNewFrame()).Times(2);
    EXPECT_CALL(*module, createDockspace(_)).Times(2);
    EXPECT_CALL(*logger, draw(_, _, _)).WillOnce(Return(false));
    EXPECT_CALL(*module, render()).Times(2);
    EXPECT_CALL(*logger, clear());
    EXPECT_CALL(*module, shutDown());

    img::ImGuiLayer imGuiLayer{module, logger};
    imGuiLayer.intialize();
    imGuiLayer.run(taskQueue);
}

// Test 3: Queue receives ready signal when window closes
TEST_F(OpenGLModuleTest, queueReadySignalOnWindowClose)
{
    EXPECT_CALL(*module, initalize());
    EXPECT_CALL(*module, windowShouldClose()).WillOnce(Return(true)); // Window closes immediately
    EXPECT_CALL(*module, startNewFrame());
    EXPECT_CALL(*module, createDockspace(_));
    EXPECT_CALL(*module, render());
    EXPECT_CALL(*logger, clear());
    EXPECT_CALL(*module, shutDown());

    img::ImGuiLayer imGuiLayer{module, logger};
    imGuiLayer.intialize();
    imGuiLayer.run(taskQueue);

    EXPECT_TRUE(taskQueue.ready.load());
}

// Test 4: Multiple frame rendering cycle
TEST_F(OpenGLModuleTest, multipleFrameRenderingCycle)
{
    EXPECT_CALL(*module, initalize());
    EXPECT_CALL(*module, windowShouldClose())
        .WillOnce(Return(false))
        .WillOnce(Return(false))
        .WillOnce(Return(true));
    EXPECT_CALL(*module, startNewFrame()).Times(3);
    EXPECT_CALL(*module, createDockspace(_)).Times(3);
    EXPECT_CALL(*logger, draw(_, _, _)).WillOnce(Return(false)).WillOnce(Return(false));
    EXPECT_CALL(*module, render()).Times(3);
    EXPECT_CALL(*logger, clear());
    EXPECT_CALL(*module, shutDown());

    img::ImGuiLayer imGuiLayer{module, logger};
    imGuiLayer.intialize();
    imGuiLayer.run(taskQueue);
}

// Test 5: Shutdown is called exactly once
TEST_F(OpenGLModuleTest, shutDownCalledOnce)
{
    EXPECT_CALL(*module, initalize());
    EXPECT_CALL(*module, windowShouldClose()).WillOnce(Return(true));
    EXPECT_CALL(*module, startNewFrame());
    EXPECT_CALL(*module, createDockspace(_));
    EXPECT_CALL(*module, render());
    EXPECT_CALL(*logger, clear());
    EXPECT_CALL(*module, shutDown()).Times(1); // Verify exactly once

    img::ImGuiLayer imGuiLayer{module, logger};
    imGuiLayer.intialize();
    imGuiLayer.run(taskQueue);
}

// Test 6: Shutdown called on destructor
TEST_F(OpenGLModuleTest, shutDownCalledOnDestructor)
{
    EXPECT_CALL(*logger, clear()).Times(1);
    EXPECT_CALL(*module, shutDown()).Times(1);

    {
        img::ImGuiLayer imGuiLayer{module, logger};
        imGuiLayer.shutdown();
    }

    EXPECT_TRUE(testing::Mock::VerifyAndClearExpectations(module.get()));
}
