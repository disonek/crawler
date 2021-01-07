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

class OpenGLModule
{
public:
    void initalize();
    bool windowShouldClose();
    void startNewFrame();
    void render();
    void shutDown();
    void createDockspace(bool& run);

    GLFWwindow* window;
    int screenWidth;
    int screenHeight;
};
} // namespace img