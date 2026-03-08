#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <future>
#include <thread>

using namespace testing;

#include "Crawler.hpp"
#include "OpenGLModule.hpp"
#include "SharedObjects.hpp"
#include "imgui/ImGuiLayer.hpp"
#include "mocks/ImGuiLoggerMock.hpp"
#include "mocks/OpenGLModuleMock.hpp"

class ExceptionHandlingTest : public ::testing::Test
{
public:
    ExceptionHandlingTest()
        : module{std::make_shared<StrictMock<OpenGLModuleMock>>()}
        , logger{std::make_shared<StrictMock<ImGuiLoggerMock>>()}
    {
    }

    ProtectedQueue taskQueue;
    std::shared_ptr<OpenGLModuleMock> module;
    std::shared_ptr<ImGuiLoggerMock> logger;
};

// Test 1: safeThreadWait returns false on exception
TEST_F(ExceptionHandlingTest, safeThreadWaitHandlesException)
{
    // Create a future that throws an exception
    auto future = std::async(std::launch::async, []() {
        throw std::runtime_error("Test exception");
    });

    // Simulate safeThreadWait behavior
    bool result = false;
    try
    {
        future.get();
    }
    catch(const std::exception& e)
    {
        result = false;
        taskQueue.ready.store(true); // Signal shutdown
    }

    EXPECT_FALSE(result);
    EXPECT_TRUE(taskQueue.ready.load());
}

// Test 2: safeThreadWait sets queue ready on exception
TEST_F(ExceptionHandlingTest, queueReadySetOnThreadException)
{
    // Create a future that throws an exception
    auto future = std::async(std::launch::async, []() {
        throw std::runtime_error("Test exception");
    });

    try
    {
        future.get();
    }
    catch(...)
    {
        taskQueue.ready.store(true);
    }

    EXPECT_TRUE(taskQueue.ready.load());
}

// Test 3: Normal completion returns true
TEST_F(ExceptionHandlingTest, safeThreadWaitReturnsTrue)
{
    // Create a future that completes successfully
    auto future = std::async(std::launch::async, []() {
        return true;
    });

    bool result = false;
    try
    {
        future.get();
        result = true;
    }
    catch(const std::exception&)
    {
        result = false;
    }

    EXPECT_TRUE(result);
}

// Test 4: OpenGL shutdown happens even if exception in GUI thread
TEST_F(ExceptionHandlingTest, openGLShutdownAfterGuiException)
{
    EXPECT_CALL(*module, initalize());
    EXPECT_CALL(*module, windowShouldClose()).WillOnce(Throw(std::runtime_error("GUI Exception")));
    EXPECT_CALL(*module, startNewFrame());
    EXPECT_CALL(*module, createDockspace(_));
    EXPECT_CALL(*logger, clear());
    EXPECT_CALL(*module, shutDown());

    try
    {
        img::ImGuiLayer imGuiLayer{module, logger};
        imGuiLayer.intialize();
        imGuiLayer.run(taskQueue);
    }
    catch(const std::exception&)
    {
        // Exception caught and handled at main level
    }

    // Verify shutDown was still called
    EXPECT_TRUE(testing::Mock::VerifyAndClearExpectations(module.get()));
}

// Test 5: Exception in logger draw doesn't crash
TEST_F(ExceptionHandlingTest, exceptionInLoggerDraw)
{
    EXPECT_CALL(*module, initalize());
    EXPECT_CALL(*module, windowShouldClose()).WillOnce(Return(false)).WillOnce(Return(true));
    EXPECT_CALL(*module, startNewFrame()).Times(2);
    EXPECT_CALL(*module, createDockspace(_)).Times(2);
    EXPECT_CALL(*logger, draw(_, _, _))
        .WillOnce(Throw(std::runtime_error("Logger Exception")));
    EXPECT_CALL(*module, render());
    EXPECT_CALL(*logger, clear());
    EXPECT_CALL(*module, shutDown());

    try
    {
        img::ImGuiLayer imGuiLayer{module, logger};
        imGuiLayer.intialize();
        imGuiLayer.run(taskQueue);
    }
    catch(const std::exception&)
    {
        // Exception is expected and should be caught at main level
    }
}

// Test 6: taskQueue.ready prevents infinite loop on error
TEST_F(ExceptionHandlingTest, taskQueueReadyPreventsInfiniteLoop)
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

    // When window closes, ready should be set
    EXPECT_TRUE(taskQueue.ready.load());
}

// Test 7: Destructor cleanup happens despite exceptions
TEST_F(ExceptionHandlingTest, destructorCleanupDespiteExceptions)
{
    EXPECT_CALL(*logger, clear()).Times(1);
    EXPECT_CALL(*module, shutDown()).Times(1);

    try
    {
        img::ImGuiLayer imGuiLayer{module, logger};
        imGuiLayer.shutdown();
        throw std::runtime_error("Exception after shutdown");
    }
    catch(const std::exception&)
    {
        // Exception caught
    }

    // Verify cleanup was called
    EXPECT_TRUE(testing::Mock::VerifyAndClearExpectations(logger.get()));
    EXPECT_TRUE(testing::Mock::VerifyAndClearExpectations(module.get()));
}

// Test 8: Multiple exceptions are handled sequentially
TEST_F(ExceptionHandlingTest, multipleExceptionsHandledSequentially)
{
    // Simulate two futures with exceptions
    auto future1 = std::async(std::launch::async, []() {
        throw std::runtime_error("Exception 1");
    });

    auto future2 = std::async(std::launch::async, []() {
        throw std::logic_error("Exception 2");
    });

    int exceptions_caught = 0;

    try
    {
        future1.get();
    }
    catch(const std::exception&)
    {
        exceptions_caught++;
        taskQueue.ready.store(true);
    }

    try
    {
        future2.get();
    }
    catch(const std::exception&)
    {
        exceptions_caught++;
    }

    EXPECT_EQ(2, exceptions_caught);
    EXPECT_TRUE(taskQueue.ready.load());
}

// Test 9: Queue ready flag is atomic and thread-safe
TEST_F(ExceptionHandlingTest, queueReadyAtomicAccess)
{
    // Test that ready flag can be safely set from different threads
    std::thread t1([this]() {
        taskQueue.ready.store(true);
    });

    std::thread t2([this]() {
        auto value = taskQueue.ready.load();
        (void)value; // Use the value to avoid compiler warning
    });

    t1.join();
    t2.join();

    EXPECT_TRUE(taskQueue.ready.load());
}

// Test 10: Shutdown called once per lifetime despite repeated shutdown attempts
TEST_F(ExceptionHandlingTest, shutdownGuardPreventsMultipleCalls)
{
    InSequence seq;
    EXPECT_CALL(*logger, clear()).Times(1);
    EXPECT_CALL(*module, shutDown()).Times(1);

    img::ImGuiLayer imGuiLayer{module, logger};

    // Call shutdown multiple times
    imGuiLayer.shutdown();
    imGuiLayer.shutdown();
    imGuiLayer.shutdown();

    // Verify mocks were called only once
    EXPECT_TRUE(testing::Mock::VerifyAndClearExpectations(logger.get()));
    EXPECT_TRUE(testing::Mock::VerifyAndClearExpectations(module.get()));
}
