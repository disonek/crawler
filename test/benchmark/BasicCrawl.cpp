#include <benchmark/benchmark.h>
#include <spdlog/spdlog.h>

#include <string>

#include "Crawler.hpp"

static void basicCrawl(benchmark::State& state)
{
    for(auto _ : state)
    {
        uint64_t numThreads = state.range(0);
        auto crawler = webcrawler::Crawler{};
        ProtectedQueue taskQueue;

        spdlog::warn("Number of threads {}", numThreads);
        spdlog::info("Initial request");

        std::set<std::string> initialRequests = crawler.getLinksFromUrl("https://www.google.com/doodles");

        spdlog::info("Crawl");
        crawler.crawl(initialRequests, taskQueue);
    }
}
BENCHMARK(basicCrawl)->Unit(benchmark::kMillisecond)->DenseRange(7, 16, 1)->MeasureProcessCPUTime();
BENCHMARK_MAIN();
