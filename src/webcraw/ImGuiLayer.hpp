#include <spdlog/spdlog.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

// clang-tidy off
#include <GL/glew.h> // Initialize with glewInit()
// Include glfw3.h after our OpenGL definitions
#include <GLFW/glfw3.h>
// clang-tidy on
namespace img {

class ImGuiLayer
{
public:
    ImGuiLayer();
    ~ImGuiLayer();
    void run();

    static void glfw_error_callback(int error, const char* description)
    {
        spdlog::error("Glfw Error {}: {}", error, description);
    }

private:
    void createDockspace(bool& run);
    GLFWwindow* window;
    int screenWidth;
    int screenHeight;
};
} // namespace img