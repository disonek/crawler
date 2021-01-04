#pragma once

#include <deque>
#include <mutex>
#include <queue>
#include <set>
#include <string>
#include <unordered_set>

#include "TaskQueue.hpp"
#include "TraceTimer.hpp"

namespace webcrawler {
class Crawler
{
public:
    static std::set<std::string> CrawlerThread(TaskQueue& taskQueue, std::string url);
    std::set<std::string> getLinksFromUrl(const std::string startURL);
    std::set<std::string> crawl(std::set<std::string> initialRequests);
    Crawler(uint8_t numThreads);

private:
    std::set<std::string> extractLinks(std::string response, std::string url);

    uint8_t numThreads;
    std::deque<std::string> requestsToDo;
    std::set<std::string> requestsDone;
};
} // namespace webcrawler
