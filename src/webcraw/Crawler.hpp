#pragma once

#include <mutex>
#include <queue>
#include <string>
#include <unordered_set>

namespace webcrawler {
class Crawler
{
public:
    void start(const std::string& startURL);

    void crawl();

private:
    std::unordered_set<std::string> extractLinks(std::string response, std::string url);
    std::mutex mutex;
    std::queue<std::string> requests;
};
} // namespace webcrawler
