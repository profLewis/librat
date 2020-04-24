"""setuptools for librat, ray tracing UCL/NCEO 

https://github.com/profLewis/librat
"""

# Always prefer setuptools over distutils

import os
import sys
from distutils.sysconfig import get_python_lib

from setuptools import setup

CURRENT_PYTHON = sys.version_info[:2]
REQUIRED_PYTHON = (3, 6)

# This check and everything above must remain compatible with Python 2.7.
if CURRENT_PYTHON < REQUIRED_PYTHON:
    sys.stderr.write("""
==========================
Unsupported Python version
==========================
This version of librat requires Python {}.{}, but you're trying to
install it on Python {}.{}.
This may be because you are using a version of pip that doesn't
understand the python_requires classifier. Make sure you
have pip >= 9.0 and setuptools >= 24.2, then try again:
    $ python -m pip install --upgrade pip setuptools
    $ python -m pip install django
This will install the latest version of librat which works on your
version of Python. If you can't upgrade your pip (or Python), request
an older version of librat:
    $ python -m pip install "django<2"
""".format(*(REQUIRED_PYTHON + CURRENT_PYTHON)))
    sys.exit(1)


from setuptools import find_packages, Extension
# To use a consistent encoding
from codecs import open
from os import path,uname

here = path.abspath(path.dirname(__file__))

PRATOBJ = ['rpv.c', 'cylinderTess.c', 'sphereTess.c', 'ratFront.c', 'disk.c', 'dummy.c', 'dem.c', 'bbox.c', 'facet.c', 'camera.c', 'intersect_objects.c', 'random.c', 'plane.c', 'reflectance.c', 'volumeRayTrace.c', 'sphere.c', 'cylinder.c', 'ellipse.c', 'spheroid.c', 'sky.c', 'sensor_rsr.c', 'files.c', 'mtllib.c', 'bilinear.c', 'dumper.c', 'useful.c', 'prat_wavefront_read.c', 'images.c', 'start_code.c']

ANCOBJS = ['error.c', 'filelib.c', 'hiplib.c', 'envilib.c', 'imagelib.c', 'libhipl.c', 'r250.c', 'randlcg.c', '4D_vectors.c', 'vectors2.c', '2D_vectors.c', 'allocate.c', 'matrix.c', 'fratP.c', 'matherr.c']

OBJS = PRATOBJ + ANCOBJS

ARCH = uname().machine

module1 = Extension('librat',
                    define_macros = [('VERBOSE','1'),
                                     ('_LARGEFILE_SOURCE','1'),
                                     ('_FILE_OFFSET_BITS','64'),
                                     ('TEST','1'),
                                     (ARCH,'1')
                                    ],
                    include_dirs = ['src'],
                    sources = ['src/'+i for i in OBJS])

# Get the long description from the relevant file
with open(path.join(here, 'README.md'), encoding='utf-8') as f:
    long_description = f.read()

setup(
    name='librat',

    # Versions should comply with PEP440.  For a discussion on single-sourcing
    # the version across setup.py and the project code, see
    # https://packaging.python.org/en/latest/single_source_version.html
    version='1.0.1',

    description='Librat',
    long_description=long_description,

    ext_modules = [module1],
    # The project's main homepage.
    url='https://github.com/profLewis/librat',

    # Author details
    author='Prof. P. Lewis',
    author_email='p.lewis@ucl.ac.uk',

    # Choose your license
    license='MIT',

    # See https://pypi.python.org/pypi?%3Aaction=list_classifiers
    classifiers=[
        # How mature is this project? Common values are
        #   3 - Alpha
        #   4 - Beta
        #   5 - Production/Stable
        'Development Status :: 3 - Alpha',

        # Indicate who your project is intended for
        'Intended Audience :: Developers',
        'Topic :: Software Development :: Build Tools',

        # Pick your license as you wish (should match "license" above)
        'License :: OSI Approved :: MIT License',

        # Specify the Python versions you support here. In particular, ensure
        # that you indicate whether you support Python 2, Python 3 or both.
        'Programming Language :: Python :: 3.7',
    ],

    # What does your project relate to?
    keywords='ray tracing',

    # You can just specify the packages manually here if your project is
    # simple. Or you can use find_packages().
    packages=find_packages(exclude=['contrib', 'docs', 'tests*']),

    # List run-time dependencies here.  These will be installed by pip when
    # your project is installed. For an analysis of "install_requires" vs pip's
    # requirements files see:
    # https://packaging.python.org/en/latest/requirements.html
    #install_requires=['numpy', 'matplotlib', 'pandas', 'jupyter','gdal'],

    # If there are data files included in your packages that need to be
    # installed, specify them here.  If using Python 2.6 or less, then these
    # have to be included in MANIFEST.in as well.
    #package_data={
    #    'sample': ['package_data.dat'],
    #},

    # Although 'package_data' is the preferred approach, in some case you may
    # need to place data files outside of your packages. See:
    # http://docs.python.org/3.4/distutils/setupscript.html#installing-additional-files # noqa
    # In this case, 'data_file' will be installed into '<sys.prefix>/my_data'
    #data_files=[('my_data', ['obj/*'])],

    # To provide executable scripts, use entry points in preference to the
    # "scripts" keyword. Entry points provide cross-platform support and allow
    # pip to create the appropriate form of executable for the target platform.
    entry_points={
        'console_scripts': [
            'sample=sample:main',
        ],
    },
)
