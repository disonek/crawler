from conan import ConanFile
from conan.tools.cmake import cmake_layout


class CompressorRecipe(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeToolchain", "CMakeDeps"

    def requirements(self):
        self.requires("boost/1.74.0") 
        self.requires('sfml/2.5.1'),
        self.requires('gtest/1.13.0'),
        self.requires('spdlog/1.13.0'),
        self.requires('cpr/1.9.0'),
        self.requires('libxml2/2.9.14'),
        self.requires('benchmark/1.6.2'),
        self.requires('entt/3.13.0'),
        self.requires('glew/2.2.0'),
        self.requires('glfw/3.3.8')


    def build_requirements(self):
        self.tool_requires("cmake/3.22.6")

    def layout(self):
        cmake_layout(self)