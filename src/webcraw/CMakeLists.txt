find_package(cpr REQUIRED)
find_package(LibXml2 REQUIRED)
find_package(EnTT REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(boost REQUIRED)
find_package(spdlog REQUIRED)

add_library(imgui STATIC)
target_sources(
  imgui
  PUBLIC    ${CMAKE_SOURCE_DIR}/external/imgui/imgui_widgets.cpp
            ${CMAKE_SOURCE_DIR}/external/imgui/imgui_demo.cpp
  INTERFACE ${CMAKE_SOURCE_DIR}/external/imgui/imconfig.h
            ${CMAKE_SOURCE_DIR}/external/imgui/imgui.h
            ${CMAKE_SOURCE_DIR}/external/imgui/imgui.cpp
            ${CMAKE_SOURCE_DIR}/external/imgui/imgui_draw.cpp
            ${CMAKE_SOURCE_DIR}/external/imgui/examples/imgui_impl_glfw.cpp
            ${CMAKE_SOURCE_DIR}/external/imgui/examples/imgui_impl_opengl3.cpp 
            ${CMAKE_SOURCE_DIR}/external/imgui/imgui_internal.h
            ${CMAKE_SOURCE_DIR}/external/imgui/imstb_rectpack.h
            ${CMAKE_SOURCE_DIR}/external/imgui/imstb_textedit.h
            ${CMAKE_SOURCE_DIR}/external/imgui/imstb_truetype.h)

target_include_directories(imgui INTERFACE  ${CMAKE_SOURCE_DIR}/external/imgui/examples ${CMAKE_SOURCE_DIR}/external/imgui)

add_library(webcrawl
  Crawler2.cpp
  Crawler2.hpp
  urls_large_data.cpp
  urls_large_data.hpp
  
  Crawler.cpp
  Crawler.hpp
  Console.cpp
  WebCurl.cpp
  WebCurl.hpp
  imgui/OpenGLModule.hpp
  imgui/OpenGLModule.cpp
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
  PUBLIC . debug_tools imgui messanger)

target_compile_definitions(webcrawl
  PRIVATE
      #from  https://curl.se/docs/caextract.html
      TESTS_CACERT_PEM="${CMAKE_CURRENT_SOURCE_DIR}/cacert.pem"
)
target_include_directories(webcrawl PUBLIC ${glfw3_INCLUDE_DIRS})

target_link_libraries(
  webcrawl
  PUBLIC spdlog::spdlog
         Threads::Threads
         cxxopts::cxxopts
         cpr::cpr
         LibXml2::LibXml2
         EnTT::EnTT
         glfw
         GLEW::GLEW 
         Boost::system
         imgui
)
