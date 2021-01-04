
#include <string>

#include "Crawler.hpp"
#include "ScopedTimer.hpp"
#include "TaskQueue.hpp"
#include "TraceTimer.hpp"
#include "imgui/ImGuiLayer.hpp"


int main(int argc, char* argv[])
{
    TaskQueue taskQueue;
    // std::string url = "http://h3.heroes.net.pl/";
    std::string url = "https://www.google.com/doodles";

    auto crawlerResult = std::async(std::launch::async,
                                    [&taskQueue, url] { return webcrawler::Crawler::CrawlerThread(taskQueue, url); });

    auto guiResult = std::async(std::launch::async, [&taskQueue] {
        img::ImGuiLayer imGuiLayer{};
        imGuiLayer.guiThread(taskQueue);
    });

    // guiResult.get();

    // crawlerResult.get();

    // for(auto result : results)
    //     spdlog::error("{}", result.c_str());
}