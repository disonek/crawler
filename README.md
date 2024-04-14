TO DO: <br />
Error Handling<br />
refactor, check what is to refactor (ImGuiLogger)<br />
type link from UI - better solution than is currently<br />

{
    "cmake.configureArgs": [
        "-DCMAKE_PROJECT_TOP_LEVEL_INCLUDES=conan_provider.cmake"
    ],
}

cmake . -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake
git submodule update --init --recursive