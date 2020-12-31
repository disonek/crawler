#include <spdlog/spdlog.h>

#include "cxxopts.hpp"

static void consoleExample(int argc, char* argv[])
{
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
}