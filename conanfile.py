from conans import ConanFile, CMake

class ConanPackage(ConanFile):
    name = 'crawler'
    version = "0.1.0"

    generators = ['cmake_find_package' , 'cmake_paths']

    requires = [
        ('boost/1.74.0'),
        #('openssl/1.1.1h'),
        #('libcurl/7.73.0'),
        #('nlohmann_json/3.9.1'),

        ('sfml/2.5.1'),
        ('gtest/1.10.0'),
        ('spdlog/1.8.0'),
        ('cpr/1.9.0'),
        ('libxml2/2.9.14'),
        ('benchmark/1.6.2'),
        ('entt/3.5.2'),
        ('glew/2.2.0'),
        ('glfw/3.3.8')
    ]

    default_options = (
        'boost:shared=False',
        'glew:shared=False'
    )