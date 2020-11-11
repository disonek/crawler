#pragma once

#include <deque>
#include <mutex>
#include <queue>
#include <set>
#include <string>
#include <unordered_set>

namespace webcrawler {
class Crawler
{
public:
    std::set<std::string> getLinksFromUrl(const std::string startURL);
    void crawl(std::set<std::string> initialRequests);

private:
    std::set<std::string> extractLinks(std::string response, std::string url);
    std::mutex mutex;
    std::deque<std::string> requestsToDo;
    std::set<std::string> requestsDone;
};
} // namespace webcrawler
