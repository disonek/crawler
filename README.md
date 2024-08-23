TO DO: <br />
Error Handling<br />
refactor, check what is to refactor (ImGuiLogger)<br />
type link from UI - better solution than is currently<br />

{
    "cmake.configureArgs": [
        "-DCMAKE_PROJECT_TOP_LEVEL_INCLUDES=conan_provider.cmake"
    ],
}

git submodule update --init --recursive

mkdir build
cmake -B build -S . -DCMAKE_PROJECT_TOP_LEVEL_INCLUDES="conan_provider.cmake" -DCMAKE_BUILD_TYPE="Debug"
