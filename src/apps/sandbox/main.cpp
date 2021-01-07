
#include <string>

#include "Crawler.hpp"
#include "SharedObjects.hpp"
#include "imgui/ImGuiLayer.hpp"

int main(int argc, char* argv[])
{
    BasicProtectedQueue taskQueue;

    std::string url = "https://www.google.com/doodles";

    auto crawlerResult = std::async(
        std::launch::async, [&taskQueue, url] { webcrawler::crawlLinksFromUrlAndPushToTaskQueue(taskQueue, url); });

    auto guiResult = std::async(std::launch::async, [&taskQueue] {
        img::ImGuiLayer imGuiLayer{};
        imGuiLayer.guiThread(taskQueue);
    });
}