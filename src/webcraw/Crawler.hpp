#pragma once

#include <gumbo.h>

#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <unordered_set>
#include <vector>

class ThreadPool;

namespace webcrawler {
class Crawler
{
private:
    static void extractLinks(GumboNode* node, std::vector<std::string>& foundLinks, const std::string& relativeToUrl);
    void crawl(const std::string& url);

public:
    explicit Crawler(size_t numThreads);
    ~Crawler();
    void start(const std::string& startURL);
    void stop();

private:
    bool stopped = false;
    std::unique_ptr<ThreadPool> pool;
    std::queue<std::string> urlPool;
    std::unordered_set<std::string> foundURLs;
    std::condition_variable urlsInPool;
    std::mutex url_mut;
    std::mutex found_mut;
};
} // namespace webcrawler
