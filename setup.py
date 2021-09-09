# -*- coding: utf-8 -*-
"""
Setup script used to build and install this package.

Adapted from the pybind cmake_example template.
https://github.com/pybind/cmake_example/blob/master/setup.py
"""
import os
import re
import subprocess
import sys

from setuptools import find_packages, setup, Extension
from setuptools.command.build_ext import build_ext

# Convert distutils Windows platform specifiers to CMake -A arguments
PLAT_TO_CMAKE = {
    "win32": "Win32",
    "win-amd64": "x64",
    "win-arm32": "ARM",
    "win-arm64": "ARM64",
}


class CMakeExtension(Extension):
    """
    An extended Extension-Class for CMake. We need this as we have to
    pass a source dir instead of a file_list to get the source-code to build

    The name must be _single_ output extension from the CMake build. If you need
    multiple extensions, see scikit-build
    """

    def __init__(self, name, sourcedir=""):
        super().__init__(name, sources=[])
        self.sourcedir = os.path.abspath(sourcedir)


class CMakeBuild(build_ext):
    """
    An extended build_ext-Class, used to build using our CMake workflow
    """

    user_options = [("debug=", None, "Whether to build in debug-mode or not")]
    boolean_options = ["debug"]

    def build_extension(self, ext):
        extension_full_path = self.get_ext_fullpath(ext.name)
        extension_dir_name = os.path.dirname(extension_full_path)
        extension_dir_path = os.path.abspath(extension_dir_name)

        # Required for auto-detection and inclusion of auxiliary "native" libs
        if not extension_dir_path.endswith(os.path.sep):
            extension_dir_path += os.path.sep

        # Get the BUILD_TYPE configuration (might even be in os.environ)
        debug = (
            int(os.environ.get("DEBUG", 0))
            if self.debug is None
            else self.debug
        )
        cfg = "Debug" if debug else "Release"

        # CMake lets you override the generator (usually on os.environ), so we
        # need to check this. Can be also set with Conda-build, for example
        cmake_generator = os.environ.get("CMAKE_GENERATOR", "")

        cmake_args = [
            # Make sure we handle RPATH/RUNPATH properly
            "-DCMAKE_INSTALL_RPATH=$ORIGIN",
            "-DCMAKE_BUILD_WITH_INSTALL_RPATH:BOOL=ON",
            "-DCMAKE_INSTALL_RPATH_USE_LINK_PATH:BOOL=OFF",
            # Set Python_EXECUTABLE instead if you use PYBIND11_FINDPYTHON
            "-DPYTHON_EXECUTABLE={}".format(sys.executable),
            # Make sure we're building python bindings
            "-DTINYUTILS_BUILD_PYTHON_BINDINGS=ON",
            # Make sure we're not building examples nor tests
            "-DTINYUTILS_BUILD_EXAMPLES=OFF",
            # Make sure we're not building any documentation
            "-DTINYUTILS_BUILD_DOCS=OFF",
        ]
        build_args = []

        # By default, place every generated artifact into the same install path
        library_outdir = extension_dir_path
        archive_outdir = extension_dir_path
        runtime_outdir = extension_dir_path

        # Adding CMake arguments set as environment variable (needed e.g. to
        # build for ARM OSx on conda-forge). Notice they are space separated
        if "CMAKE_ARGS" in os.environ:
            cmake_args += [
                item for item in os.environ["CMAKE_ARGS"].split(" ") if item
            ]

        if self.compiler.compiler_type != "msvc":
            # Using Ninja-build since it a) is available as a wheel and b)
            # multithreads automatically. MSVC would require all variables be
            # exported for Ninja to pick it up, which is a little tricky to do
            if not cmake_generator:
                try:
                    # pylint: disable=import-outside-toplevel, unused-import
                    import ninja

                    cmake_args += ["-GNinja"]
                except ImportError:
                    pass

            # Send all generated artifacts to the same install location
            cmake_args += [
                "-DCMAKE_LIBRARY_OUTPUT_DIRECTORY={}".format(library_outdir),
                "-DCMAKE_ARCHIVE_OUTPUT_DIRECTORY={}".format(archive_outdir),
                "-DCMAKE_RUNTIME_OUTPUT_DIRECTOROY={}".format(runtime_outdir),
            ]
        else:
            # Single config generators are handled "normally"
            single_config = any(
                x in cmake_generator for x in ["NMake", "Ninja"]
            )

            # CMake allows an arch-in-generator style for backward compatibility
            contains_arch = any(x in cmake_generator for x in ["ARM", "Win64"])

            # Specify the arch if using MSVC generator, but only if it doesn't
            # contain a backward-compat. arch spec already in the generator name
            if not single_config and not contains_arch:
                cmake_args += ["-A", PLAT_TO_CMAKE[self.plat_name]]

            # Multi-config generators have a different way to specify configs
            if not single_config:
                cmake_args += [
                    "-DCMAKE_LIBRARY_OUTPUT_DIRECTORY_{}={}".format(
                        cfg.upper(), library_outdir
                    ),
                    "-DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_{}={}".format(
                        cfg.upper(), archive_outdir
                    ),
                    "-DCMAKE_RUNTIME_OUTPUT_DIRECTORY_{}={}".format(
                        cfg.upper(), runtime_outdir
                    ),
                ]
                build_args += ["--config", cfg]

        if sys.platform.startswith("darwin"):
            # Cross-compile support for macOS - respect ARCHFLAGS if set
            archs = re.findall(r"-arch (\S+)", os.environ.get("ARCHFLAGS", ""))
            if archs:
                cmake_args += [
                    "-DCMAKE_OSX_ARCHITECTURE={}".format(";".join(archs))
                ]

        # Set CMAKE_BUILD_PARALLEL_LEVEL to control the parallel build level
        # across all generators (if not set as an environment variable)
        if "CMAKE_BUILD_PARALLEL_LEVEL" not in os.environ:
            # self.parallel is a Python-3 only way to set parallel jobs by
            # hand using -j in the build_ext call, not supported by pip or
            # PyPA-build
            if hasattr(self, "parallel") and self.parallel:
                # CMake 3.12+ only
                build_args += ["-j{}".format(self.parallel)]

        if not os.path.exists(self.build_temp):
            os.makedirs(self.build_temp)

        subprocess.check_call(
            ["cmake", ext.sourcedir] + cmake_args, cwd=self.build_temp
        )
        subprocess.check_call(
            ["cmake", "--build", "."] + build_args, cwd=self.build_temp
        )


