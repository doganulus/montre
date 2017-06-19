import os
import sys

from os.path import join

from distutils.core import setup, Extension
from distutils.command.build_ext import build_ext

class build_ext(build_ext):

    def build_extension(self, ext):
        self._ctypes = isinstance(ext, CTypes)
        return super().build_extension(ext)

    def get_export_symbols(self, ext):
        if self._ctypes:
            return ext.export_symbols
        return super().get_export_symbols(ext)

    def get_ext_filename(self, ext_name):
        if self._ctypes:
            return ext_name + '.so'
        return super().get_ext_filename(ext_name)

class CTypes(Extension): pass

module1 = CTypes('libmontre',
            define_macros = [ ('MAJOR_VERSION', '0'),
                              ('MINOR_VERSION', '6')],
            sources = [ join("src", "libmontre", "zone2.cpp"),
                        join("src", "libmontre", "zoneset.cpp"),
                        join("src", "libmontre", "boolean.cpp"),
                        join("src", "libmontre", "interval.cpp")],
            depends = [ join("src", "libmontre", "zone2.h"),
                        join("src", "libmontre", "zoneset.h"),
                        join("src", "libmontre", "boolean.h"),
                        join("src", "libmontre", "interval.h")],
            include_dirs=["libmontre"],
            extra_compile_args=['-std=c++11'])

setup (name = 'montre',
       version='0.1dev',
       author='Dogan Ulus',
       author_email='doganulus@gmail.com',
       packages=["montre"],
       license='GPLv3+',
       classifiers=[
        # How mature is this project? Common values are
        #   3 - Alpha
        #   4 - Beta
        #   5 - Production/Stable
        'Development Status :: 3 - Alpha',

        # Indicate who your project is intended for
        'Intended Audience :: Science/Research',
        'Topic :: Scientific/Engineering :: Mathematics',

        # Pick your license as you wish (should match "license" above)
        'License :: OSI Approved :: GNU General Public License v3 or later (GPLv3+)',

        # Specify the Python versions you support here. In particular, ensure
        # that you indicate whether you support Python 2, Python 3 or both.
        'Programming Language :: Python :: 3',
    ],
       description = 'This is a Python binding for timed pattern matching',
       url='http://pypi.python.org/pypi/montre/',
       py_modules = ["montre.algebra"],
       ext_modules = [module1],
       cmdclass={'build_ext': build_ext})

