#pragma once

#include <deque>
#include <mutex>
#include <set>
#include <string>

#include "SharedObjects.hpp"
#include "TraceTimer.hpp"

namespace webcrawler {
class Crawler
{
public:
    std::set<std::string> getLinksFromUrl(const std::string startURL);
    Crawler(uint8_t numThreads);
    std::set<std::string> crawl(std::set<std::string> initialRequests);

private:
    std::set<std::string> extractLinks(std::string response, std::string url);
    uint8_t numThreads;
    std::deque<std::string> requestsToDo;
    std::set<std::string> requestsDone;
};

void crawlLinksFromUrlAndPushToTaskQueue(BasicProtectedQueue<>& taskQueue, std::string url);

} // namespace webcrawler
