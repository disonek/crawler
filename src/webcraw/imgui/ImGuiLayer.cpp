#include "ImGuiLayer.hpp"

namespace img {
ImGuiLayer::ImGuiLayer()
{
    openGLModule.initalize();
}

ImGuiLayer::~ImGuiLayer()
{
    openGLModule.shutDown();
}

void ImGuiLayer::consumeLogs(std::set<std::string>&& messages)
{
    for(auto message : messages)
        logger.AddLog("%s\n", message.c_str());
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
    while(!openGLModule.windowShouldClose())
    {
        openGLModule.startNewFrame();

        bool run = true;
        openGLModule.createDockspace(run);
        printResultsToImGuiLogger(taskQueue);
        logger.Draw("Example: Log", &run);

        openGLModule.render();
    }
}

} // namespace img