from conans import ConanFile, CMake

class RPLidarBridgeConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    requires = (
        "json/1.0.0@paulobrizolara/stable",
        "RPLidarSDK/1.5.7@gdepuille/stable"
    )
    generators = "cmake"

    def build(self):
        cmake = CMake(self.settings)
        self.run('cmake "%s" %s' % (self.conanfile_directory, cmake.command_line))
        self.run('cmake --build . %s' % cmake.build_config)
