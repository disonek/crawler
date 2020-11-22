#include <spdlog/spdlog.h>

#include <string>

#include "Crawler.hpp"
#include "Timer.hpp"
#include "Trace.hpp"
#include "cxxopts.hpp"

int main(int argc, char* argv[])
{
    Timer timer(__func__);
    utils::Instrumentor::session("main");
    uint8_t numThreads = 10;
    std::string url;
    bool needHelp = false;
    bool printVersion = false;
    std::string link;

    cxxopts::Options options(argv[0]);
    options.add_options()("h, help", "Get help message", cxxopts::value<bool>(needHelp))(
        "t, threads", "Set amount of threads (default 4)", cxxopts::value<uint8_t>(numThreads))(
        "a, link", "Set the link to begin downloading from", cxxopts::value<std::string>(link));
    options.parse(argc, argv);

    if(needHelp)
    {
        spdlog::info("{}", options.help().c_str());
        return 0;
    }
    if(!link.empty())
    {
        url = link;
    }
    else
    {
        url = link;
    }

    auto crawler = webcrawler::Crawler{numThreads};

    spdlog::warn("Number of threads {}", numThreads);
    spdlog::info("Initial request");
    std::set<std::string> initialRequests = crawler.getLinksFromUrl(url);

    spdlog::info("Crawl");
    crawler.crawl(initialRequests);

    utils::Instrumentor::endSession();
    return 0;
}
