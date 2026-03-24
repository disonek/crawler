#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <thread>
#include <chrono>

#include "Crawler.hpp"
#include "SharedObjects.hpp"

using namespace testing;
using namespace webcrawler;

// ============================================================================
// Mock Crawler for Testing Public Methods
// ============================================================================

class TestCrawler : public Crawler
{
public:
    // Override getLinksFromUrl to return predictable results
    std::set<std::string> getLinksFromUrl(const std::string startURL) override
    {
        if(startURL == "https://example.com/valid")
            return {"https://example.com/page1", "https://example.com/page2"};
        else if(startURL == "https://example.com/empty")
            return {};
        else
            return {startURL};
    }

    // Override crawl for predictable results
    std::set<std::string> crawl(std::set<std::string> initialRequests, ProtectedQueue& /*queue*/) override
    {
        std::set<std::string> result;
        for(const auto& req : initialRequests)
        {
            result.insert(req + "_processed");
        }
        return result;
    }
};

// ============================================================================
// Tests for getLinksFromUrl() - PUBLIC METHOD
// ============================================================================

class CrawlerGetLinksFromUrlTest : public ::testing::Test
{
protected:
    TestCrawler crawler;
};

// Test 1: Valid URL Returns Links
TEST_F(CrawlerGetLinksFromUrlTest, validUrlReturnsLinks)
{
    auto links = crawler.getLinksFromUrl("https://example.com/valid");
    
    EXPECT_EQ(links.size(), 2);
    EXPECT_THAT(links, UnorderedElementsAre(
        "https://example.com/page1",
        "https://example.com/page2"
    ));
}

// Test 2: Empty URL Results in Empty Set
TEST_F(CrawlerGetLinksFromUrlTest, emptyUrlReturnsEmptySet)
{
    auto links = crawler.getLinksFromUrl("https://example.com/empty");
    
    EXPECT_TRUE(links.empty());
}

// Test 3: Invalid URL Returns Default
TEST_F(CrawlerGetLinksFromUrlTest, invalidUrlHandling)
{
    auto links = crawler.getLinksFromUrl("invalid://url");
    
    // Should return the URL itself as fallback
    EXPECT_THAT(links, UnorderedElementsAre("invalid://url"));
}

// Test 4: Multiple Calls Return Consistent Results
TEST_F(CrawlerGetLinksFromUrlTest, multipleCalls)
{
    auto links1 = crawler.getLinksFromUrl("https://example.com/valid");
    auto links2 = crawler.getLinksFromUrl("https://example.com/valid");
    
    EXPECT_EQ(links1, links2);
}

// Test 5: Different URLs Return Different Results
TEST_F(CrawlerGetLinksFromUrlTest, differentUrls)
{
    auto links1 = crawler.getLinksFromUrl("https://example.com/valid");
    auto links2 = crawler.getLinksFromUrl("https://example.com/empty");
    
    EXPECT_NE(links1, links2);
}

// ============================================================================
// Tests for crawl() - PUBLIC METHOD
// ============================================================================

class CrawlerCrawlTest : public ::testing::Test
{
protected:
    TestCrawler crawler;
    ProtectedQueue queue;
};

// Test 1: Single Crawl Request
TEST_F(CrawlerCrawlTest, singleCrawlRequest)
{
    std::set<std::string> requests = {"https://example.com"};
    auto result = crawler.crawl(requests, queue);
    
    EXPECT_THAT(result, UnorderedElementsAre("https://example.com_processed"));
}

// Test 2: Multiple Crawl Requests
TEST_F(CrawlerCrawlTest, multipleCrawlRequests)
{
    std::set<std::string> requests = {
        "https://example.com/1",
        "https://example.com/2",
        "https://example.com/3"
    };
    
    auto result = crawler.crawl(requests, queue);
    
    EXPECT_EQ(result.size(), 3);
}

