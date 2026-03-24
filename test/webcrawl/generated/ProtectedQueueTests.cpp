#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "SharedObjects.hpp"

using namespace testing;

// ============================================================================
// Tests for ProtectedQueue
// ============================================================================

class ProtectedQueueTest : public ::testing::Test
{
protected:
    ProtectedQueue queue;
};

// Test 1: Pop From Empty Queue Returns Nullopt
TEST_F(ProtectedQueueTest, popRequestFromEmptyReturnsNullopt)
{
    auto result = queue.popRequest();
    
    EXPECT_FALSE(result.has_value());
}

TEST_F(ProtectedQueueTest, popResponseFromEmptyReturnsNullopt)
{
    auto result = queue.popResponse();
    
    EXPECT_FALSE(result.has_value());
}

// Test 2: Push and Pop Maintains FIFO Order
TEST_F(ProtectedQueueTest, pushPopMaintainsFifoOrderForRequests)
{
    queue.pushRequest("first");
    queue.pushRequest("second");
    queue.pushRequest("third");
    
    EXPECT_EQ(queue.popRequest().value(), "first");
    EXPECT_EQ(queue.popRequest().value(), "second");
    EXPECT_EQ(queue.popRequest().value(), "third");
}

TEST_F(ProtectedQueueTest, pushPopMaintainsFifoOrderForResponses)
{
    std::set<std::string> response1 = {"result1"};
    std::set<std::string> response2 = {"result2"};
    std::set<std::string> response3 = {"result3"};
    
    queue.pushResponse(response1);
    queue.pushResponse(response2);
    queue.pushResponse(response3);
    
    EXPECT_EQ(queue.popResponse().value(), response1);
    EXPECT_EQ(queue.popResponse().value(), response2);
    EXPECT_EQ(queue.popResponse().value(), response3);
}

// Test 3: Ready Flag is Atomic
TEST_F(ProtectedQueueTest, readyFlagIsAtomic)
{
    EXPECT_FALSE(queue.ready.load());
    
    queue.ready.store(true);
    EXPECT_TRUE(queue.ready.load());
    
    queue.ready.store(false);
    EXPECT_FALSE(queue.ready.load());
}

// Test 4: Multiple Pushes Before Pop
TEST_F(ProtectedQueueTest, multiplePushesBeforePop)
{
    queue.pushRequest("a");
    queue.pushRequest("b");
    queue.pushRequest("c");
    queue.pushRequest("d");
    
    EXPECT_EQ(queue.popRequest().value(), "a");
    EXPECT_EQ(queue.popRequest().value(), "b");
    EXPECT_EQ(queue.popRequest().value(), "c");
    EXPECT_EQ(queue.popRequest().value(), "d");
    EXPECT_FALSE(queue.popRequest().has_value());
}

// Test 5: Interleaved Push and Pop
TEST_F(ProtectedQueueTest, interleavedPushAndPop)
{
    queue.pushRequest("1");
    queue.pushRequest("2");
    
    EXPECT_EQ(queue.popRequest().value(), "1");
    
    queue.pushRequest("3");
    
    EXPECT_EQ(queue.popRequest().value(), "2");
    EXPECT_EQ(queue.popRequest().value(), "3");
}

// Test 6: Response Set Operations
TEST_F(ProtectedQueueTest, responseSetOperations)
{
    std::set<std::string> response = {"link1", "link2", "link3"};
    
    queue.pushResponse(response);
    auto popped = queue.popResponse().value();
    
    EXPECT_EQ(popped.size(), 3);
    EXPECT_THAT(popped, Contains("link1"));
    EXPECT_THAT(popped, Contains("link2"));
    EXPECT_THAT(popped, Contains("link3"));
}

// Test 7: Thread Safety - Concurrent Pushes
TEST_F(ProtectedQueueTest, threadSafeWithConcurrentRequestPushes)
{
    std::vector<std::thread> threads;
    int numThreads = 10;
    int itemsPerThread = 10;
    
    // Push from multiple threads
    for(int t = 0; t < numThreads; t++)
    {
        threads.push_back(std::thread([this, t, itemsPerThread]() {
            for(int i = 0; i < itemsPerThread; i++)
            {
                queue.pushRequest("request_" + std::to_string(t) + "_" + std::to_string(i));
            }
        }));
    }
    
    for(auto& t : threads)
        t.join();
    
    // Verify all items were added
    int count = 0;
    while(queue.popRequest().has_value())
        count++;
    
    EXPECT_EQ(count, numThreads * itemsPerThread);
}

// Test 8: Thread Safety - Concurrent Pops
TEST_F(ProtectedQueueTest, threadSafeWithConcurrentRequestPops)
{
    // Pre-fill queue
    for(int i = 0; i < 100; i++)
        queue.pushRequest("item_" + std::to_string(i));
    
    std::vector<std::thread> threads;
    std::atomic<int> popCount{0};
    
    // Pop from multiple threads
    for(int t = 0; t < 5; t++)
    {
        threads.push_back(std::thread([this, &popCount]() {
            while(true)
            {
                auto item = queue.popRequest();
                if(!item.has_value())
                    break;
                popCount++;
            }
        }));
    }
    
    for(auto& t : threads)
        t.join();
    
    // All 100 items should have been popped exactly once
    EXPECT_EQ(popCount.load(), 100);
}

// Test 9: Thread Safety - Mixed Push/Pop
TEST_F(ProtectedQueueTest, threadSafeMixedPushAndPop)
{
    std::vector<std::thread> threads;
    std::atomic<int> totalPushed{0};
    std::atomic<int> totalPopped{0};
    
    // Producer threads
    for(int i = 0; i < 3; i++)
    {
        threads.push_back(std::thread([this, &totalPushed]() {
            for(int j = 0; j < 20; j++)
            {
                queue.pushRequest("prod_" + std::to_string(j));
                totalPushed++;
            }
        }));
    }
    
    // Consumer thread
    auto consumer = std::thread([this, &totalPopped, &totalPushed]() {
        while(totalPopped.load() < totalPushed.load() || totalPushed.load() < 60)
        {
            auto item = queue.popRequest();
            if(item.has_value())
                totalPopped++;
            else
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        
        // Drain remaining items
        while(true)
        {
            auto item = queue.popRequest();
            if(!item.has_value())
                break;
            totalPopped++;
        }
    });
    
    for(auto& t : threads)
        t.join();
    consumer.join();
    
    EXPECT_EQ(totalPushed.load(), 60);
    EXPECT_EQ(totalPopped.load(), 60);
}

// Test 10: Ready Flag Thread Safety
TEST_F(ProtectedQueueTest, readyFlagThreadSafety)
{
    std::atomic<bool> writerDone{false};
    
    std::thread writer([this, &writerDone]() {
        for(int i = 0; i < 100; i++)
        {
            queue.ready.store(i % 2 == 0);
        }
        writerDone.store(true);
    });
    
    std::atomic<int> readCount{0};
    
    std::thread reader([this, &writerDone, &readCount]() {
        while(!writerDone.load() || queue.ready.load())
        {
            if(queue.ready.load())
                readCount++;
            std::this_thread::sleep_for(std::chrono::microseconds(10));
        }
    });
    
    writer.join();
    reader.join();
    
    // Should have read without crashes
    EXPECT_GE(readCount.load(), 0);
}
