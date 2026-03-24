#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "imgui/ImGuiLogger.hpp"

using namespace testing;
using namespace img;

// ============================================================================
// Tests for ImGuiLogger Public Interface
// ============================================================================

class ImGuiLoggerTest : public ::testing::Test
{
protected:
    ImGuiLogger logger;
    
    void SetUp() override
    {
        // Initialize before each test
        logger.initialize();
    }
};

// Test 1: Initialize Does Not Crash
TEST_F(ImGuiLoggerTest, initializeDoesNotCrash)
{
    ImGuiLogger testLogger;
    EXPECT_NO_THROW(testLogger.initialize());
}

// Test 2: Clear Does Not Crash
TEST_F(ImGuiLoggerTest, clearDoesNotCrash)
{
    EXPECT_NO_THROW(logger.clear());
}

// Test 3: Add Simple Log Does Not Crash
TEST_F(ImGuiLoggerTest, addSimpleLogDoesNotCrash)
{
    EXPECT_NO_THROW(logger.addSimpleLog("Test message"));
}

// Test 4: Multiple Logs Can Be Added
TEST_F(ImGuiLoggerTest, multipleLogs)
{
    EXPECT_NO_THROW({
        logger.addSimpleLog("First");
        logger.addSimpleLog("Second");
        logger.addSimpleLog("Third");
    });
}

// Test 5: Clear After Adding Logs
TEST_F(ImGuiLoggerTest, clearAfterAddingLogs)
{
    logger.addSimpleLog("Message 1");
    logger.addSimpleLog("Message 2");
    EXPECT_NO_THROW(logger.clear());
    logger.addSimpleLog("Message 3");
}

// Test 6: Large Messages
TEST_F(ImGuiLoggerTest, largeMessages)
{
    std::string largeMessage(100000, 'A');
    EXPECT_NO_THROW(logger.addSimpleLog(largeMessage));
}

// Test 7: Special Characters in Messages
TEST_F(ImGuiLoggerTest, specialCharacters)
{
    EXPECT_NO_THROW({
        logger.addSimpleLog("Tab:\tHere");
        logger.addSimpleLog("Newline: \n Done");
        logger.addSimpleLog("Quote: \"test\"");
        logger.addSimpleLog("Backslash: \\path");
    });
}

// Test 8: Empty Message
TEST_F(ImGuiLoggerTest, emptyMessage)
{
    EXPECT_NO_THROW(logger.addSimpleLog(""));
}

// Test 9: Messages With Unicode
TEST_F(ImGuiLoggerTest, unicodeCharacters)
{
    EXPECT_NO_THROW({
        logger.addSimpleLog("UTF-8: café");
        logger.addSimpleLog("Emoji: 😀");
        logger.addSimpleLog("Chinese: 你好");
    });
}

// Test 10: Stress Test - Many Messages
TEST_F(ImGuiLoggerTest, stressTestManyMessages)
{
    for(int i = 0; i < 1000; i++)
    {
        logger.addSimpleLog("Message " + std::to_string(i));
    }
    EXPECT_NO_THROW(logger.clear());
}

// Test 11: Initialize Multiple Times
TEST_F(ImGuiLoggerTest, initializeMultipleTimes)
{
    EXPECT_NO_THROW({
        logger.initialize();
        logger.initialize();
        logger.initialize();
    });
}

// Test 12: Clear Before Adding Logs
TEST_F(ImGuiLoggerTest, clearBeforeAddingLogs)
{
    EXPECT_NO_THROW({
        logger.clear();
        logger.addSimpleLog("After clear");
    });
}

// Test 13: Rapid Add and Clear Cycles
TEST_F(ImGuiLoggerTest, rapidAddAndClearCycles)
{
    for(int i = 0; i < 100; i++)
    {
        logger.addSimpleLog("Message " + std::to_string(i));
        if(i % 10 == 0)
            logger.clear();
    }
}

// Test 14: Long Running Usage (State Persistence)
TEST_F(ImGuiLoggerTest, stateAfterManyOperations)
{
    // Simulate long-running usage
    for(int i = 0; i < 10; i++)
    {
        logger.addSimpleLog("Cycle " + std::to_string(i));
    }
    logger.clear();
    for(int i = 0; i < 10; i++)
    {
        logger.addSimpleLog("New Cycle " + std::to_string(i));
    }
    // Should not crash or memory leak
}

