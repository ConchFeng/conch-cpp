from conan import ConanFile
from conan.tools.cmake import CMake, cmake_layout

class MyCrossPlatformProject(ConanFile):
    name = "ConchCrossProject"
    version = "1.0.0"
    settings = "os", "compiler", "build_type", "arch"
    
    # Define generators: CMakeDeps and CMakeToolchain
    generators = "CMakeDeps", "CMakeToolchain"

    # Define all required third-party libraries
    def requirements(self):
        self.requires("spdlog/1.16.0")
        self.requires("fmt/11.0.2")
        self.requires("libuv/1.51.0")
        self.requires("gtest/1.17.0") 
        self.requires("benchmark/1.9.4")
        
        # Qt6 is only needed for desktop apps, not mobile (using native iOS/Android)
        if self.settings.os not in ["Android", "iOS"]:
            self.requires("qt/6.10.1")
        
        # Optional: self.requires("nlohmann_json/3.11.3")

    def configure(self):
        # Qt6 optimization configuration (only for desktop)
        if self.settings.os not in ["Android", "iOS"]:
            self.options["qt"].shared = True  # Desktop typically uses dynamic linking

    def layout(self):
        # Use standard CMake layout
        cmake_layout(self)

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
