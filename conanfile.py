import os
from conan import ConanFile
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain, cmake_layout
from conan.tools.env import VirtualRunEnv
from conan.tools.files import copy, load

class unQliteppConan(ConanFile):
    name = "unqlitepp"
    license = "MIT"
    description = "a modern c++ wrapper over the unqlite library"
    url = "https://github.com/julienlopez/unqlitepp"
    topics = ("nosql", "database", "transactional", "storage")
    settings = ["os", "compiler", "build_type", "arch"]
    options = {
        "shared": [True, False],
        "build_testing": [True, False],
        "coverage_enable": [True, False],
        "enable_msvc_code_analysis": [True, False],
    }
    default_options = {
        "shared": True,
        "build_testing": False,
        "coverage_enable": False,
        "enable_msvc_code_analysis": False,
    }

    def layout(self):
        cmake_layout(self, src_folder=".", build_folder=".")

    def requirements(self):
        self.requires("unqlite/1.1.9")
        self.requires("tl-expected/1.1.0")
        # self.requires("magic_enum/0.8.1")
        if self.options.build_testing:
            self.requires("catch2/3.3.2")

    def build_requirements(self):
        self.tool_requires("cmake/3.25.3")
        if self.settings.os == "Windows":
            self.tool_requires("ninja/1.11.1")
        else:
            self.tool_requires("make/4.3")
        # if self.options.build_testing:
        #     self.requires("catch2/3.3.2")

    def imports(self):
        # allow to run the binaries from Visual Studio without calling activate_run.bat
        # and can be useful to create an installer
        for dep in self.dependencies.values():
            for bindir in dep.cpp_info.bindirs:
                copy(self, "*.dll", bindir, os.path.join(self.build_folder, "bin"))

    def export_sources(self):
        copy(
            self,
            "CMakeLists.txt",
            src=self.recipe_folder,
            dst=self.export_sources_folder,
        )
        copy(self, "lib/*", src=self.recipe_folder, dst=self.export_sources_folder)
        copy(self, "VERSION", src=self.recipe_folder, dst=self.export_sources_folder)

    def set_version(self):
        self.version = load(self, os.path.join(self.recipe_folder, "VERSION")).strip()

    def generate(self):
        tc = CMakeToolchain(self)
        tc.variables["BUILD_TESTING"] = self.options.build_testing
        tc.variables["ENABLE_COVERAGE"] = self.options.coverage_enable
        tc.variables["ENABLE_MSVC_CODE_ANALYSIS"] = self.options.enable_msvc_code_analysis
        tc.generate()

        deps = CMakeDeps(self)
        # catch2 is used for both building (catch_discover_tests cmake macro) and running (unit test is linked to catch2main lib)
        deps.build_context_activated = ["catch2", "cmakecommonfiles"]
        deps.build_context_suffix = {"catch2": "_BUILD"}
        deps.build_context_build_modules = ["cmakecommonfiles"]
        deps.generate()

        runenv = VirtualRunEnv(self)
        runenv.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def _copy_filetype_in_dir(self, file_type : str, output_directory_name : str, source_directory = None, keep_path=False):
        if source_directory is None:
            source_directory = self.build_folder
        copy(
            self,
            file_type,
            src=source_directory,
            dst=os.path.join(self.package_folder, output_directory_name),
            keep_path=keep_path,
        )

    def package(self):
        self._copy_filetype_in_dir("*.so", "lib")
        self._copy_filetype_in_dir("*.a", "lib")
        self._copy_filetype_in_dir("*.dll", "bin")
        self._copy_filetype_in_dir("*.lib", "lib")
        self._copy_filetype_in_dir("*.hpp", "include", os.path.join(self.source_folder, "lib"), keep_path=True)

    def package_info(self):
        if not self.in_local_cache:
            self.cpp_info.includedirs = ["lib/unqlitepp"]

        self.cpp_info.requires = [
            "unqlite::unqlite",
            "tl-expected::tl-expected",
            #"magic_enum::magic_enum",
        ]

        # add a suffix on windows to differentiate release and debug libraries (not compatible)
        debug_suffix = (
            "d"
            if self.settings.build_type == "Debug"
            and self.settings.compiler == "Visual Studio"
            else ""
        )

        self.cpp_info.libs = ["unqlitepp%s" % debug_suffix]