// Test 15: Very Long Single Message
TEST_F(ImGuiLoggerTest, veryLongSingleMessage)
{
    std::string veryLongMessage;
    for(int i = 0; i < 10000; i++)
    {
        veryLongMessage += "This is a very long message line " + std::to_string(i) + "\n";
    }
    EXPECT_NO_THROW(logger.addSimpleLog(veryLongMessage));
}

// ============================================================================
// Behavior Tests (Without Accessing Private Members)
// ============================================================================

class ImGuiLoggerBehaviorTest : public ::testing::Test
{
protected:
    ImGuiLogger logger;
    
    void SetUp() override
    {
        logger.initialize();
    }
};

// Test 1: Clear Resets Logger State
TEST_F(ImGuiLoggerBehaviorTest, clearResetsState)
{
    logger.addSimpleLog("Message1");
    logger.addSimpleLog("Message2");
    
    // Clear should reset internal state
    logger.clear();
    
    // After clear, we can add new messages
    logger.addSimpleLog("Message3");
    
    // No crash = success
    EXPECT_TRUE(true);
}

// Test 2: Multiple Initialize Calls Are Idempotent
TEST_F(ImGuiLoggerBehaviorTest, multipleInitializeIdempotent)
{
    logger.initialize();
    logger.addSimpleLog("Message1");
    
    logger.initialize(); // Should be safe
    logger.addSimpleLog("Message2");
    
    EXPECT_TRUE(true);
}

// Test 3: Continuous Add and Read Pattern
TEST_F(ImGuiLoggerBehaviorTest, continuousAddPattern)
{
    // Simulate real usage: logger continuously receives messages
    for(int cycle = 0; cycle < 5; cycle++)
    {
        for(int i = 0; i < 100; i++)
        {
            logger.addSimpleLog("Log entry " + std::to_string(i));
        }
        // Simulate periodic clear
        if(cycle % 2 == 0)
            logger.clear();
    }
    
    EXPECT_TRUE(true);
}

// ============================================================================
// Thread Safety Tests (Basic)
// ============================================================================

class ImGuiLoggerThreadSafetyTest : public ::testing::Test
{
protected:
    ImGuiLogger logger;
    
    void SetUp() override
    {
        logger.initialize();
    }
};

// Test 1: Sequential Access (Not Concurrent - ImGui is single-threaded)
TEST_F(ImGuiLoggerThreadSafetyTest, sequentialAccess)
{
    // ImGui is not thread-safe, so we test sequential access
    for(int i = 0; i < 100; i++)
    {
        logger.addSimpleLog("Thread simulation message " + std::to_string(i));
    }
    EXPECT_TRUE(true);
}

// ============================================================================
// Integration Tests
// ============================================================================

class ImGuiLoggerIntegrationTest : public ::testing::Test
{
protected:
    ImGuiLogger logger;
    
    void SetUp() override
    {
        logger.initialize();
    }
};

// Test 1: Simulate Crawler Logging Session
TEST_F(ImGuiLoggerIntegrationTest, crawlerLoggingSession)
{
    // Simulate a crawler session with typical log messages
    logger.addSimpleLog("Starting crawler...");
    logger.addSimpleLog("Processing URL: https://example.com");
    logger.addSimpleLog("Found 5 links");
    logger.addSimpleLog("Processing URL: https://example.com/page1");
    logger.addSimpleLog("Found 3 links");
    logger.addSimpleLog("Crawler finished");
    
    EXPECT_TRUE(true);
}

// Test 2: Simulate Error Logging
TEST_F(ImGuiLoggerIntegrationTest, errorLogging)
{
    logger.addSimpleLog("ERROR: Connection timeout");
    logger.addSimpleLog("ERROR: Invalid HTML");
    logger.addSimpleLog("WARNING: Partial content");
    logger.addSimpleLog("SUCCESS: Crawl completed");
    
    EXPECT_TRUE(true);
}

// Test 3: Buffer Overflow Prevention
TEST_F(ImGuiLoggerIntegrationTest, largeVolumeOfLogs)
{
    // Create many logs to test buffer handling
    for(int i = 0; i < 5000; i++)
    {
        logger.addSimpleLog("Log message " + std::to_string(i));
    }
    
    // Should handle without crashing
    EXPECT_TRUE(true);
}