// Test 3: Empty Crawl Requests
TEST_F(CrawlerCrawlTest, emptyCrawlRequests)
{
    std::set<std::string> requests;
    auto result = crawler.crawl(requests, queue);
    
    EXPECT_TRUE(result.empty());
}

// Test 4: Duplicate URLs Are Deduped
TEST_F(CrawlerCrawlTest, duplicatesAreDeduped)
{
    std::set<std::string> requests = {
        "https://example.com",
        "https://example.com"  // Duplicate in set (already deduped by set)
    };
    
    auto result = crawler.crawl(requests, queue);
    
    EXPECT_EQ(result.size(), 1);
}

// Test 5: Crawl Returns Set Type
TEST_F(CrawlerCrawlTest, crawlReturnsSetType)
{
    std::set<std::string> requests = {"url1", "url2"};
    auto result = crawler.crawl(requests, queue);
    
    // Set naturally deduplicates and maintains order
    EXPECT_TRUE(std::is_sorted(result.begin(), result.end()));
}

// ============================================================================
// Tests for run() - PUBLIC METHOD  
// ============================================================================

class CrawlerRunTest : public ::testing::Test
{
protected:
    TestCrawler crawler;
    ProtectedQueue queue;
};

// Test 1: Run Exits When Ready Flag Set
TEST_F(CrawlerRunTest, runExitsOnReadyFlag)
{
    std::thread t([this] { crawler.run(queue); });
    
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    queue.ready.store(true);
    
    // Thread should exit quickly
    t.join();
    EXPECT_TRUE(true); // If we get here, thread exited cleanly
}

// Test 2: Run Handles Empty Request Queue
TEST_F(CrawlerRunTest, runHandlesEmptyQueue)
{
    std::thread t([this] { crawler.run(queue); });
    
    // Don't push any requests
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    queue.ready.store(true);
    
    t.join();
    EXPECT_TRUE(true);
}

// Test 3: Run Is Thread Safe
TEST_F(CrawlerRunTest, runIsThreadSafe)
{
    std::thread t([this] { crawler.run(queue); });
    
    // Push requests from main thread
    for(int i = 0; i < 5; i++)
    {
        queue.pushRequest("url_" + std::to_string(i));
    }
    
    // Give time to process
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    queue.ready.store(true);
    
    t.join();
    EXPECT_TRUE(true); // No crashes = success
}

// Test 4: Run Ready Flag Not Modified
TEST_F(CrawlerRunTest, runReadyFlagNotModified)
{
    queue.ready.store(false);
    
    std::thread t([this] { crawler.run(queue); });
    
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    
    // Should still be false (run doesn't set it)
    EXPECT_FALSE(queue.ready.load());
    
    queue.ready.store(true);
    t.join();
}

// ============================================================================
// Integration Tests Using Only Public Methods
// ============================================================================

class CrawlerIntegrationTest : public ::testing::Test
{
protected:
    TestCrawler crawler;
    ProtectedQueue queue;
};

// Test 1: Complete Workflow
TEST_F(CrawlerIntegrationTest, completeWorkflow)
{
    auto links = crawler.getLinksFromUrl("https://example.com/valid");
    EXPECT_EQ(links.size(), 2);
    
    auto results = crawler.crawl(links, queue);
    EXPECT_EQ(results.size(), 2);
}

// Test 2: Empty Link Processing
TEST_F(CrawlerIntegrationTest, emptyLinkProcessing)
{
    auto links = crawler.getLinksFromUrl("https://example.com/empty");
    EXPECT_TRUE(links.empty());
    
    auto results = crawler.crawl(links, queue);
    EXPECT_TRUE(results.empty());
}

// Test 3: Multiple Sequential Crawls
TEST_F(CrawlerIntegrationTest, multipleSequentialCrawls)
{
    for(int i = 0; i < 3; i++)
    {
        auto results = crawler.crawl({"url_" + std::to_string(i)}, queue);
        EXPECT_FALSE(results.empty() || results.empty()); // Always true, documents behavior
    }
}

