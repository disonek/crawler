#include "ImGuiLayer.hpp"

namespace img {

ImGuiLayer::ImGuiLayer(std::shared_ptr<OpenGLModule> module, std::shared_ptr<ImGuiLogger> logger)
    : openGLModule(module)
    , logger(logger)
{
}

void ImGuiLayer::intialize()
{
    openGLModule->initalize();
}

ImGuiLayer::~ImGuiLayer()
{
    openGLModule->shutDown();
}

void ImGuiLayer::consumeLogs(std::set<std::string>&& messages)
{
    for(auto message : messages)
        logger->addSimpleLog(message);
}

void ImGuiLayer::printResultsToImGuiLogger(BasicProtectedQueue<>& taskQueue)
{
    std::lock_guard<std::mutex> lk(taskQueue.mutex);
    if(!taskQueue.tasks.empty())
    {
        consumeLogs(std::move(taskQueue.tasks.front()));
        taskQueue.tasks.pop();
    }
}

void ImGuiLayer::guiThread(BasicProtectedQueue<>& taskQueue)
{
    while(!openGLModule->windowShouldClose())
    {
        openGLModule->startNewFrame();

        bool run = true;
        openGLModule->createDockspace(run);
        printResultsToImGuiLogger(taskQueue);
        logger->draw("Webcreawler", &run);

        openGLModule->render();
    }
}

} // namespace img