#include <spdlog/spdlog.h>

#include <string>

#include "Crawler.hpp"
#include "Timer.hpp"
#include "cxxopts.hpp"

int main(int argc, char* argv[])
{
    Timer timer("main timer");
    int numThreads = 4;
    std::string url;
    bool needHelp = false;
    bool printVersion = false;
    std::string link;

    cxxopts::Options options(argv[0]);
    options.add_options()("h, help", "Get help message", cxxopts::value<bool>(needHelp))(
        "t, threads", "Set amount of threads (default 4)", cxxopts::value<int>(numThreads))(
        "a, link", "Set the link to begin downloading from", cxxopts::value<std::string>(link));
    options.parse(argc, argv);

    if(needHelp)
    {
        spdlog::info("{}", options.help().c_str());
        return 0;
    }
    else if(!link.empty())
    {
        url = link;
    }

    auto crawler = webcrawler::Crawler{};
    std::set<std::string> initialRequests = crawler.getLinksFromUrl(url);
    crawler.crawl(initialRequests);

    return 0;
}
