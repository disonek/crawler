
#include <string>

#include "Crawler.hpp"
#include "SharedObjects.hpp"
#include "imgui/ImGuiLayer.hpp"

int main(int argc, char* argv[])
{
    BasicProtectedQueue taskQueue;

    webcrawler::Crawler crawler;
    auto crawlerResult = std::async(std::launch::async, [&taskQueue, &crawler] { crawler.run(taskQueue); });

    auto guiResult = std::async(std::launch::async, [&taskQueue] {
        img::ImGuiLayer imGuiLayer{std::make_shared<img::OpenGLModule>(), std::make_shared<img::ImGuiLogger>()};
        imGuiLayer.intialize();
        imGuiLayer.guiThread(taskQueue);
    });
}

// int main(int argc, char* argv[])
// {
//     webcrawler::Crawler crawler{};
//     img::ImGuiLayer imGuiLayer{
//         std::make_shared<img::OpenGLModule>(), std::make_shared<img::ImGuiLogger>(), crawler.getSender()};
//     crawler.setSender(imGuiLayer.getSender());

//     auto crawlerResult = std::async(std::launch::async, [&crawler] { crawler.run(); });

//     auto guiResult = std::async(std::launch::async, [&imGuiLayer, &crawler] {
//         imGuiLayer.intialize();
//         imGuiLayer.run();
//     });
// }