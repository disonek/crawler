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
    shutdown();
}

void ImGuiLayer::shutdown()
{
    if(isShutdown)
        return; // Prevent double-shutdown
    
    isShutdown = true;
    logger->clear(); // Clear any remaining logs
    openGLModule->shutDown();
}

void ImGuiLayer::consumeLogs(std::set<std::string>&& messages)
{
    for(auto message : messages)
    {
        logger->addSimpleLog(message);
    }
}

void ImGuiLayer::printResultsToImGuiLogger(ProtectedQueue& taskQueue)
{
    auto response = taskQueue.popResponse();
    if(std::nullopt != response)
        consumeLogs(std::move(response.value()));
}

void ImGuiLayer::run(ProtectedQueue& taskQueue)
{
    std::string link{};
    bool loggerWindowOpen = true; // Persist across frames so window close state is maintained

    while(!openGLModule->windowShouldClose())
    {
        openGLModule->startNewFrame();
        bool dockspaceOpen = true; // Dockspace always open (no close button)
        openGLModule->createDockspace(dockspaceOpen);
        printResultsToImGuiLogger(taskQueue);
        if(loggerWindowOpen && logger->draw("Webcreawler", &loggerWindowOpen, link))
        {
            taskQueue.pushRequest(link);
        }

        openGLModule->render();
    }
    taskQueue.ready.store(true);
}

} // namespace img