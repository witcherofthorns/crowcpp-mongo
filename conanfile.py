from conan import ConanFile

class HelloConan(ConanFile):
    name = "crowcpp-mongo"
    version = "0.1"
    author = "Miroslaw Badurow"
    settings = "os", "arch", "compiler", "build_type"
    generators = "CMakeDeps", "CMakeToolchain"

    def requirements(self):
        self.requires("asio/1.29.0")
        self.requires("mongo-cxx-driver/3.8.1")
        self.requires("nlohmann_json/3.11.3")
