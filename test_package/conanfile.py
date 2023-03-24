import os

from conan import ConanFile
from conan.tools.build import can_run
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain, cmake_layout
from conan.tools.files import copy

class LibUtilsTestConan(ConanFile):
    test_type = "explicit"
    settings = ["os", "compiler", "build_type", "arch"]
    generators = ["CMakeDeps", "CMakeToolchain", "VirtualRunEnv"]

    def layout(self):
        cmake_layout(self, src_folder=".", build_folder=".")

    def requirements(self):
        self.requires(self.tested_reference_str)

    def build_requirements(self):
        self.tool_requires("cmake/3.25.3")
        if self.settings.os == "Windows":
            self.tool_requires("ninja/1.11.1")
        else:
            self.tool_requires("make/4.3")

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def test(self):
        if can_run(self):
            bin_path = os.path.join(self.cpp.build.bindirs[0], "unqlitepp_test")
            self.run(bin_path, env="conanrun", run_environment=True)
