
#include <string>

#include "Crawler.hpp"
#include "SharedObjects.hpp"
#include "imgui/ImGuiLayer.hpp"

void runByAsync()
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

void runByThreads()
{
    ProtectedQueue taskQueue;

    webcrawler::Crawler crawler;
    img::ImGuiLayer imGuiLayer{std::make_shared<img::OpenGLModule>(), std::make_shared<img::ImGuiLogger>()};
    std::thread crawlerThread(&webcrawler::Crawler::run, &crawler, std::ref(taskQueue));

    std::thread imGuiLayerThread([&taskQueue, &imGuiLayer]() {
        imGuiLayer.intialize();
        imGuiLayer.run(taskQueue);
    });

    crawlerThread.join();
    imGuiLayerThread.join();
}

void runInTerminalModeTryout()
{
    ProtectedQueue taskQueue;
    webcrawler::Crawler crawler;
    auto crawlerResult = std::async(std::launch::async, [&taskQueue, &crawler] { crawler.run(taskQueue); });
    crawlerResult.get();
}


int main(int argc, char* argv[])
{
    runByThreads();
}
