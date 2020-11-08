

#include <algorithm>
#include <cassert>
#include <csignal>
#include <cstdio>
#include <iostream>
#include <string>

#include "Crawler.hpp"
#include "cxxopts.hpp"

//#define _AGENT      "WebCrawlerBot"

// TODO
void exit_handler(int s)
{
    printf("Caught signal %d\n", s);
    exit(1);
}

int main(int argc, char* argv[])
{
    int numThreads = 4;
    std::string url;
    bool needHelp = false;
    bool printVersion = false;
    std::string searchParam;
    std::string link;

    cxxopts::Options options(argv[0]);
    options.add_options()("h, help", "Get help message", cxxopts::value<bool>(needHelp))(
        "v, version", "Get version", cxxopts::value<bool>(printVersion))(
        "t, threads", "Set amount of threads (default 4)", cxxopts::value<int>(numThreads))(
        "a, link", "Set the link to begin downloading from", cxxopts::value<std::string>(link))(
        "s, search", "Search for links inside the database", cxxopts::value<std::string>(searchParam));
    options.parse(argc, argv);

    if(needHelp)
    {
        puts(options.help().c_str());
        return 0;
    }
    if(printVersion)
    {
        puts("WebCrawler 1.0");
        return 0;
    }
    if(!searchParam.empty())
    {
    }
    else if(!link.empty())
    {
        url = link;
    }

    webcrawler::Crawler crawler(numThreads);
    crawler.start(url);
    return EXIT_SUCCESS;
}
