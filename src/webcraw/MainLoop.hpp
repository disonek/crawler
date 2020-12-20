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
            url = "https://www.google.com/doodles";
            // url = "https://www.google.com/search?q=starcraft";
        }

        auto crawler = webcrawler::Crawler{numThreads};

        spdlog::warn("Number of threads {}", numThreads);
        spdlog::info("Initial request");
        std::set<std::string> initialRequests = crawler.getLinksFromUrl(url);

        spdlog::info("Crawl");
        crawler.crawl(initialRequests);

        utils::Trace::get().endSession();
    }

    double getPageDuraton{};
    double parseDuration{};

    utils::Registry reg;
    reg.get().view<utils::Duration, utils::Tag>().each(
        [&getPageDuraton, &parseDuration](auto entity, auto& duration, auto& tag) {
            spdlog::warn("{} duraton = {}", tag, duration);
            if(tag == "getPage")
                getPageDuraton += duration;
            if(tag == "extractLinks")
                parseDuration += duration;
        });

    spdlog::error("Downloat time = {}", getPageDuraton);
    spdlog::error("Parse time = {}", parseDuration);
    return 0;
}