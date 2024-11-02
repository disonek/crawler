# Simple web crawler
Project supports currently Windows Os. Hovewer support for a Linux/MacOs can be relatively easly provided.

## Tools needed to build 
**msvc** compiler <br />
**conan 2.0** package manager <br />
**cmake** build system generator<br />

### More info
 [conan](https://docs.conan.io/2/index.html)

## Build steps: 

1) git submodule update --init --recursive <br />
2) mkdir build<br />
3) cmake -B build -S . -DCMAKE_PROJECT_TOP_LEVEL_INCLUDES="conan_provider.cmake" -DCMAKE_BUILD_TYPE="Debug" <br />
3) cmake --build build --config Debug <br />

### More info
 [cmake-conan](https://github.com/conan-io/cmake-conan) <br />

###TO DO: <br />
Error Handling<br />
refactor, check what is to refactor (ImGuiLogger)<br />
type link from UI - better solution than is currently<br />
