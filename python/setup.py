from glob import glob
from setuptools import setup
from pybind11.setup_helpers import Pybind11Extension
from pybind11.setup_helpers import ParallelCompile, naive_recompile
import warnings
import os

__version__ = "4.1.0"
__libraries__ = ['state_representation', 'clproto']
__include_dirs__ = ['include']

__install_clproto_module__ = True

# check that eigen and state_representation libraries can be found
try:
    eigen_dir = os.popen(
        'cmake --find-package -DNAME=Eigen3 -DCOMPILER_ID=GNU -DLANGUAGE=C -DMODE=COMPILE').read().strip()
    if eigen_dir.startswith('-I'):
        __include_dirs__.append(eigen_dir.lstrip('-I'))
    else:
        raise Exception('Could not find Eigen3 package!')

    for lib in __libraries__:
        status = os.popen(f'ldconfig -p | grep {lib}').read().strip()
        if len(status) == 0:
            msg = f'Could not find library {lib}!'
            if lib == 'clproto':
                warnings.warn(f'{msg} The clproto module will not be installed.')
                __install_clproto_module__ = False
            else:
                raise Exception(msg)
except Exception as e:
    msg = f'Error with control library dependencies: {e.args[0]}. Ensure the control libraries are properly installed.'
    warnings.warn(msg)

ParallelCompile("NPY_NUM_BUILD_JOBS", needs_recompile=naive_recompile).install()

ext_modules = [
    Pybind11Extension("state_representation",
                      sorted(glob("source/state_representation/*.cpp") + glob("source/common/*.cpp")),
                      cxx_std=17,
                      include_dirs=__include_dirs__,
                      libraries=['state_representation'],
                      define_macros=[('MODULE_VERSION_INFO', __version__)],
                      )
]

if __install_clproto_module__:
    ext_modules.append(
        Pybind11Extension("clproto",
                          sorted(glob("source/clproto/*.cpp") + glob("source/common/*.cpp")),
                          cxx_std=17,
                          include_dirs=__include_dirs__,
                          libraries=['state_representation', 'clproto'],
                          define_macros=[('MODULE_VERSION_INFO', __version__)],
                          )
    )

setup(
    name="control-libraries",
    version=__version__,
    author="Enrico Eberhard",
    author_email="enrico.eberhard@epfl.ch",
    url="https://github.com/epfl-lasa/control-libraries",
    description="Python bindings for the C++ control libraries",
    long_description="",
    ext_modules=ext_modules,
    test_suite="tests",
    python_requires='>=3',
    license='GNU GPL v3',
    zip_safe=False,
)
