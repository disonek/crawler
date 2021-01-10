#include "ImGuiLayer.hpp"

namespace img {

ImGuiLayer::ImGuiLayer(std::shared_ptr<OpenGLModule> module, std::shared_ptr<ImGuiLogger> logger)
    : openGLModule(module)
    , logger(logger)
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
        logger->addLog("%s\n", message.c_str());
    messages.clear();
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
        logger->draw("Example: Log", &run);

        openGLModule->render();
    }
}

} // namespace img