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
    // void crawlLinksFromUrlAndPushToTaskQueue(ProtectedQueue& taskQueue, std::string url);

    std::set<std::string> getLinksFromUrl(const std::string startURL);
    std::set<std::string> crawl(std::set<std::string> initialRequests, ProtectedQueue& taskQueue);
    void run(ProtectedQueue& taskQueue);

private:
    std::set<std::string> extractLinks(std::string response, std::string url);
    std::deque<std::string> requestsToDo;
    std::set<std::string> requestsDone;
};

} // namespace webcrawler
