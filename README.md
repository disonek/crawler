TO DO: <br />
Error Handling<br />
refactor, check what is to refactor (ImGuiLogger)<br />
type link from UI - better solution than is currently<br />

conan profile:<br />
[settings]<br />
os=Windows<br />
os_build=Windows<br />
arch=x86_64<br />
arch_build=x86_64<br />
build_type=Debug<br />
compiler=Visual Studio<br />
compiler.version=16<br />
compiler.runtime=MDd<br />
[options]<br />
[conf]<br />
[build_requires]<br />
[env]<br />

[proc] Executing command: "C:\Program Files\CMake\bin\cmake.EXE" --no-warn-unused-cli -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -Sc:/Users/dison/dev/crawler -Bc:/Users/dison/dev/crawler/build -G "Visual Studio 16 2019" -T host=x64 -A x64