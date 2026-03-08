#include <gtest/gtest.h>
#include <thread>
#include <chrono>

#include "Crawler.hpp"
#include "SharedObjects.hpp"
#include <gmock/gmock.h> // for matchers such as UnorderedElementsAre

using namespace testing;

using namespace testing;

// dummy crawler that doesn't perform any network I/O.  We override the
// crawl() method so we have deterministic output; the base implementation
// contains a fair amount of concurrency and also touches WebCurl which is
// hard to mock without introducing more infrastructure.
class DummyCrawler : public webcrawler::Crawler
{
public:
    // avoid doing any I/O; simply echo the URL back so that run() has
    // something to feed into crawl().
    std::set<std::string> getLinksFromUrl(const std::string startURL) override
    {
        return {startURL};
    }

    std::set<std::string> crawl(std::set<std::string> initialRequests, ProtectedQueue& /*taskQueue*/) override
    {
        std::set<std::string> out;
        for (auto const& req : initialRequests)
        {
            out.insert(req + "_ok");
        }
        return out;
    }
};

// helper that polls the response queue until a value is available or a
// timeout expires.  This keeps the tests from flaking due to scheduling
// differences.
static std::optional<ProtectedQueue::Response> waitForResponse(ProtectedQueue& q,
                                                               std::chrono::milliseconds timeout = std::chrono::milliseconds(200))
{
    auto deadline = std::chrono::steady_clock::now() + timeout;
    while (std::chrono::steady_clock::now() < deadline)
    {
        auto resp = q.popResponse();
        if (resp)
            return resp;
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    return std::nullopt;
}

TEST(CrawlerTests, canHandleMultipleRequests)
{
    ProtectedQueue q;
    DummyCrawler crawler;

    // start the crawler in its own thread.  it will keep running until we
    // explicitly set the ready flag.
    std::thread t([&] { crawler.run(q); });

    q.pushRequest("first");
    q.pushRequest("second");

    auto r1 = waitForResponse(q);
    ASSERT_TRUE(r1.has_value());
    EXPECT_THAT(r1.value(), UnorderedElementsAre("first_ok"));

    auto r2 = waitForResponse(q);
    ASSERT_TRUE(r2.has_value());
    EXPECT_THAT(r2.value(), UnorderedElementsAre("second_ok"));

    // tell the crawler to exit and join the thread
    q.ready.store(true);
    t.join();
}
