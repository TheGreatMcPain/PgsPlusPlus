from conans import ConanFile, CMake, tools


class PgsPlusPlusConan(ConanFile):
    name = "pgs++"
    version = "0.1.0"
    license = "LGPLv2"
    author = "Brenden Davidson <davidson.brenden15@gmail.com>"
    url = "https://github.com/iAmSomeone2/libpgs"
    description = "A C++ library for reading Presentation Graphics Stream (PGS) subtitles."
    topics = ("C++", "PGS", "Subtitles", "Blu-ray")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "build_testing": [True, False]}
    default_options = {"shared": False, "build_testing": False}
    generators = "cmake", "cmake_find_package"
    exports_sources = "src/*", "include/*", "CMakeLists.txt"

    def build(self):
        cmake = CMake(self)

        if self.options.build_testing:
            cmake.definitions["BUILD_TESTING"] = "on"
        else:
            cmake.definitions["BUILD_TESTING"] = "off"

        if self.options.shared:
            cmake.definitions["BUILD_SHARED_LIBS"] = "on"
        else:
            cmake.definitions["BUILD_SHARED_LIBS"] = "off"

        cmake.configure()
        cmake.build()

    def package(self):
        self.copy("*.hpp", dst="include/pgs++", src="include")
        self.copy("*.hpp", dst="include/pgs++", src="src")
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.dylib", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["pgs++"]

