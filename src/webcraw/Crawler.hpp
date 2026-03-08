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
    virtual ~Crawler() = default;

    // void crawlLinksFromUrlAndPushToTaskQueue(ProtectedQueue& taskQueue, std::string url);

    // made virtual for easier testing / subclassing
    virtual std::set<std::string> getLinksFromUrl(const std::string startURL);
    virtual std::set<std::string> crawl(std::set<std::string> initialRequests, ProtectedQueue& taskQueue);
    void run(ProtectedQueue& taskQueue);

private:
    std::set<std::string> extractLinks(std::string response, std::string url);
    std::deque<std::string> requestsToDo;
    std::set<std::string> requestsDone;
};

} // namespace webcrawler
