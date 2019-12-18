import os
import fnmatch
from conans import ConanFile, AutoToolsBuildEnvironment

class CattalibConan(ConanFile):
    name = "catta"
    license = "<Put the package license here>"
    author = "<Put your name here> <And your email here>"
    url = "<Package recipe repository url here, for issues about the package>"
    description = "This recipe file used to build and package binaries of catta repository"
    topics = ("<Put some tag here>", "<here>", "<and here>")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = "shared=False"
    generators = "make"

    @property
    def _targets(self):
        return {
            "iOS-x86-*": "i386-apple-ios",
            "iOS-x86_64-*": "x86_64-apple-ios"
        }

    def build(self):
        autotools = AutoToolsBuildEnvironment(self)
        # self.run("cd .. && autoreconf -fsi ")
        query = "%s-%s-%s" % (self.settings.os, self.settings.arch, self.settings.compiler)
        ancestor = next((self._targets[i] for i in self._targets if fnmatch.fnmatch(query, i)), None)
        if not ancestor:
            autotools.configure(configure_dir= "..",args= args, use_default_install_dirs=True)
        else:
            autotools.configure(configure_dir= "..",args= args, use_default_install_dirs=True, host= ancestor)
        autotools.make()
        autotools.install()

    def package(self):
        self.copy("*.h", dst="include", src="include/include")
        self.copy("*", dst="lib", src="src/lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = [ "catta" ]
