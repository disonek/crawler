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
    auto response = taskQueue.popResponse();
    if(std::nullopt != response)
        consumeLogs(std::move(response.value()));
}

void ImGuiLayer::guiThread(BasicProtectedQueue<>& taskQueue)
{
    while(!openGLModule->windowShouldClose())
    {
        openGLModule->startNewFrame();

        bool run = true;
        openGLModule->createDockspace(run);
        printResultsToImGuiLogger(taskQueue);
        if(logger->draw("Webcreawler", &run))
        {
            taskQueue.pushRequest("https://www.google.com/doodle");
        }

        openGLModule->render();
    }
}

} // namespace img