
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

    auto something = taskQueue.postTaskForGuiThread(webcrawler::Crawler::CrawlerThread);

    img::ImGuiLayer imGuiLayer{};
    // imGuiLayer.setLogMessages(result);
    imGuiLayer.guiThread(taskQueue);
    auto results = something.get();

    for(auto result : results)
        spdlog::error("{}", result.c_str());

    utils::registryExampleUsage();
}