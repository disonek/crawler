#pragma once

#include <deque>
#include <mutex>
#include <queue>
#include <set>
#include <string>
#include <unordered_set>

#include "TraceTimer.hpp"

namespace webcrawler {
class Crawler
{
public:
    std::set<std::string> getLinksFromUrl(const std::string startURL);
    void crawl(std::set<std::string> initialRequests);
    Crawler(uint8_t numThreads);

private:
    std::set<std::string> extractLinks(std::string response, std::string url);

    uint8_t numThreads;
    std::deque<std::string> requestsToDo;
    std::set<std::string> requestsDone;
};
} // namespace webcrawler
