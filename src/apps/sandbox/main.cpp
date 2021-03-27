
#include <string>

#include "Crawler.hpp"
#include "SharedObjects.hpp"
#include "imgui/ImGuiLayer.hpp"

int main(int argc, char* argv[])
{
    ProtectedQueue taskQueue;

    webcrawler::Crawler crawler;
    img::ImGuiLayer imGuiLayer{std::make_shared<img::OpenGLModule>(), std::make_shared<img::ImGuiLogger>()};

    auto guiResult = std::async(std::launch::async, [&taskQueue, &imGuiLayer] {
        imGuiLayer.intialize();
        imGuiLayer.run(taskQueue);
    });

    auto crawlerResult = std::async(std::launch::async, [&taskQueue, &crawler] { crawler.run(taskQueue); });

    guiResult.get();
    crawlerResult.get();
}
