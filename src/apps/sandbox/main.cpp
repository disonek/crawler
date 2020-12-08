// #include <spdlog/spdlog.h>

// #include <string>

// #include "Crawler.hpp"
// #include "ScopedTimer.hpp"
// #include "TraceTimer.hpp"
// #include "cxxopts.hpp"

// int main(int argc, char* argv[])
// {
//     {
//         utils::ScopedTimer timer(__func__, "main");
//         utils::Trace::get().beginSession("Session Name");
//         uint8_t numThreads = 15;
//         std::string url;
//         bool needHelp = false;
//         bool printVersion = false;
//         std::string link;

//         cxxopts::Options options(argv[0]);
//         options.add_options()("h, help", "Get help message", cxxopts::value<bool>(needHelp))(
//             "t, threads", "Set amount of threads (default 4)", cxxopts::value<uint8_t>(numThreads))(
//             "a, link", "Set the link to begin downloading from", cxxopts::value<std::string>(link));
//         options.parse(argc, argv);

//         if(needHelp)
//         {
//             spdlog::info("{}", options.help().c_str());
//             return 0;
//         }
//         if(!link.empty())
//         {
//             url = link;
//         }
//         else
//         {
//             url = "https://www.google.com/doodles";
//             // url = "https://www.google.com/search?q=starcraft";
//         }

//         auto crawler = webcrawler::Crawler{numThreads};

//         spdlog::warn("Number of threads {}", numThreads);
//         spdlog::info("Initial request");
//         std::set<std::string> initialRequests = crawler.getLinksFromUrl(url);

//         spdlog::info("Crawl");
//         crawler.crawl(initialRequests);

//         utils::Trace::get().endSession();
//     }

//     double getPageDuraton{};
//     double parseDuration{};

//     utils::Registry reg;
//     reg.get().view<utils::Duration, utils::Tag>().each(
//         [&getPageDuraton, &parseDuration](auto entity, auto& duration, auto& tag) {
//             spdlog::warn("{} duraton = {}", tag, duration);
//             if(tag == "getPage")
//                 getPageDuraton += duration;
//             if(tag == "extractLinks")
//                 parseDuration += duration;
//         });

//     spdlog::error("Downloat time = {}", getPageDuraton);
//     spdlog::error("Parse time = {}", parseDuration);
//     return 0;
// }

#include <stdio.h>

#include <iostream>
#include <vector>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

// clang-tidy off
#include <GL/glew.h> // Initialize with glewInit()
// Include glfw3.h after our OpenGL definitions
#include <GLFW/glfw3.h>

// clang-tidy on

#define PI 3.14159265358979323846

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

void render_conan_logo()
{
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    float sz = 300.0f;
    static ImVec4 col1 = ImVec4(68.0 / 255.0, 83.0 / 255.0, 89.0 / 255.0, 1.0f);
    static ImVec4 col2 = ImVec4(40.0 / 255.0, 60.0 / 255.0, 80.0 / 255.0, 1.0f);
    static ImVec4 col3 = ImVec4(50.0 / 255.0, 65.0 / 255.0, 82.0 / 255.0, 1.0f);
    static ImVec4 col4 = ImVec4(20.0 / 255.0, 40.0 / 255.0, 60.0 / 255.0, 1.0f);
    const ImVec2 p = ImGui::GetCursorScreenPos();
    float x = p.x + 4.0f, y = p.y + 4.0f;
    draw_list->AddQuadFilled(ImVec2(x, y + 0.25 * sz),
                             ImVec2(x + 0.5 * sz, y + 0.5 * sz),
                             ImVec2(x + sz, y + 0.25 * sz),
                             ImVec2(x + 0.5 * sz, y),
                             ImColor(col1));
    draw_list->AddQuadFilled(ImVec2(x, y + 0.25 * sz),
                             ImVec2(x + 0.5 * sz, y + 0.5 * sz),
                             ImVec2(x + 0.5 * sz, y + 1.0 * sz),
                             ImVec2(x, y + 0.75 * sz),
                             ImColor(col2));
    draw_list->AddQuadFilled(ImVec2(x + 0.5 * sz, y + 0.5 * sz),
                             ImVec2(x + sz, y + 0.25 * sz),
                             ImVec2(x + sz, y + 0.75 * sz),
                             ImVec2(x + 0.5 * sz, y + 1.0 * sz),
                             ImColor(col3));
    draw_list->AddLine(ImVec2(x + 0.75 * sz, y + 0.375 * sz), ImVec2(x + 0.75 * sz, y + 0.875 * sz), ImColor(col4));
    draw_list->AddBezierCurve(ImVec2(x + 0.72 * sz, y + 0.24 * sz),
                              ImVec2(x + 0.68 * sz, y + 0.15 * sz),
                              ImVec2(x + 0.48 * sz, y + 0.13 * sz),
                              ImVec2(x + 0.39 * sz, y + 0.17 * sz),
                              ImColor(col4),
                              10,
                              18);
    draw_list->AddBezierCurve(ImVec2(x + 0.39 * sz, y + 0.17 * sz),
                              ImVec2(x + 0.2 * sz, y + 0.25 * sz),
                              ImVec2(x + 0.3 * sz, y + 0.35 * sz),
                              ImVec2(x + 0.49 * sz, y + 0.38 * sz),
                              ImColor(col4),
                              10,
                              18);
}

void create_triangle(unsigned int& vbo, unsigned int& vao, unsigned int& ebo)
{
    // create the triangle
    float triangle_vertices[] = {
        0.0f,
        0.25f,
        0.0f, // position vertex 1
        1.0f,
        0.0f,
        0.0f, // color vertex 1
        0.25f,
        -0.25f,
        0.0f, // position vertex 1
        0.0f,
        1.0f,
        0.0f, // color vertex 1
        -0.25f,
        -0.25f,
        0.0f, // position vertex 1
        0.0f,
        0.0f,
        1.0f, // color vertex 1
    };
    unsigned int triangle_indices[] = {0, 1, 2};
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertices), triangle_vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangle_indices), triangle_indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

int main(int, char**)
{
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if(!glfwInit())
        return 1;

    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Dear ImGui - Conan", NULL, NULL);
    if(window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    bool err = glewInit() != GLEW_OK;

    if(err)
    {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        return 1;
    }

    int screen_width, screen_height;
    glfwGetFramebufferSize(window, &screen_width, &screen_height);
    glViewport(0, 0, screen_width, screen_height);

    // create our geometries
    unsigned int vbo, vao, ebo;
    create_triangle(vbo, vao, ebo);

    // init shader

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);

        // feed inputs to dear imgui, start new frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // render your GUI
        ImGui::Begin("Triangle Position/Color");
        static float rotation = 0.0;
        ImGui::SliderFloat("rotation", &rotation, 0, 2 * PI);
        static float translation[] = {0.0, 0.0};
        ImGui::SliderFloat2("position", translation, -1.0, 1.0);
        static float color[4] = {1.0f, 1.0f, 1.0f, 1.0f};
        // pass the parameters to the shader
        // color picker

        ImGui::ColorEdit3("color", color);
        // multiply triangle's color with this color
        ImGui::End();

        ImGui::Begin("Conan logo");
        render_conan_logo();
        ImGui::End();
        // Render dear imgui into screen
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}