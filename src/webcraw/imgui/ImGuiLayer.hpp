
#include <memory>
#include <set>

#include "ImGuiLogger.hpp"
#include "OpenGLModule.hpp"
#include "SharedObjects.hpp"

namespace img {

class ImGuiLayer
{
public:
    ImGuiLayer(std::shared_ptr<OpenGLModule> module, std::shared_ptr<ImGuiLogger> logger);
    ~ImGuiLayer();
    void guiThread(BasicProtectedQueue<>& taskQueue);
    void intialize();

private:
    void printResultsToImGuiLogger(BasicProtectedQueue<>& taskQueue);
    void consumeLogs(std::set<std::string>&& messages);

    std::shared_ptr<OpenGLModule> openGLModule;
    std::shared_ptr<ImGuiLogger> logger;
};
} // namespace img