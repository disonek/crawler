
#include <set>

#include "ImGuiLogger.hpp"
#include "OpenGLModule.hpp"
#include "SharedObjects.hpp"


namespace img {

class ImGuiLayer
{
public:
    ImGuiLayer();
    ~ImGuiLayer();
    void guiThread(BasicProtectedQueue<>& taskQueue);

private:
    void printResultsToImGuiLogger(BasicProtectedQueue<>& taskQueue);
    void consumeLogs(std::set<std::string>&& messages);

    OpenGLModule openGLModule;
    ImGuiAppLog logger;
};
} // namespace img