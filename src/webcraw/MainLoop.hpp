#include <spdlog/spdlog.h>

#include <string>

#include "Crawler.hpp"
#include "ScopedTimer.hpp"
#include "TraceTimer.hpp"
#include "cxxopts.hpp"

int mainLoop(int argc, char* argv[])
{
    {
        utils::ScopedTimer timer(__func__, "main");
        utils::Trace::get().beginSession("Session Name");

        uint8_t numThreads = 15;
        std::string url = "https://www.google.com/doodles";

        auto crawler = webcrawler::Crawler{numThreads};
        std::set<std::string> initialRequests = crawler.getLinksFromUrl(url);
        crawler.crawl(initialRequests);

        utils::Trace::get().endSession();
    }

    utils::registryExampleUsage();

    return 0;
}