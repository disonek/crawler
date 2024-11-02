#pragma once
#include <spdlog/spdlog.h>


// clang-tidy off
#include <GL/glew.h> // Initialize with glewInit()
// Include glfw3.h after our OpenGL definitions
#include <GLFW/glfw3.h>
// clang-tidy on

namespace img {

class OpenGLModule
{
public:
    virtual ~OpenGLModule() = default;
    virtual void initalize();
    virtual bool windowShouldClose();
    virtual void startNewFrame();
    virtual void render();
    virtual void shutDown();
    virtual void createDockspace(bool& run);

    GLFWwindow* window;
    int screenWidth;
    int screenHeight;
};
} // namespace img
