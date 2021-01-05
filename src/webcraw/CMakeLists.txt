find_package(spdlog REQUIRED)
find_package(cpr REQUIRED)
find_package(LibXml2 REQUIRED)
find_package(EnTT REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)


add_library(webcrawl
  Crawler.cpp
  Crawler.hpp
  Console.cpp
  WebCurl.cpp
  WebCurl.hpp
  imgui/ImGuiLayer.hpp
  imgui/ImGuiLayer.cpp
  imgui/ImGuiLogger.hpp 
  imgui/ImGuiLogger.cpp
  debug_tools/Timer.hpp
  debug_tools/Trace.hpp
  debug_tools/TraceTimer.hpp
  debug_tools/ScopedTimer.hpp
  debug_tools/ScopedTimer.cpp
)

target_include_directories(webcrawl 
  PUBLIC . debug_tools imgui)

target_link_libraries(
  webcrawl
  PUBLIC project_options 
         spdlog::spdlog
         Threads::Threads
         cxxopts::cxxopts
         cpr::cpr
         LibXml2::LibXml2
         EnTT::EnTT
         glfw::glfw
         GLEW::GLEW 
         imgui
         )
