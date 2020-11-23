#include <benchmark/benchmark.h>

#include <string>

#include "Crawler.hpp"
#include "Timer.hpp"

static void basicCrawl(benchmark::State& state)
{
    Timer timer(__func__);
    for(auto _ : state)
    {
        uint64_t numThreads = state.range(0);
        auto crawler = webcrawler::Crawler{static_cast<uint8_t>(numThreads)};

        spdlog::warn("Number of threads {}", numThreads);
        spdlog::info("Initial request");
        std::set<std::string> initialRequests = crawler.getLinksFromUrl("https://www.google.com/doodles");

        spdlog::info("Crawl");
        crawler.crawl(initialRequests);
    }
}
BENCHMARK(basicCrawl)->Unit(benchmark::kMillisecond)->DenseRange(7, 16, 1)->MeasureProcessCPUTime();
BENCHMARK_MAIN();
