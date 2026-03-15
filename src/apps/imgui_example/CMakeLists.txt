find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

add_executable(imgui_example)

target_sources(imgui_example PRIVATE ${CMAKE_SOURCE_DIR}/external/imgui/examples/example_glfw_opengl3/main.cpp)

target_include_directories(imgui_example PUBLIC ${CMAKE_SOURCE_DIR}/external/imgui/backends ${CMAKE_SOURCE_DIR}/external/imgui ${glfw3_INCLUDE_DIRS})
target_link_libraries(imgui_example PRIVATE imgui GLEW::GLEW glfw)
