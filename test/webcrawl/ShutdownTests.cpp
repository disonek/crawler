#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace testing;

#include "Crawler.hpp"
#include "OpenGLModule.hpp"
#include "SharedObjects.hpp"
#include "imgui/ImGuiLayer.hpp"
#include "mocks/ImGuiLoggerMock.hpp"
#include "mocks/OpenGLModuleMock.hpp"

class ShutdownTest : public ::testing::Test
{
public:
    ShutdownTest()
        : module{std::make_shared<StrictMock<OpenGLModuleMock>>()}
        , logger{std::make_shared<StrictMock<ImGuiLoggerMock>>()}
    {
    }

    ProtectedQueue taskQueue;
    std::shared_ptr<OpenGLModuleMock> module;
    std::shared_ptr<ImGuiLoggerMock> logger;
};

// Test 1: shutdown() method calls logger->clear() and openGLModule->shutDown()
TEST_F(ShutdownTest, shutdownCallsClearAndShutDown)
{
    EXPECT_CALL(*logger, clear()).Times(1);
    EXPECT_CALL(*module, shutDown()).Times(1);

    img::ImGuiLayer imGuiLayer{module, logger};
    imGuiLayer.shutdown();

    // Verify expectations met
    EXPECT_TRUE(testing::Mock::VerifyAndClearExpectations(logger.get()));
    EXPECT_TRUE(testing::Mock::VerifyAndClearExpectations(module.get()));
}

// Test 2: Double-shutdown prevention
TEST_F(ShutdownTest, doubleShutdownPrevention)
{
    EXPECT_CALL(*logger, clear()).Times(1);
    EXPECT_CALL(*module, shutDown()).Times(1);

    img::ImGuiLayer imGuiLayer{module, logger};
    imGuiLayer.shutdown();
    imGuiLayer.shutdown(); // Second call should not call the mocked methods again

    // Verify expectations met - should only be called once
    EXPECT_TRUE(testing::Mock::VerifyAndClearExpectations(logger.get()));
    EXPECT_TRUE(testing::Mock::VerifyAndClearExpectations(module.get()));
}

// Test 3: Destructor calls shutdown()
TEST_F(ShutdownTest, destructorCallsShutdown)
{
    EXPECT_CALL(*logger, clear()).Times(1);
    EXPECT_CALL(*module, shutDown()).Times(1);

    {
        img::ImGuiLayer imGuiLayer{module, logger};
        // Destructor will be called when leaving scope
    }

    EXPECT_TRUE(testing::Mock::VerifyAndClearExpectations(logger.get()));
    EXPECT_TRUE(testing::Mock::VerifyAndClearExpectations(module.get()));
}

// Test 4: Shutdown order - clear called before shutDown
TEST_F(ShutdownTest, shutdownOrderClearBeforeShutDown)
{
    InSequence seq;
    EXPECT_CALL(*logger, clear()).Times(1);
    EXPECT_CALL(*module, shutDown()).Times(1);

    img::ImGuiLayer imGuiLayer{module, logger};
    imGuiLayer.shutdown();
}