# @todo(wilbert): remove in case $ORIGIN and the new way of handling RPATH works
# def get_installation_dir():
#     """
#     Gets the installation directory in which to install the built targets.
#     Adapted from https://tinyurl.com/yytrft4u
#
#     @todo(wilbert): Check that this works for Windows as well
#     @todo(wilbert): Check if there's a general way to get the installation dir
#     """
#     py_version = "%s.%s" % (sys.version_info[0], sys.version_info[1])
#     install_path_candidates = (
#         path % (py_version)
#         for path in (
#             os.path.join(sys.prefix, "/lib/python%s/dist-packages/"),
#             os.path.join(sys.prefix, "/lib/python%s/site-packages/"),
#             os.path.join(sys.prefix, "/local/lib/python%s/dist-packages/"),
#             os.path.join(sys.prefix, "/local/lib/python%s/site-packages/"),
#             "/Library/Python/%s/site-packages/",
#         )
#     )
#
#     for path_candidate in install_path_candidates:
#         if os.path.exists(path_candidate):
#             return path_candidate
#
#     print("ERROR >>> No installation path found", file=sys.stderr)
#     return None

long_description = ""
if os.path.exists("README.md"):
    with open("README.md", "r", encoding="utf-8") as fh:
        long_description = fh.read()

required_packages = []
if os.path.exists("requirements.txt"):
    with open("requirements.txt", "r", encoding="utf-8") as fh:
        required_packages = [line.replace("\n", "") for line in fh.readlines()]

setup(
    name="wp-tinyutils",
    version="0.0.1",
    description="A small C/C++ helper library for some common utils",
    long_description=long_description,
    long_description_content_type="text/markdown",
    author="Wilbert Santos Pumacay Huallpa",
    license="MIT License",
    author_email="wpumacay@gmail.com",
    url="https://github.com/wpumacay/tiny_utils",
    keywords="C C++ Utils",
    classifiers=[
        "License :: OSI Approved :: MIT License",
        "Operating System :: POSIX :: Linux",
    ],
    packages=find_packages(),
    zip_safe=False,
    install_requires=required_packages,
    package_data={},
    ext_modules=[CMakeExtension("tinyutils", ".")],
    cmdclass={"build_ext": CMakeBuild},
)
