r"""Wrapper for hipl_format.h

Generated with:
/Users/plewis/anaconda3/bin/ctypesgen -L. -L/Users/plewis/Documents/GitHub/librat/src -I. -I/Users/plewis/Documents/GitHub/librat/src -llibratlib.so hipl_format.h define_float.h useful3.h vectors2.h 4D_vectors.h 2D_vectors.h wavefront.h hips.h allocate.h intersect_objects.h materials.h prat_objects.h sensor_rsr.h camera.h globals.h reflectance.h volume.h rpv.h mtllib.h element_types.h reflectance_functions.h bags.h vector_macros.h 3D_vectors_double.h prat.h materials.h frat.h rat.h ratFront.h imagelib.h image_formats.h -o librat.py

Do not modify this file.
"""

__docformat__ = "restructuredtext"

# Begin preamble for Python v(3, 2)

import ctypes, os, sys
from ctypes import *

_int_types = (c_int16, c_int32)
if hasattr(ctypes, "c_int64"):
    # Some builds of ctypes apparently do not have c_int64
    # defined; it's a pretty good bet that these builds do not
    # have 64-bit pointers.
    _int_types += (c_int64,)
for t in _int_types:
    if sizeof(t) == sizeof(c_size_t):
        c_ptrdiff_t = t
del t
del _int_types


class UserString:
    def __init__(self, seq):
        if isinstance(seq, bytes):
            self.data = seq
        elif isinstance(seq, UserString):
            self.data = seq.data[:]
        else:
            self.data = str(seq).encode()

    def __bytes__(self):
        return self.data

    def __str__(self):
        return self.data.decode()

    def __repr__(self):
        return repr(self.data)

    def __int__(self):
        return int(self.data.decode())

    def __long__(self):
        return int(self.data.decode())

    def __float__(self):
        return float(self.data.decode())

    def __complex__(self):
        return complex(self.data.decode())

    def __hash__(self):
        return hash(self.data)

    def __cmp__(self, string):
        if isinstance(string, UserString):
            return cmp(self.data, string.data)
        else:
            return cmp(self.data, string)

    def __le__(self, string):
        if isinstance(string, UserString):
            return self.data <= string.data
        else:
            return self.data <= string

    def __lt__(self, string):
        if isinstance(string, UserString):
            return self.data < string.data
        else:
            return self.data < string

    def __ge__(self, string):
        if isinstance(string, UserString):
            return self.data >= string.data
        else:
            return self.data >= string

    def __gt__(self, string):
        if isinstance(string, UserString):
            return self.data > string.data
        else:
            return self.data > string

    def __eq__(self, string):
        if isinstance(string, UserString):
            return self.data == string.data
        else:
            return self.data == string

    def __ne__(self, string):
        if isinstance(string, UserString):
            return self.data != string.data
        else:
            return self.data != string

    def __contains__(self, char):
        return char in self.data

    def __len__(self):
        return len(self.data)

    def __getitem__(self, index):
        return self.__class__(self.data[index])

    def __getslice__(self, start, end):
        start = max(start, 0)
        end = max(end, 0)
        return self.__class__(self.data[start:end])

    def __add__(self, other):
        if isinstance(other, UserString):
            return self.__class__(self.data + other.data)
        elif isinstance(other, bytes):
            return self.__class__(self.data + other)
        else:
            return self.__class__(self.data + str(other).encode())

    def __radd__(self, other):
        if isinstance(other, bytes):
            return self.__class__(other + self.data)
        else:
            return self.__class__(str(other).encode() + self.data)

    def __mul__(self, n):
        return self.__class__(self.data * n)

    __rmul__ = __mul__

    def __mod__(self, args):
        return self.__class__(self.data % args)

    # the following methods are defined in alphabetical order:
    def capitalize(self):
        return self.__class__(self.data.capitalize())

    def center(self, width, *args):
        return self.__class__(self.data.center(width, *args))

    def count(self, sub, start=0, end=sys.maxsize):
        return self.data.count(sub, start, end)

    def decode(self, encoding=None, errors=None):  # XXX improve this?
        if encoding:
            if errors:
                return self.__class__(self.data.decode(encoding, errors))
            else:
                return self.__class__(self.data.decode(encoding))
        else:
            return self.__class__(self.data.decode())

    def encode(self, encoding=None, errors=None):  # XXX improve this?
        if encoding:
            if errors:
                return self.__class__(self.data.encode(encoding, errors))
            else:
                return self.__class__(self.data.encode(encoding))
        else:
            return self.__class__(self.data.encode())

    def endswith(self, suffix, start=0, end=sys.maxsize):
        return self.data.endswith(suffix, start, end)

    def expandtabs(self, tabsize=8):
        return self.__class__(self.data.expandtabs(tabsize))

    def find(self, sub, start=0, end=sys.maxsize):
        return self.data.find(sub, start, end)

    def index(self, sub, start=0, end=sys.maxsize):
        return self.data.index(sub, start, end)

    def isalpha(self):
        return self.data.isalpha()

    def isalnum(self):
        return self.data.isalnum()

    def isdecimal(self):
        return self.data.isdecimal()

    def isdigit(self):
        return self.data.isdigit()

    def islower(self):
        return self.data.islower()

    def isnumeric(self):
        return self.data.isnumeric()

    def isspace(self):
        return self.data.isspace()

    def istitle(self):
        return self.data.istitle()

    def isupper(self):
        return self.data.isupper()

    def join(self, seq):
        return self.data.join(seq)

    def ljust(self, width, *args):
        return self.__class__(self.data.ljust(width, *args))

    def lower(self):
        return self.__class__(self.data.lower())

    def lstrip(self, chars=None):
        return self.__class__(self.data.lstrip(chars))

    def partition(self, sep):
        return self.data.partition(sep)

    def replace(self, old, new, maxsplit=-1):
        return self.__class__(self.data.replace(old, new, maxsplit))

    def rfind(self, sub, start=0, end=sys.maxsize):
        return self.data.rfind(sub, start, end)

    def rindex(self, sub, start=0, end=sys.maxsize):
        return self.data.rindex(sub, start, end)

    def rjust(self, width, *args):
        return self.__class__(self.data.rjust(width, *args))

    def rpartition(self, sep):
        return self.data.rpartition(sep)

    def rstrip(self, chars=None):
        return self.__class__(self.data.rstrip(chars))

    def split(self, sep=None, maxsplit=-1):
        return self.data.split(sep, maxsplit)

    def rsplit(self, sep=None, maxsplit=-1):
        return self.data.rsplit(sep, maxsplit)

    def splitlines(self, keepends=0):
        return self.data.splitlines(keepends)

    def startswith(self, prefix, start=0, end=sys.maxsize):
        return self.data.startswith(prefix, start, end)

    def strip(self, chars=None):
        return self.__class__(self.data.strip(chars))

    def swapcase(self):
        return self.__class__(self.data.swapcase())

    def title(self):
        return self.__class__(self.data.title())

    def translate(self, *args):
        return self.__class__(self.data.translate(*args))

    def upper(self):
        return self.__class__(self.data.upper())

    def zfill(self, width):
        return self.__class__(self.data.zfill(width))


class MutableString(UserString):
    """mutable string objects

    Python strings are immutable objects.  This has the advantage, that
    strings may be used as dictionary keys.  If this property isn't needed
    and you insist on changing string values in place instead, you may cheat
    and use MutableString.

    But the purpose of this class is an educational one: to prevent
    people from inventing their own mutable string class derived
    from UserString and than forget thereby to remove (override) the
    __hash__ method inherited from UserString.  This would lead to
    errors that would be very hard to track down.

    A faster and better solution is to rewrite your program using lists."""

    def __init__(self, string=""):
        self.data = string

    def __hash__(self):
        raise TypeError("unhashable type (it is mutable)")

    def __setitem__(self, index, sub):
        if index < 0:
            index += len(self.data)
        if index < 0 or index >= len(self.data):
            raise IndexError
        self.data = self.data[:index] + sub + self.data[index + 1 :]

    def __delitem__(self, index):
        if index < 0:
            index += len(self.data)
        if index < 0 or index >= len(self.data):
            raise IndexError
        self.data = self.data[:index] + self.data[index + 1 :]

    def __setslice__(self, start, end, sub):
        start = max(start, 0)
        end = max(end, 0)
        if isinstance(sub, UserString):
            self.data = self.data[:start] + sub.data + self.data[end:]
        elif isinstance(sub, bytes):
            self.data = self.data[:start] + sub + self.data[end:]
        else:
            self.data = self.data[:start] + str(sub).encode() + self.data[end:]

    def __delslice__(self, start, end):
        start = max(start, 0)
        end = max(end, 0)
        self.data = self.data[:start] + self.data[end:]

    def immutable(self):
        return UserString(self.data)

    def __iadd__(self, other):
        if isinstance(other, UserString):
            self.data += other.data
        elif isinstance(other, bytes):
            self.data += other
        else:
            self.data += str(other).encode()
        return self

    def __imul__(self, n):
        self.data *= n
        return self


class String(MutableString, Union):

    _fields_ = [("raw", POINTER(c_char)), ("data", c_char_p)]

    def __init__(self, obj=""):
        if isinstance(obj, (bytes, UserString)):
            self.data = bytes(obj)
        else:
            self.raw = obj

    def __len__(self):
        return self.data and len(self.data) or 0

    def from_param(cls, obj):
        # Convert None or 0
        if obj is None or obj == 0:
            return cls(POINTER(c_char)())

        # Convert from String
        elif isinstance(obj, String):
            return obj

        # Convert from bytes
        elif isinstance(obj, bytes):
            return cls(obj)

        # Convert from str
        elif isinstance(obj, str):
            return cls(obj.encode())

        # Convert from c_char_p
        elif isinstance(obj, c_char_p):
            return obj

        # Convert from POINTER(c_char)
        elif isinstance(obj, POINTER(c_char)):
            return obj

        # Convert from raw pointer
        elif isinstance(obj, int):
            return cls(cast(obj, POINTER(c_char)))

        # Convert from c_char array
        elif isinstance(obj, c_char * len(obj)):
            return obj

        # Convert from object
        else:
            return String.from_param(obj._as_parameter_)

    from_param = classmethod(from_param)


def ReturnString(obj, func=None, arguments=None):
    return String.from_param(obj)


# As of ctypes 1.0, ctypes does not support custom error-checking
# functions on callbacks, nor does it support custom datatypes on
# callbacks, so we must ensure that all callbacks return
# primitive datatypes.
#
# Non-primitive return values wrapped with UNCHECKED won't be
# typechecked, and will be converted to c_void_p.
def UNCHECKED(type):
    if hasattr(type, "_type_") and isinstance(type._type_, str) and type._type_ != "P":
        return type
    else:
        return c_void_p


# ctypes doesn't have direct support for variadic functions, so we have to write
# our own wrapper class
class _variadic_function(object):
    def __init__(self, func, restype, argtypes, errcheck):
        self.func = func
        self.func.restype = restype
        self.argtypes = argtypes
        if errcheck:
            self.func.errcheck = errcheck

    def _as_parameter_(self):
        # So we can pass this variadic function as a function pointer
        return self.func

    def __call__(self, *args):
        fixed_args = []
        i = 0
        for argtype in self.argtypes:
            # Typecheck what we can
            fixed_args.append(argtype.from_param(args[i]))
            i += 1
        return self.func(*fixed_args + list(args[i:]))


def ord_if_char(value):
    """
    Simple helper used for casts to simple builtin types:  if the argument is a
    string type, it will be converted to it's ordinal value.

    This function will raise an exception if the argument is string with more
    than one characters.
    """
    return ord(value) if (isinstance(value, bytes) or isinstance(value, str)) else value

# End preamble

_libs = {}
_libdirs = ['.', '/Users/plewis/Documents/GitHub/librat/src']

# Begin loader

# ----------------------------------------------------------------------------
# Copyright (c) 2008 David James
# Copyright (c) 2006-2008 Alex Holkner
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
#  * Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#  * Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in
#    the documentation and/or other materials provided with the
#    distribution.
#  * Neither the name of pyglet nor the names of its
#    contributors may be used to endorse or promote products
#    derived from this software without specific prior written
#    permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
# ----------------------------------------------------------------------------

import os.path, re, sys, glob
import platform
import ctypes
import ctypes.util


def _environ_path(name):
    if name in os.environ:
        return os.environ[name].split(":")
    else:
        return []


class LibraryLoader(object):
    # library names formatted specifically for platforms
    name_formats = ["%s"]

    class Lookup(object):
        mode = ctypes.DEFAULT_MODE

        def __init__(self, path):
            super(LibraryLoader.Lookup, self).__init__()
            self.access = dict(cdecl=ctypes.CDLL(path, self.mode))

        def get(self, name, calling_convention="cdecl"):
            if calling_convention not in self.access:
                raise LookupError(
                    "Unknown calling convention '{}' for function '{}'".format(
                        calling_convention, name
                    )
                )
            return getattr(self.access[calling_convention], name)

        def has(self, name, calling_convention="cdecl"):
            if calling_convention not in self.access:
                return False
            return hasattr(self.access[calling_convention], name)

        def __getattr__(self, name):
            return getattr(self.access["cdecl"], name)

    def __init__(self):
        self.other_dirs = []

    def __call__(self, libname):
        """Given the name of a library, load it."""
        paths = self.getpaths(libname)

        for path in paths:
            try:
                return self.Lookup(path)
            except:
                pass

        raise ImportError("Could not load %s." % libname)

    def getpaths(self, libname):
        """Return a list of paths where the library might be found."""
        if os.path.isabs(libname):
            yield libname
        else:
            # search through a prioritized series of locations for the library

            # we first search any specific directories identified by user
            for dir_i in self.other_dirs:
                for fmt in self.name_formats:
                    # dir_i should be absolute already
                    yield os.path.join(dir_i, fmt % libname)

            # then we search the directory where the generated python interface is stored
            for fmt in self.name_formats:
                yield os.path.abspath(os.path.join(os.path.dirname(__file__), fmt % libname))

            # now, use the ctypes tools to try to find the library
            for fmt in self.name_formats:
                path = ctypes.util.find_library(fmt % libname)
                if path:
                    yield path

            # then we search all paths identified as platform-specific lib paths
            for path in self.getplatformpaths(libname):
                yield path

            # Finally, we'll try the users current working directory
            for fmt in self.name_formats:
                yield os.path.abspath(os.path.join(os.path.curdir, fmt % libname))

    def getplatformpaths(self, libname):
        return []


# Darwin (Mac OS X)


class DarwinLibraryLoader(LibraryLoader):
    name_formats = [
        "lib%s.dylib",
        "lib%s.so",
        "lib%s.bundle",
        "%s.dylib",
        "%s.so",
        "%s.bundle",
        "%s",
    ]

    class Lookup(LibraryLoader.Lookup):
        # Darwin requires dlopen to be called with mode RTLD_GLOBAL instead
        # of the default RTLD_LOCAL.  Without this, you end up with
        # libraries not being loadable, resulting in "Symbol not found"
        # errors
        mode = ctypes.RTLD_GLOBAL

    def getplatformpaths(self, libname):
        if os.path.pathsep in libname:
            names = [libname]
        else:
            names = [format % libname for format in self.name_formats]

        for dir in self.getdirs(libname):
            for name in names:
                yield os.path.join(dir, name)

    def getdirs(self, libname):
        """Implements the dylib search as specified in Apple documentation:

        http://developer.apple.com/documentation/DeveloperTools/Conceptual/
            DynamicLibraries/Articles/DynamicLibraryUsageGuidelines.html

        Before commencing the standard search, the method first checks
        the bundle's ``Frameworks`` directory if the application is running
        within a bundle (OS X .app).
        """

        dyld_fallback_library_path = _environ_path("DYLD_FALLBACK_LIBRARY_PATH")
        if not dyld_fallback_library_path:
            dyld_fallback_library_path = [os.path.expanduser("~/lib"), "/usr/local/lib", "/usr/lib"]

        dirs = []

        if "/" in libname:
            dirs.extend(_environ_path("DYLD_LIBRARY_PATH"))
        else:
            dirs.extend(_environ_path("LD_LIBRARY_PATH"))
            dirs.extend(_environ_path("DYLD_LIBRARY_PATH"))

        if hasattr(sys, "frozen") and sys.frozen == "macosx_app":
            dirs.append(os.path.join(os.environ["RESOURCEPATH"], "..", "Frameworks"))

        dirs.extend(dyld_fallback_library_path)

        return dirs


# Posix


class PosixLibraryLoader(LibraryLoader):
    _ld_so_cache = None

    _include = re.compile(r"^\s*include\s+(?P<pattern>.*)")

    class _Directories(dict):
        def __init__(self):
            self.order = 0

        def add(self, directory):
            if len(directory) > 1:
                directory = directory.rstrip(os.path.sep)
            # only adds and updates order if exists and not already in set
            if not os.path.exists(directory):
                return
            o = self.setdefault(directory, self.order)
            if o == self.order:
                self.order += 1

        def extend(self, directories):
            for d in directories:
                self.add(d)

        def ordered(self):
            return (i[0] for i in sorted(self.items(), key=lambda D: D[1]))

    def _get_ld_so_conf_dirs(self, conf, dirs):
        """
        Recursive funtion to help parse all ld.so.conf files, including proper
        handling of the `include` directive.
        """

        try:
            with open(conf) as f:
                for D in f:
                    D = D.strip()
                    if not D:
                        continue

                    m = self._include.match(D)
                    if not m:
                        dirs.add(D)
                    else:
                        for D2 in glob.glob(m.group("pattern")):
                            self._get_ld_so_conf_dirs(D2, dirs)
        except IOError:
            pass

    def _create_ld_so_cache(self):
        # Recreate search path followed by ld.so.  This is going to be
        # slow to build, and incorrect (ld.so uses ld.so.cache, which may
        # not be up-to-date).  Used only as fallback for distros without
        # /sbin/ldconfig.
        #
        # We assume the DT_RPATH and DT_RUNPATH binary sections are omitted.

        directories = self._Directories()
        for name in (
            "LD_LIBRARY_PATH",
            "SHLIB_PATH",  # HPUX
            "LIBPATH",  # OS/2, AIX
            "LIBRARY_PATH",  # BE/OS
        ):
            if name in os.environ:
                directories.extend(os.environ[name].split(os.pathsep))

        self._get_ld_so_conf_dirs("/etc/ld.so.conf", directories)

        bitage = platform.architecture()[0]

        unix_lib_dirs_list = []
        if bitage.startswith("64"):
            # prefer 64 bit if that is our arch
            unix_lib_dirs_list += ["/lib64", "/usr/lib64"]

        # must include standard libs, since those paths are also used by 64 bit
        # installs
        unix_lib_dirs_list += ["/lib", "/usr/lib"]
        if sys.platform.startswith("linux"):
            # Try and support multiarch work in Ubuntu
            # https://wiki.ubuntu.com/MultiarchSpec
            if bitage.startswith("32"):
                # Assume Intel/AMD x86 compat
                unix_lib_dirs_list += ["/lib/i386-linux-gnu", "/usr/lib/i386-linux-gnu"]
            elif bitage.startswith("64"):
                # Assume Intel/AMD x86 compat
                unix_lib_dirs_list += ["/lib/x86_64-linux-gnu", "/usr/lib/x86_64-linux-gnu"]
            else:
                # guess...
                unix_lib_dirs_list += glob.glob("/lib/*linux-gnu")
        directories.extend(unix_lib_dirs_list)

        cache = {}
        lib_re = re.compile(r"lib(.*)\.s[ol]")
        ext_re = re.compile(r"\.s[ol]$")
        for dir in directories.ordered():
            try:
                for path in glob.glob("%s/*.s[ol]*" % dir):
                    file = os.path.basename(path)

                    # Index by filename
                    cache_i = cache.setdefault(file, set())
                    cache_i.add(path)

                    # Index by library name
                    match = lib_re.match(file)
                    if match:
                        library = match.group(1)
                        cache_i = cache.setdefault(library, set())
                        cache_i.add(path)
            except OSError:
                pass

        self._ld_so_cache = cache

    def getplatformpaths(self, libname):
        if self._ld_so_cache is None:
            self._create_ld_so_cache()

        result = self._ld_so_cache.get(libname, set())
        for i in result:
            # we iterate through all found paths for library, since we may have
            # actually found multiple architectures or other library types that
            # may not load
            yield i


# Windows


class WindowsLibraryLoader(LibraryLoader):
    name_formats = ["%s.dll", "lib%s.dll", "%slib.dll", "%s"]

    class Lookup(LibraryLoader.Lookup):
        def __init__(self, path):
            super(WindowsLibraryLoader.Lookup, self).__init__(path)
            self.access["stdcall"] = ctypes.windll.LoadLibrary(path)


# Platform switching

# If your value of sys.platform does not appear in this dict, please contact
# the Ctypesgen maintainers.

loaderclass = {
    "darwin": DarwinLibraryLoader,
    "cygwin": WindowsLibraryLoader,
    "win32": WindowsLibraryLoader,
    "msys": WindowsLibraryLoader,
}

load_library = loaderclass.get(sys.platform, PosixLibraryLoader)()


def add_library_search_dirs(other_dirs):
    """
    Add libraries to search paths.
    If library paths are relative, convert them to absolute with respect to this
    file's directory
    """
    for F in other_dirs:
        if not os.path.isabs(F):
            F = os.path.abspath(F)
        load_library.other_dirs.append(F)


del loaderclass

# End loader

add_library_search_dirs(['.', '/Users/plewis/Documents/GitHub/librat/src'])

# Begin libraries
_libs["libratlib.so"] = load_library("libratlib.so")

# 1 libraries
# End libraries

# No modules

# /Users/plewis/Documents/GitHub/librat/src/hipl_format.h: 26
class struct_header(Structure):
    pass

struct_header.__slots__ = [
    'orig_name',
    'seq_name',
    'num_frame',
    'orig_date',
    'rows',
    'cols',
    'bits_per_pixel',
    'bit_packing',
    'pixel_format',
    'seq_history',
    'seq_desc',
]
struct_header._fields_ = [
    ('orig_name', String),
    ('seq_name', String),
    ('num_frame', c_int),
    ('orig_date', String),
    ('rows', c_int),
    ('cols', c_int),
    ('bits_per_pixel', c_int),
    ('bit_packing', c_int),
    ('pixel_format', c_int),
    ('seq_history', String),
    ('seq_desc', String),
]

# /Users/plewis/Documents/GitHub/librat/src/useful3.h: 40
class struct_anon_2(Structure):
    pass

struct_anon_2.__slots__ = [
    'x',
    'y',
    'z',
]
struct_anon_2._fields_ = [
    ('x', c_double),
    ('y', c_double),
    ('z', c_double),
]

triplet = struct_anon_2# /Users/plewis/Documents/GitHub/librat/src/useful3.h: 40

# /Users/plewis/Documents/GitHub/librat/src/useful3.h: 50
class struct_anon_3(Structure):
    pass

struct_anon_3.__slots__ = [
    'data',
]
struct_anon_3._fields_ = [
    ('data', (c_double * int(3)) * int(3)),
]

matrix3 = struct_anon_3# /Users/plewis/Documents/GitHub/librat/src/useful3.h: 50

# /Users/plewis/Documents/GitHub/librat/src/useful3.h: 58
class struct_anon_4(Structure):
    pass

struct_anon_4.__slots__ = [
    'data',
]
struct_anon_4._fields_ = [
    ('data', (c_double * int(4)) * int(4)),
]

matrix4 = struct_anon_4# /Users/plewis/Documents/GitHub/librat/src/useful3.h: 58

# /Users/plewis/Documents/GitHub/librat/src/useful3.h: 65
class struct_anon_5(Structure):
    pass

struct_anon_5.__slots__ = [
    'v',
]
struct_anon_5._fields_ = [
    ('v', c_double * int(4)),
]

vector4 = struct_anon_5# /Users/plewis/Documents/GitHub/librat/src/useful3.h: 65

# /Users/plewis/Documents/GitHub/librat/src/useful3.h: 72
class struct_anon_6(Structure):
    pass

struct_anon_6.__slots__ = [
    'ox',
    'oy',
    'sx',
    'sy',
    'gx',
    'gy',
    'row',
    'col',
]
struct_anon_6._fields_ = [
    ('ox', c_double),
    ('oy', c_double),
    ('sx', c_double),
    ('sy', c_double),
    ('gx', c_double),
    ('gy', c_double),
    ('row', c_int),
    ('col', c_int),
]

image_descriptor = struct_anon_6# /Users/plewis/Documents/GitHub/librat/src/useful3.h: 72

# /Users/plewis/Documents/GitHub/librat/src/useful3.h: 79
class struct_anon_7(Structure):
    pass

struct_anon_7.__slots__ = [
    'b',
    'i',
]
struct_anon_7._fields_ = [
    ('b', String),
    ('i', POINTER(c_int)),
]

char_int = struct_anon_7# /Users/plewis/Documents/GitHub/librat/src/useful3.h: 79

# /Users/plewis/Documents/GitHub/librat/src/useful3.h: 84
class struct_anon_8(Structure):
    pass

struct_anon_8.__slots__ = [
    'flag',
    'Type',
    'no_of_parameters',
]
struct_anon_8._fields_ = [
    ('flag', POINTER(POINTER(c_char))),
    ('Type', String),
    ('no_of_parameters', String),
]

Data_type = struct_anon_8# /Users/plewis/Documents/GitHub/librat/src/useful3.h: 84

# /Users/plewis/Documents/GitHub/librat/src/useful3.h: 90
class struct_anon_9(Structure):
    pass

struct_anon_9.__slots__ = [
    'x',
    'y',
]
struct_anon_9._fields_ = [
    ('x', c_double),
    ('y', c_double),
]

pair = struct_anon_9# /Users/plewis/Documents/GitHub/librat/src/useful3.h: 90

# /Users/plewis/Documents/GitHub/librat/src/useful3.h: 96
class struct_anon_10(Structure):
    pass

struct_anon_10.__slots__ = [
    'x',
    'y',
]
struct_anon_10._fields_ = [
    ('x', c_int),
    ('y', c_int),
]

ipair = struct_anon_10# /Users/plewis/Documents/GitHub/librat/src/useful3.h: 96

# /Users/plewis/Documents/GitHub/librat/src/useful3.h: 99
class struct_anon_11(Structure):
    pass

struct_anon_11.__slots__ = [
    'column',
]
struct_anon_11._fields_ = [
    ('column', pair * int(2)),
]

matrix_2D = struct_anon_11# /Users/plewis/Documents/GitHub/librat/src/useful3.h: 99

# /Users/plewis/Documents/GitHub/librat/src/useful3.h: 102
class struct_anon_12(Structure):
    pass

struct_anon_12.__slots__ = [
    'vertex',
]
struct_anon_12._fields_ = [
    ('vertex', pair * int(3)),
]

triangle = struct_anon_12# /Users/plewis/Documents/GitHub/librat/src/useful3.h: 102

# /Users/plewis/Documents/GitHub/librat/src/vectors2.h: 5
if _libs["libratlib.so"].has("nearly", "cdecl"):
    nearly = _libs["libratlib.so"].get("nearly", "cdecl")
    nearly.argtypes = [c_int, c_double]
    nearly.restype = c_int

# /Users/plewis/Documents/GitHub/librat/src/vectors2.h: 6
if _libs["libratlib.so"].has("test_program_ok", "cdecl"):
    test_program_ok = _libs["libratlib.so"].get("test_program_ok", "cdecl")
    test_program_ok.argtypes = []
    test_program_ok.restype = None

# /Users/plewis/Documents/GitHub/librat/src/vectors2.h: 7
if _libs["libratlib.so"].has("matrix3_ip", "cdecl"):
    matrix3_ip = _libs["libratlib.so"].get("matrix3_ip", "cdecl")
    matrix3_ip.argtypes = [triplet, triplet, triplet]
    matrix3_ip.restype = matrix3

# /Users/plewis/Documents/GitHub/librat/src/vectors2.h: 8
if _libs["libratlib.so"].has("matrix3_mult", "cdecl"):
    matrix3_mult = _libs["libratlib.so"].get("matrix3_mult", "cdecl")
    matrix3_mult.argtypes = [matrix3, triplet]
    matrix3_mult.restype = triplet

# /Users/plewis/Documents/GitHub/librat/src/vectors2.h: 9
if _libs["libratlib.so"].has("matrix3_copy", "cdecl"):
    matrix3_copy = _libs["libratlib.so"].get("matrix3_copy", "cdecl")
    matrix3_copy.argtypes = [matrix3]
    matrix3_copy.restype = matrix3

# /Users/plewis/Documents/GitHub/librat/src/vectors2.h: 10
if _libs["libratlib.so"].has("matrix3_inverse", "cdecl"):
    matrix3_inverse = _libs["libratlib.so"].get("matrix3_inverse", "cdecl")
    matrix3_inverse.argtypes = [matrix3]
    matrix3_inverse.restype = matrix3

# /Users/plewis/Documents/GitHub/librat/src/vectors2.h: 11
if _libs["libratlib.so"].has("m3_inverse", "cdecl"):
    m3_inverse = _libs["libratlib.so"].get("m3_inverse", "cdecl")
    m3_inverse.argtypes = [POINTER(matrix3)]
    m3_inverse.restype = c_double

# /Users/plewis/Documents/GitHub/librat/src/vectors2.h: 12
if _libs["libratlib.so"].has("inverse3", "cdecl"):
    inverse3 = _libs["libratlib.so"].get("inverse3", "cdecl")
    inverse3.argtypes = [matrix3]
    inverse3.restype = matrix3

# /Users/plewis/Documents/GitHub/librat/src/vectors2.h: 13
if _libs["libratlib.so"].has("transpose3", "cdecl"):
    transpose3 = _libs["libratlib.so"].get("transpose3", "cdecl")
    transpose3.argtypes = [matrix3]
    transpose3.restype = matrix3

# /Users/plewis/Documents/GitHub/librat/src/vectors2.h: 14
if _libs["libratlib.so"].has("matrix_sign", "cdecl"):
    matrix_sign = _libs["libratlib.so"].get("matrix_sign", "cdecl")
    matrix_sign.argtypes = [c_int]
    matrix_sign.restype = c_int

# /Users/plewis/Documents/GitHub/librat/src/vectors2.h: 15
if _libs["libratlib.so"].has("scale", "cdecl"):
    scale = _libs["libratlib.so"].get("scale", "cdecl")
    scale.argtypes = [matrix3]
    scale.restype = c_double

# /Users/plewis/Documents/GitHub/librat/src/vectors2.h: 16
if _libs["libratlib.so"].has("boundck", "cdecl"):
    boundck = _libs["libratlib.so"].get("boundck", "cdecl")
    boundck.argtypes = [c_int]
    boundck.restype = c_int

# /Users/plewis/Documents/GitHub/librat/src/vectors2.h: 17
if _libs["libratlib.so"].has("cofactors", "cdecl"):
    cofactors = _libs["libratlib.so"].get("cofactors", "cdecl")
    cofactors.argtypes = [matrix3, c_int, c_int]
    cofactors.restype = c_double

# /Users/plewis/Documents/GitHub/librat/src/vectors2.h: 18
if _libs["libratlib.so"].has("vector_compare", "cdecl"):
    vector_compare = _libs["libratlib.so"].get("vector_compare", "cdecl")
    vector_compare.argtypes = [c_int, triplet, triplet]
    vector_compare.restype = c_int

# /Users/plewis/Documents/GitHub/librat/src/vectors2.h: 19
if _libs["libratlib.so"].has("vector_plus", "cdecl"):
    vector_plus = _libs["libratlib.so"].get("vector_plus", "cdecl")
    vector_plus.argtypes = [triplet, triplet]
    vector_plus.restype = triplet

# /Users/plewis/Documents/GitHub/librat/src/vectors2.h: 20
if _libs["libratlib.so"].has("vector_minus", "cdecl"):
    vector_minus = _libs["libratlib.so"].get("vector_minus", "cdecl")
    vector_minus.argtypes = [triplet, triplet]
    vector_minus.restype = triplet

# /Users/plewis/Documents/GitHub/librat/src/vectors2.h: 21
if _libs["libratlib.so"].has("spherical_to_cartesian", "cdecl"):
    spherical_to_cartesian = _libs["libratlib.so"].get("spherical_to_cartesian", "cdecl")
    spherical_to_cartesian.argtypes = [triplet, c_int]
    spherical_to_cartesian.restype = triplet

# /Users/plewis/Documents/GitHub/librat/src/vectors2.h: 22
if _libs["libratlib.so"].has("cartesian_to_spherical", "cdecl"):
    cartesian_to_spherical = _libs["libratlib.so"].get("cartesian_to_spherical", "cdecl")
    cartesian_to_spherical.argtypes = [triplet]
    cartesian_to_spherical.restype = triplet

# /Users/plewis/Documents/GitHub/librat/src/vectors2.h: 23
if _libs["libratlib.so"].has("xy_angle", "cdecl"):
    xy_angle = _libs["libratlib.so"].get("xy_angle", "cdecl")
    xy_angle.argtypes = [triplet]
    xy_angle.restype = c_double

# /Users/plewis/Documents/GitHub/librat/src/vectors2.h: 24
if _libs["libratlib.so"].has("multiply_vector", "cdecl"):
    multiply_vector = _libs["libratlib.so"].get("multiply_vector", "cdecl")
    multiply_vector.argtypes = [triplet, triplet]
    multiply_vector.restype = triplet

# /Users/plewis/Documents/GitHub/librat/src/vectors2.h: 25
if _libs["libratlib.so"].has("V_factor", "cdecl"):
    V_factor = _libs["libratlib.so"].get("V_factor", "cdecl")
    V_factor.argtypes = [triplet, c_double]
    V_factor.restype = triplet

# /Users/plewis/Documents/GitHub/librat/src/vectors2.h: 26
if _libs["libratlib.so"].has("normalise", "cdecl"):
    normalise = _libs["libratlib.so"].get("normalise", "cdecl")
    normalise.argtypes = [triplet]
    normalise.restype = triplet

# /Users/plewis/Documents/GitHub/librat/src/vectors2.h: 27
if _libs["libratlib.so"].has("V_dot", "cdecl"):
    V_dot = _libs["libratlib.so"].get("V_dot", "cdecl")
    V_dot.argtypes = [triplet, triplet]
    V_dot.restype = c_double

# /Users/plewis/Documents/GitHub/librat/src/vectors2.h: 28
if _libs["libratlib.so"].has("vector_cross", "cdecl"):
    vector_cross = _libs["libratlib.so"].get("vector_cross", "cdecl")
    vector_cross.argtypes = [triplet, triplet]
    vector_cross.restype = triplet

# /Users/plewis/Documents/GitHub/librat/src/vectors2.h: 29
if _libs["libratlib.so"].has("V_mod", "cdecl"):
    V_mod = _libs["libratlib.so"].get("V_mod", "cdecl")
    V_mod.argtypes = [triplet]
    V_mod.restype = c_double

# /Users/plewis/Documents/GitHub/librat/src/vectors2.h: 30
if _libs["libratlib.so"].has("vector_copy", "cdecl"):
    vector_copy = _libs["libratlib.so"].get("vector_copy", "cdecl")
    vector_copy.argtypes = [triplet]
    vector_copy.restype = triplet

# /Users/plewis/Documents/GitHub/librat/src/vectors2.h: 31
if _libs["libratlib.so"].has("vector_copy2", "cdecl"):
    vector_copy2 = _libs["libratlib.so"].get("vector_copy2", "cdecl")
    vector_copy2.argtypes = [c_double, c_double, c_double]
    vector_copy2.restype = triplet

# /Users/plewis/Documents/GitHub/librat/src/vectors2.h: 32
if _libs["libratlib.so"].has("rotate_vector", "cdecl"):
    rotate_vector = _libs["libratlib.so"].get("rotate_vector", "cdecl")
    rotate_vector.argtypes = [triplet, triplet]
    rotate_vector.restype = triplet

# /Users/plewis/Documents/GitHub/librat/src/vectors2.h: 33
if _libs["libratlib.so"].has("rotate_about", "cdecl"):
    rotate_about = _libs["libratlib.so"].get("rotate_about", "cdecl")
    rotate_about.argtypes = [triplet, triplet, c_double]
    rotate_about.restype = triplet

# /Users/plewis/Documents/GitHub/librat/src/vectors2.h: 34
if _libs["libratlib.so"].has("OLDrotate_about", "cdecl"):
    OLDrotate_about = _libs["libratlib.so"].get("OLDrotate_about", "cdecl")
    OLDrotate_about.argtypes = [triplet, triplet, c_double]
    OLDrotate_about.restype = triplet

# /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/math.h: 361
if _libs["libratlib.so"].has("exp", "cdecl"):
    exp = _libs["libratlib.so"].get("exp", "cdecl")
    exp.argtypes = [c_double]
    exp.restype = c_double

# /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/math.h: 433
if _libs["libratlib.so"].has("sqrt", "cdecl"):
    sqrt = _libs["libratlib.so"].get("sqrt", "cdecl")
    sqrt.argtypes = [c_double]
    sqrt.restype = c_double

# /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/malloc/_malloc.h: 41
if _libs["libratlib.so"].has("calloc", "cdecl"):
    calloc = _libs["libratlib.so"].get("calloc", "cdecl")
    calloc.argtypes = [c_size_t, c_size_t]
    calloc.restype = POINTER(c_ubyte)
    calloc.errcheck = lambda v,*a : cast(v, c_void_p)

# /Users/plewis/Documents/GitHub/librat/src/4D_vectors.h: 21
if _libs["libratlib.so"].has("load_identity_matrix4", "cdecl"):
    load_identity_matrix4 = _libs["libratlib.so"].get("load_identity_matrix4", "cdecl")
    load_identity_matrix4.argtypes = []
    load_identity_matrix4.restype = matrix4

# /Users/plewis/Documents/GitHub/librat/src/4D_vectors.h: 22
if _libs["libratlib.so"].has("pload_identity_matrix4", "cdecl"):
    pload_identity_matrix4 = _libs["libratlib.so"].get("pload_identity_matrix4", "cdecl")
    pload_identity_matrix4.argtypes = [POINTER(matrix4)]
    pload_identity_matrix4.restype = None

# /Users/plewis/Documents/GitHub/librat/src/4D_vectors.h: 23
if _libs["libratlib.so"].has("load_translation_matrix4", "cdecl"):
    load_translation_matrix4 = _libs["libratlib.so"].get("load_translation_matrix4", "cdecl")
    load_translation_matrix4.argtypes = [triplet]
    load_translation_matrix4.restype = matrix4

# /Users/plewis/Documents/GitHub/librat/src/4D_vectors.h: 24
if _libs["libratlib.so"].has("multiply_matrix4_matrix4", "cdecl"):
    multiply_matrix4_matrix4 = _libs["libratlib.so"].get("multiply_matrix4_matrix4", "cdecl")
    multiply_matrix4_matrix4.argtypes = [matrix4, matrix4]
    multiply_matrix4_matrix4.restype = matrix4

# /Users/plewis/Documents/GitHub/librat/src/4D_vectors.h: 25
if _libs["libratlib.so"].has("vector_copy4", "cdecl"):
    vector_copy4 = _libs["libratlib.so"].get("vector_copy4", "cdecl")
    vector_copy4.argtypes = [c_double, c_double, c_double, c_double]
    vector_copy4.restype = vector4

# /Users/plewis/Documents/GitHub/librat/src/4D_vectors.h: 26
if _libs["libratlib.so"].has("multiply_matrix4_vector4", "cdecl"):
    multiply_matrix4_vector4 = _libs["libratlib.so"].get("multiply_matrix4_vector4", "cdecl")
    multiply_matrix4_vector4.argtypes = [matrix4, vector4]
    multiply_matrix4_vector4.restype = vector4

# /Users/plewis/Documents/GitHub/librat/src/4D_vectors.h: 27
if _libs["libratlib.so"].has("multiply_matrix4_vector3", "cdecl"):
    multiply_matrix4_vector3 = _libs["libratlib.so"].get("multiply_matrix4_vector3", "cdecl")
    multiply_matrix4_vector3.argtypes = [matrix4, triplet, c_int]
    multiply_matrix4_vector3.restype = triplet

# /Users/plewis/Documents/GitHub/librat/src/4D_vectors.h: 28
if _libs["libratlib.so"].has("load_scaling_matrix4", "cdecl"):
    load_scaling_matrix4 = _libs["libratlib.so"].get("load_scaling_matrix4", "cdecl")
    load_scaling_matrix4.argtypes = [c_double]
    load_scaling_matrix4.restype = matrix4

# /Users/plewis/Documents/GitHub/librat/src/4D_vectors.h: 29
if _libs["libratlib.so"].has("load_differential_scaling_matrix4", "cdecl"):
    load_differential_scaling_matrix4 = _libs["libratlib.so"].get("load_differential_scaling_matrix4", "cdecl")
    load_differential_scaling_matrix4.argtypes = [triplet]
    load_differential_scaling_matrix4.restype = matrix4

# /Users/plewis/Documents/GitHub/librat/src/4D_vectors.h: 30
if _libs["libratlib.so"].has("load_x_axis_rotation_matrix4", "cdecl"):
    load_x_axis_rotation_matrix4 = _libs["libratlib.so"].get("load_x_axis_rotation_matrix4", "cdecl")
    load_x_axis_rotation_matrix4.argtypes = [c_double]
    load_x_axis_rotation_matrix4.restype = matrix4

# /Users/plewis/Documents/GitHub/librat/src/4D_vectors.h: 31
if _libs["libratlib.so"].has("load_y_axis_rotation_matrix4", "cdecl"):
    load_y_axis_rotation_matrix4 = _libs["libratlib.so"].get("load_y_axis_rotation_matrix4", "cdecl")
    load_y_axis_rotation_matrix4.argtypes = [c_double]
    load_y_axis_rotation_matrix4.restype = matrix4

# /Users/plewis/Documents/GitHub/librat/src/4D_vectors.h: 32
if _libs["libratlib.so"].has("load_z_axis_rotation_matrix4", "cdecl"):
    load_z_axis_rotation_matrix4 = _libs["libratlib.so"].get("load_z_axis_rotation_matrix4", "cdecl")
    load_z_axis_rotation_matrix4.argtypes = [c_double]
    load_z_axis_rotation_matrix4.restype = matrix4

# /Users/plewis/Documents/GitHub/librat/src/4D_vectors.h: 33
if _libs["libratlib.so"].has("load_scaling_fix_point_matrix4", "cdecl"):
    load_scaling_fix_point_matrix4 = _libs["libratlib.so"].get("load_scaling_fix_point_matrix4", "cdecl")
    load_scaling_fix_point_matrix4.argtypes = [c_double, triplet]
    load_scaling_fix_point_matrix4.restype = matrix4

# /Users/plewis/Documents/GitHub/librat/src/4D_vectors.h: 34
if _libs["libratlib.so"].has("load_differential_scaling_fix_point_matrix4", "cdecl"):
    load_differential_scaling_fix_point_matrix4 = _libs["libratlib.so"].get("load_differential_scaling_fix_point_matrix4", "cdecl")
    load_differential_scaling_fix_point_matrix4.argtypes = [triplet, triplet]
    load_differential_scaling_fix_point_matrix4.restype = matrix4

# /Users/plewis/Documents/GitHub/librat/src/4D_vectors.h: 35
if _libs["libratlib.so"].has("load_x_axis_rotation_fix_point_matrix4", "cdecl"):
    load_x_axis_rotation_fix_point_matrix4 = _libs["libratlib.so"].get("load_x_axis_rotation_fix_point_matrix4", "cdecl")
    load_x_axis_rotation_fix_point_matrix4.argtypes = [c_double, triplet]
    load_x_axis_rotation_fix_point_matrix4.restype = matrix4

# /Users/plewis/Documents/GitHub/librat/src/4D_vectors.h: 36
if _libs["libratlib.so"].has("load_y_axis_rotation_fix_point_matrix4", "cdecl"):
    load_y_axis_rotation_fix_point_matrix4 = _libs["libratlib.so"].get("load_y_axis_rotation_fix_point_matrix4", "cdecl")
    load_y_axis_rotation_fix_point_matrix4.argtypes = [c_double, triplet]
    load_y_axis_rotation_fix_point_matrix4.restype = matrix4

# /Users/plewis/Documents/GitHub/librat/src/4D_vectors.h: 37
if _libs["libratlib.so"].has("load_z_axis_rotation_fix_point_matrix4", "cdecl"):
    load_z_axis_rotation_fix_point_matrix4 = _libs["libratlib.so"].get("load_z_axis_rotation_fix_point_matrix4", "cdecl")
    load_z_axis_rotation_fix_point_matrix4.argtypes = [c_double, triplet]
    load_z_axis_rotation_fix_point_matrix4.restype = matrix4

# /Users/plewis/Documents/GitHub/librat/src/4D_vectors.h: 38
if _libs["libratlib.so"].has("transpose4", "cdecl"):
    transpose4 = _libs["libratlib.so"].get("transpose4", "cdecl")
    transpose4.argtypes = [matrix4]
    transpose4.restype = matrix4

# /Users/plewis/Documents/GitHub/librat/src/4D_vectors.h: 39
if _libs["libratlib.so"].has("clock_count", "cdecl"):
    clock_count = _libs["libratlib.so"].get("clock_count", "cdecl")
    clock_count.argtypes = [c_int, c_int]
    clock_count.restype = c_int

# /Users/plewis/Documents/GitHub/librat/src/4D_vectors.h: 40
if _libs["libratlib.so"].has("modulus_matrix4", "cdecl"):
    modulus_matrix4 = _libs["libratlib.so"].get("modulus_matrix4", "cdecl")
    modulus_matrix4.argtypes = [matrix4]
    modulus_matrix4.restype = c_double

# /Users/plewis/Documents/GitHub/librat/src/4D_vectors.h: 41
if _libs["libratlib.so"].has("inverse_matrix4", "cdecl"):
    inverse_matrix4 = _libs["libratlib.so"].get("inverse_matrix4", "cdecl")
    inverse_matrix4.argtypes = [matrix4]
    inverse_matrix4.restype = matrix4

# /Users/plewis/Documents/GitHub/librat/src/4D_vectors.h: 43
if _libs["libratlib.so"].has("load_arbitrary_axis_rotation_matrix4", "cdecl"):
    load_arbitrary_axis_rotation_matrix4 = _libs["libratlib.so"].get("load_arbitrary_axis_rotation_matrix4", "cdecl")
    load_arbitrary_axis_rotation_matrix4.argtypes = [triplet, c_double, triplet]
    load_arbitrary_axis_rotation_matrix4.restype = matrix4

# /Users/plewis/Documents/GitHub/librat/src/4D_vectors.h: 44
for _lib in _libs.values():
    if not _lib.has("__multiply_matrix4_vector3", "cdecl"):
        continue
    __multiply_matrix4_vector3 = _lib.get("__multiply_matrix4_vector3", "cdecl")
    __multiply_matrix4_vector3.argtypes = [matrix4, triplet]
    __multiply_matrix4_vector3.restype = triplet
    break

# /Users/plewis/Documents/GitHub/librat/src/4D_vectors.h: 45
if _libs["libratlib.so"].has("old_multiply_matrix4_vector3", "cdecl"):
    old_multiply_matrix4_vector3 = _libs["libratlib.so"].get("old_multiply_matrix4_vector3", "cdecl")
    old_multiply_matrix4_vector3.argtypes = [matrix4, triplet]
    old_multiply_matrix4_vector3.restype = triplet

# /Users/plewis/Documents/GitHub/librat/src/4D_vectors.h: 46
if _libs["libratlib.so"].has("multiply_upper_matrix3_vector3", "cdecl"):
    multiply_upper_matrix3_vector3 = _libs["libratlib.so"].get("multiply_upper_matrix3_vector3", "cdecl")
    multiply_upper_matrix3_vector3.argtypes = [matrix4, triplet]
    multiply_upper_matrix3_vector3.restype = triplet

# /Users/plewis/Documents/GitHub/librat/src/2D_vectors.h: 20
if _libs["libratlib.so"].has("FsIgN", "cdecl"):
    FsIgN = _libs["libratlib.so"].get("FsIgN", "cdecl")
    FsIgN.argtypes = [c_float]
    FsIgN.restype = c_int

# /Users/plewis/Documents/GitHub/librat/src/2D_vectors.h: 21
if _libs["libratlib.so"].has("same_sign", "cdecl"):
    same_sign = _libs["libratlib.so"].get("same_sign", "cdecl")
    same_sign.argtypes = [pair, pair]
    same_sign.restype = pair

# /Users/plewis/Documents/GitHub/librat/src/2D_vectors.h: 22
if _libs["libratlib.so"].has("normalise_2D", "cdecl"):
    normalise_2D = _libs["libratlib.so"].get("normalise_2D", "cdecl")
    normalise_2D.argtypes = [pair]
    normalise_2D.restype = pair

# /Users/plewis/Documents/GitHub/librat/src/2D_vectors.h: 23
if _libs["libratlib.so"].has("fabs_2D", "cdecl"):
    fabs_2D = _libs["libratlib.so"].get("fabs_2D", "cdecl")
    fabs_2D.argtypes = [pair]
    fabs_2D.restype = pair

# /Users/plewis/Documents/GitHub/librat/src/2D_vectors.h: 24
if _libs["libratlib.so"].has("copy_2D", "cdecl"):
    copy_2D = _libs["libratlib.so"].get("copy_2D", "cdecl")
    copy_2D.argtypes = [c_float, c_float]
    copy_2D.restype = pair

# /Users/plewis/Documents/GitHub/librat/src/2D_vectors.h: 25
if _libs["libratlib.so"].has("icopy_2D", "cdecl"):
    icopy_2D = _libs["libratlib.so"].get("icopy_2D", "cdecl")
    icopy_2D.argtypes = [c_int, c_int]
    icopy_2D.restype = ipair

# /Users/plewis/Documents/GitHub/librat/src/2D_vectors.h: 26
if _libs["libratlib.so"].has("bbox", "cdecl"):
    bbox = _libs["libratlib.so"].get("bbox", "cdecl")
    bbox.argtypes = [POINTER(pair), c_int]
    bbox.restype = POINTER(pair)

# /Users/plewis/Documents/GitHub/librat/src/2D_vectors.h: 27
if _libs["libratlib.so"].has("odd_check", "cdecl"):
    odd_check = _libs["libratlib.so"].get("odd_check", "cdecl")
    odd_check.argtypes = [c_int]
    odd_check.restype = c_int

# /Users/plewis/Documents/GitHub/librat/src/2D_vectors.h: 28
if _libs["libratlib.so"].has("check_group_belonging", "cdecl"):
    check_group_belonging = _libs["libratlib.so"].get("check_group_belonging", "cdecl")
    check_group_belonging.argtypes = [pair, triangle, pair]
    check_group_belonging.restype = c_int

# /Users/plewis/Documents/GitHub/librat/src/2D_vectors.h: 29
if _libs["libratlib.so"].has("plus_2D", "cdecl"):
    plus_2D = _libs["libratlib.so"].get("plus_2D", "cdecl")
    plus_2D.argtypes = [pair, pair]
    plus_2D.restype = pair

# /Users/plewis/Documents/GitHub/librat/src/2D_vectors.h: 30
if _libs["libratlib.so"].has("iplus_2D", "cdecl"):
    iplus_2D = _libs["libratlib.so"].get("iplus_2D", "cdecl")
    iplus_2D.argtypes = [ipair, ipair]
    iplus_2D.restype = ipair

# /Users/plewis/Documents/GitHub/librat/src/2D_vectors.h: 31
if _libs["libratlib.so"].has("factor_2D", "cdecl"):
    factor_2D = _libs["libratlib.so"].get("factor_2D", "cdecl")
    factor_2D.argtypes = [pair, c_float]
    factor_2D.restype = pair

# /Users/plewis/Documents/GitHub/librat/src/2D_vectors.h: 32
if _libs["libratlib.so"].has("multiply_2D", "cdecl"):
    multiply_2D = _libs["libratlib.so"].get("multiply_2D", "cdecl")
    multiply_2D.argtypes = [pair, pair]
    multiply_2D.restype = pair

# /Users/plewis/Documents/GitHub/librat/src/2D_vectors.h: 33
if _libs["libratlib.so"].has("divide_2D", "cdecl"):
    divide_2D = _libs["libratlib.so"].get("divide_2D", "cdecl")
    divide_2D.argtypes = [pair, pair]
    divide_2D.restype = pair

# /Users/plewis/Documents/GitHub/librat/src/2D_vectors.h: 34
if _libs["libratlib.so"].has("minus_2D", "cdecl"):
    minus_2D = _libs["libratlib.so"].get("minus_2D", "cdecl")
    minus_2D.argtypes = [pair, pair]
    minus_2D.restype = pair

# /Users/plewis/Documents/GitHub/librat/src/2D_vectors.h: 35
if _libs["libratlib.so"].has("compare_2D", "cdecl"):
    compare_2D = _libs["libratlib.so"].get("compare_2D", "cdecl")
    compare_2D.argtypes = [pair, pair]
    compare_2D.restype = c_int

# /Users/plewis/Documents/GitHub/librat/src/2D_vectors.h: 36
if _libs["libratlib.so"].has("determinant_2D", "cdecl"):
    determinant_2D = _libs["libratlib.so"].get("determinant_2D", "cdecl")
    determinant_2D.argtypes = [matrix_2D]
    determinant_2D.restype = c_float

# /Users/plewis/Documents/GitHub/librat/src/2D_vectors.h: 37
if _libs["libratlib.so"].has("factor_matrix_2D", "cdecl"):
    factor_matrix_2D = _libs["libratlib.so"].get("factor_matrix_2D", "cdecl")
    factor_matrix_2D.argtypes = [matrix_2D, c_float]
    factor_matrix_2D.restype = matrix_2D

# /Users/plewis/Documents/GitHub/librat/src/2D_vectors.h: 38
if _libs["libratlib.so"].has("inverse_2D", "cdecl"):
    inverse_2D = _libs["libratlib.so"].get("inverse_2D", "cdecl")
    inverse_2D.argtypes = [matrix_2D]
    inverse_2D.restype = matrix_2D

# /Users/plewis/Documents/GitHub/librat/src/2D_vectors.h: 39
if _libs["libratlib.so"].has("mod_2D", "cdecl"):
    mod_2D = _libs["libratlib.so"].get("mod_2D", "cdecl")
    mod_2D.argtypes = [pair]
    mod_2D.restype = c_float

# /Users/plewis/Documents/GitHub/librat/src/2D_vectors.h: 40
if _libs["libratlib.so"].has("transpose_2D", "cdecl"):
    transpose_2D = _libs["libratlib.so"].get("transpose_2D", "cdecl")
    transpose_2D.argtypes = [matrix_2D]
    transpose_2D.restype = matrix_2D

# /Users/plewis/Documents/GitHub/librat/src/2D_vectors.h: 41
if _libs["libratlib.so"].has("matrix_mult_2D", "cdecl"):
    matrix_mult_2D = _libs["libratlib.so"].get("matrix_mult_2D", "cdecl")
    matrix_mult_2D.argtypes = [matrix_2D, pair]
    matrix_mult_2D.restype = pair

# /Users/plewis/Documents/GitHub/librat/src/2D_vectors.h: 42
if _libs["libratlib.so"].has("add_2D", "cdecl"):
    add_2D = _libs["libratlib.so"].get("add_2D", "cdecl")
    add_2D.argtypes = [pair]
    add_2D.restype = c_float

# /Users/plewis/Documents/GitHub/librat/src/2D_vectors.h: 43
if _libs["libratlib.so"].has("f2i_2D", "cdecl"):
    f2i_2D = _libs["libratlib.so"].get("f2i_2D", "cdecl")
    f2i_2D.argtypes = [pair]
    f2i_2D.restype = ipair

# /Users/plewis/Documents/GitHub/librat/src/2D_vectors.h: 44
if _libs["libratlib.so"].has("i2f_2D", "cdecl"):
    i2f_2D = _libs["libratlib.so"].get("i2f_2D", "cdecl")
    i2f_2D.argtypes = [ipair]
    i2f_2D.restype = pair

# /Users/plewis/Documents/GitHub/librat/src/2D_vectors.h: 45
if _libs["libratlib.so"].has("lo_allign_to_grid", "cdecl"):
    lo_allign_to_grid = _libs["libratlib.so"].get("lo_allign_to_grid", "cdecl")
    lo_allign_to_grid.argtypes = [pair, pair, pair]
    lo_allign_to_grid.restype = pair

# /Users/plewis/Documents/GitHub/librat/src/2D_vectors.h: 46
if _libs["libratlib.so"].has("hi_allign_to_grid", "cdecl"):
    hi_allign_to_grid = _libs["libratlib.so"].get("hi_allign_to_grid", "cdecl")
    hi_allign_to_grid.argtypes = [pair, pair, pair]
    hi_allign_to_grid.restype = pair

# /Users/plewis/Documents/GitHub/librat/src/2D_vectors.h: 47
if _libs["libratlib.so"].has("line_intersection_2D", "cdecl"):
    line_intersection_2D = _libs["libratlib.so"].get("line_intersection_2D", "cdecl")
    line_intersection_2D.argtypes = [pair, pair, pair, pair]
    line_intersection_2D.restype = pair

# /Users/plewis/Documents/GitHub/librat/src/2D_vectors.h: 48
if _libs["libratlib.so"].has("line_distance_2D", "cdecl"):
    line_distance_2D = _libs["libratlib.so"].get("line_distance_2D", "cdecl")
    line_distance_2D.argtypes = [pair, pair, pair, pair]
    line_distance_2D.restype = c_float

# /Users/plewis/Documents/GitHub/librat/src/2D_vectors.h: 49
if _libs["libratlib.so"].has("quick_affine", "cdecl"):
    quick_affine = _libs["libratlib.so"].get("quick_affine", "cdecl")
    quick_affine.argtypes = [c_int, pair, pair, pair]
    quick_affine.restype = triangle

# /Users/plewis/Documents/GitHub/librat/src/2D_vectors.h: 50
if _libs["libratlib.so"].has("affine_transform", "cdecl"):
    affine_transform = _libs["libratlib.so"].get("affine_transform", "cdecl")
    affine_transform.argtypes = [triangle, pair]
    affine_transform.restype = pair

# /Users/plewis/Documents/GitHub/librat/src/2D_vectors.h: 51
if _libs["libratlib.so"].has("backwards_affine_transform", "cdecl"):
    backwards_affine_transform = _libs["libratlib.so"].get("backwards_affine_transform", "cdecl")
    backwards_affine_transform.argtypes = [triangle, pair]
    backwards_affine_transform.restype = pair

# /Users/plewis/Documents/GitHub/librat/src/wavefront.h: 18
class struct_anon_19(Structure):
    pass

struct_anon_19.__slots__ = [
    'vertex',
]
struct_anon_19._fields_ = [
    ('vertex', c_int * int(3)),
]

Ivect3 = struct_anon_19# /Users/plewis/Documents/GitHub/librat/src/wavefront.h: 18

# /Users/plewis/Documents/GitHub/librat/src/wavefront.h: 21
class struct_anon_20(Structure):
    pass

struct_anon_20.__slots__ = [
    'vertex',
]
struct_anon_20._fields_ = [
    ('vertex', c_int * int(16)),
]

Bezier_patch = struct_anon_20# /Users/plewis/Documents/GitHub/librat/src/wavefront.h: 21

# /Users/plewis/Documents/GitHub/librat/src/wavefront.h: 27
class struct_anon_21(Structure):
    pass

struct_anon_21.__slots__ = [
    'new',
    'block',
    'no_of_vertex_numbers',
    'triangles',
]
struct_anon_21._fields_ = [
    ('new', c_int),
    ('block', c_int),
    ('no_of_vertex_numbers', c_int),
    ('triangles', POINTER(Ivect3) * int(200)),
]

Plant_triangles = struct_anon_21# /Users/plewis/Documents/GitHub/librat/src/wavefront.h: 27

# /Users/plewis/Documents/GitHub/librat/src/wavefront.h: 33
class struct_anon_22(Structure):
    pass

struct_anon_22.__slots__ = [
    'new',
    'block',
    'no_of_vertex_numbers',
]
struct_anon_22._fields_ = [
    ('new', c_int),
    ('block', c_int),
    ('no_of_vertex_numbers', c_int),
]

Plant_bzp = struct_anon_22# /Users/plewis/Documents/GitHub/librat/src/wavefront.h: 33

# /Users/plewis/Documents/GitHub/librat/src/wavefront.h: 40
class struct_anon_23(Structure):
    pass

struct_anon_23.__slots__ = [
    'leaf_area',
    'projected_leaf_area',
    'zeniths',
    'look_zeniths',
    'azimuths',
]
struct_anon_23._fields_ = [
    ('leaf_area', c_double),
    ('projected_leaf_area', c_double),
    ('zeniths', POINTER(c_int)),
    ('look_zeniths', POINTER(c_int)),
    ('azimuths', POINTER(c_int)),
]

Plant_area = struct_anon_23# /Users/plewis/Documents/GitHub/librat/src/wavefront.h: 40

# /Users/plewis/Documents/GitHub/librat/src/wavefront.h: 51
class struct_anon_24(Structure):
    pass

struct_anon_24.__slots__ = [
    'in',
    'blocksize',
    'bbox_min',
    'bbox_max',
    'plant_triangles',
    'plant_bzp',
    'plant_area',
]
struct_anon_24._fields_ = [
    ('in', c_int),
    ('blocksize', c_int),
    ('bbox_min', triplet),
    ('bbox_max', triplet),
    ('plant_triangles', Plant_triangles),
    ('plant_bzp', Plant_bzp),
    ('plant_area', Plant_area),
]

Plant = struct_anon_24# /Users/plewis/Documents/GitHub/librat/src/wavefront.h: 51

# /Users/plewis/Documents/GitHub/librat/src/wavefront.h: 61
class struct_anon_25(Structure):
    pass

struct_anon_25.__slots__ = [
    'in',
    'new',
    'displacement',
    'plant_number',
    'plant_area',
]
struct_anon_25._fields_ = [
    ('in', c_int),
    ('new', c_int),
    ('displacement', triplet),
    ('plant_number', c_int),
    ('plant_area', Plant_area),
]

Clone = struct_anon_25# /Users/plewis/Documents/GitHub/librat/src/wavefront.h: 61

# /Users/plewis/Documents/GitHub/librat/src/wavefront.h: 70
class struct_anon_26(Structure):
    pass

struct_anon_26.__slots__ = [
    'new',
    'block',
    'no_of_vertices',
    'blocksize',
    'vertices',
]
struct_anon_26._fields_ = [
    ('new', c_int * int(1000)),
    ('block', c_int),
    ('no_of_vertices', c_int),
    ('blocksize', c_int),
    ('vertices', POINTER(triplet) * int(1000)),
]

VeRtIcEs = struct_anon_26# /Users/plewis/Documents/GitHub/librat/src/wavefront.h: 70

IVECT3 = Ivect3# /Users/plewis/Documents/GitHub/librat/src/wavefront.h: 72

# /Users/plewis/Documents/GitHub/librat/src/hips.h: 11
class struct_anon_27(Structure):
    pass

struct_anon_27.__slots__ = [
    'fdata',
    'bdata',
    'sdata',
    'idata',
]
struct_anon_27._fields_ = [
    ('fdata', POINTER(c_float)),
    ('bdata', POINTER(c_ubyte)),
    ('sdata', POINTER(c_short)),
    ('idata', POINTER(c_int)),
]

Data = struct_anon_27# /Users/plewis/Documents/GitHub/librat/src/hips.h: 11

# /Users/plewis/Documents/GitHub/librat/src/hips.h: 18
for _lib in _libs.values():
    try:
        imagename = (String).in_dll(_lib, "imagename")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/allocate.h: 6
if _libs["libratlib.so"].has("t_allocate", "cdecl"):
    t_allocate = _libs["libratlib.so"].get("t_allocate", "cdecl")
    t_allocate.argtypes = [c_int]
    t_allocate.restype = POINTER(triplet)

# /Users/plewis/Documents/GitHub/librat/src/allocate.h: 7
if _libs["libratlib.so"].has("VM_allocate", "cdecl"):
    VM_allocate = _libs["libratlib.so"].get("VM_allocate", "cdecl")
    VM_allocate.argtypes = [c_uint, c_uint]
    if sizeof(c_int) == sizeof(c_void_p):
        VM_allocate.restype = ReturnString
    else:
        VM_allocate.restype = String
        VM_allocate.errcheck = ReturnString

# /Users/plewis/Documents/GitHub/librat/src/allocate.h: 8
if _libs["libratlib.so"].has("v_allocate", "cdecl"):
    v_allocate = _libs["libratlib.so"].get("v_allocate", "cdecl")
    v_allocate.argtypes = [c_int, c_int]
    v_allocate.restype = POINTER(c_ubyte)
    v_allocate.errcheck = lambda v,*a : cast(v, c_void_p)

# /Users/plewis/Documents/GitHub/librat/src/allocate.h: 9
if _libs["libratlib.so"].has("i_allocate", "cdecl"):
    i_allocate = _libs["libratlib.so"].get("i_allocate", "cdecl")
    i_allocate.argtypes = [c_int]
    i_allocate.restype = POINTER(c_int)

# /Users/plewis/Documents/GitHub/librat/src/allocate.h: 10
if _libs["libratlib.so"].has("B_allocate", "cdecl"):
    B_allocate = _libs["libratlib.so"].get("B_allocate", "cdecl")
    B_allocate.argtypes = [c_int]
    B_allocate.restype = POINTER(Bezier_patch)

# /Users/plewis/Documents/GitHub/librat/src/allocate.h: 11
if _libs["libratlib.so"].has("I_allocate", "cdecl"):
    I_allocate = _libs["libratlib.so"].get("I_allocate", "cdecl")
    I_allocate.argtypes = [c_int]
    I_allocate.restype = POINTER(Ivect3)

# /Users/plewis/Documents/GitHub/librat/src/allocate.h: 12
if _libs["libratlib.so"].has("f_allocate", "cdecl"):
    f_allocate = _libs["libratlib.so"].get("f_allocate", "cdecl")
    f_allocate.argtypes = [c_int]
    f_allocate.restype = POINTER(c_float)

# /Users/plewis/Documents/GitHub/librat/src/allocate.h: 13
if _libs["libratlib.so"].has("d_allocate", "cdecl"):
    d_allocate = _libs["libratlib.so"].get("d_allocate", "cdecl")
    d_allocate.argtypes = [c_int]
    d_allocate.restype = POINTER(c_double)

# /Users/plewis/Documents/GitHub/librat/src/allocate.h: 14
if _libs["libratlib.so"].has("b_allocate", "cdecl"):
    b_allocate = _libs["libratlib.so"].get("b_allocate", "cdecl")
    b_allocate.argtypes = [c_int]
    if sizeof(c_int) == sizeof(c_void_p):
        b_allocate.restype = ReturnString
    else:
        b_allocate.restype = String
        b_allocate.errcheck = ReturnString

# /Users/plewis/Documents/GitHub/librat/src/allocate.h: 15
if _libs["libratlib.so"].has("c_allocate", "cdecl"):
    c_allocate = _libs["libratlib.so"].get("c_allocate", "cdecl")
    c_allocate.argtypes = [c_int]
    if sizeof(c_int) == sizeof(c_void_p):
        c_allocate.restype = ReturnString
    else:
        c_allocate.restype = String
        c_allocate.errcheck = ReturnString

# /Users/plewis/Documents/GitHub/librat/src/allocate.h: 16
if _libs["libratlib.so"].has("ipair_allocate", "cdecl"):
    ipair_allocate = _libs["libratlib.so"].get("ipair_allocate", "cdecl")
    ipair_allocate.argtypes = [c_int]
    ipair_allocate.restype = POINTER(ipair)

# /Users/plewis/Documents/GitHub/librat/src/allocate.h: 17
if _libs["libratlib.so"].has("D_allocate", "cdecl"):
    D_allocate = _libs["libratlib.so"].get("D_allocate", "cdecl")
    D_allocate.argtypes = [c_int, c_int]
    D_allocate.restype = Data

# /Users/plewis/Documents/GitHub/librat/src/intersect_objects.h: 4
class struct_Sort_Hit(Structure):
    pass

struct_Sort_Hit.__slots__ = [
    'D',
    'type',
    'hit',
    'next_hit',
]
struct_Sort_Hit._fields_ = [
    ('D', c_double),
    ('type', c_char),
    ('hit', POINTER(None)),
    ('next_hit', POINTER(struct_Sort_Hit)),
]

# ./rpv.h: 12
class struct_anon_28(Structure):
    pass

struct_anon_28.__slots__ = [
    'rho0',
    'k',
    'bigtheta',
    'rhoc',
]
struct_anon_28._fields_ = [
    ('rho0', c_double),
    ('k', c_double),
    ('bigtheta', c_double),
    ('rhoc', c_double),
]

Rpv = struct_anon_28# ./rpv.h: 12

# ./rpv.h: 14
if _libs["libratlib.so"].has("rpv", "cdecl"):
    rpv = _libs["libratlib.so"].get("rpv", "cdecl")
    rpv.argtypes = [c_double, c_double, c_double, c_double, c_double, c_double, c_double]
    rpv.restype = c_double

# /Users/plewis/Documents/GitHub/librat/src/materials.h: 8
class struct_anon_29(Structure):
    pass

struct_anon_29.__slots__ = [
    'data',
    'hd',
]
struct_anon_29._fields_ = [
    ('data', Data),
    ('hd', struct_header),
]

ImageMap = struct_anon_29# /Users/plewis/Documents/GitHub/librat/src/materials.h: 8

# /Users/plewis/Documents/GitHub/librat/src/materials.h: 10
class struct_Material_table_struct(Structure):
    pass

struct_Material_table_struct.__slots__ = [
    'material',
    'material_proportions',
    'no_of_materials',
    'imagemap',
    'material_type',
    'u',
    'v',
    'diffuse_transmittance_flag',
    'diffuse_reflectance_flag',
    'material_table',
    'local_coords',
    'transWeightingThreshold',
    'transWeighting',
    'reflWeighting',
    'basis',
]
struct_Material_table_struct._fields_ = [
    ('material', POINTER(c_int)),
    ('material_proportions', POINTER(c_double)),
    ('no_of_materials', c_int),
    ('imagemap', POINTER(ImageMap)),
    ('material_type', String),
    ('u', c_char),
    ('v', c_char),
    ('diffuse_transmittance_flag', c_char),
    ('diffuse_reflectance_flag', c_char),
    ('material_table', POINTER(POINTER(struct_Material_table_struct))),
    ('local_coords', ipair),
    ('transWeightingThreshold', c_double),
    ('transWeighting', c_double),
    ('reflWeighting', c_double),
    ('basis', POINTER(triplet)),
]

Material_table = struct_Material_table_struct# /Users/plewis/Documents/GitHub/librat/src/materials.h: 28

# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 24
class struct_Plane_struct(Structure):
    pass

struct_Plane_struct.__slots__ = [
    'normal',
    'dw',
    'material',
    'next_plane',
]
struct_Plane_struct._fields_ = [
    ('normal', triplet),
    ('dw', c_double),
    ('material', POINTER(Material_table)),
    ('next_plane', POINTER(struct_Plane_struct)),
]

Plane = struct_Plane_struct# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 31

# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 37
class struct_anon_30(Structure):
    pass

struct_anon_30.__slots__ = [
    'vertex_normal',
    'normal_du',
    'normal_dv',
]
struct_anon_30._fields_ = [
    ('vertex_normal', triplet),
    ('normal_du', triplet),
    ('normal_dv', triplet),
]

Vertex_normals = struct_anon_30# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 37

# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 46
class struct_anon_31(Structure):
    pass

struct_anon_31.__slots__ = [
    'vertex_local',
    'local_du',
    'local_dv',
    'basis',
    'du_length',
    'dv_length',
]
struct_anon_31._fields_ = [
    ('vertex_local', pair),
    ('local_du', pair),
    ('local_dv', pair),
    ('basis', triplet * int(2)),
    ('du_length', c_double),
    ('dv_length', c_double),
]

Vertex_locals = struct_anon_31# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 46

# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 57
class struct_anon_32(Structure):
    pass

struct_anon_32.__slots__ = [
    'orientation',
    'g',
    'dw',
    'normal',
    'material',
]
struct_anon_32._fields_ = [
    ('orientation', c_char),
    ('g', pair * int(3)),
    ('dw', c_double),
    ('normal', triplet),
    ('material', POINTER(Material_table)),
]

Facet_quick = struct_anon_32# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 57

# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 61
class struct_D_Facet_struct(Structure):
    pass

struct_D_Facet_struct.__slots__ = [
    'orientation',
    'base',
    'store',
    'dw',
    'normal',
    'vertex_normals',
    'local_coords',
    'material',
    'next_facet',
]
struct_D_Facet_struct._fields_ = [
    ('orientation', c_char),
    ('base', pair),
    ('store', c_double * int(4)),
    ('dw', c_double),
    ('normal', triplet),
    ('vertex_normals', POINTER(Vertex_normals)),
    ('local_coords', POINTER(Vertex_locals)),
    ('material', POINTER(Material_table)),
    ('next_facet', POINTER(struct_D_Facet_struct)),
]

D_Facet = struct_D_Facet_struct# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 73

# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 75
class struct_Disk_struct(Structure):
    pass

struct_Disk_struct.__slots__ = [
    'objectSize',
    'origin',
    'normal',
    'r2',
    'material',
    'next_disk',
]
struct_Disk_struct._fields_ = [
    ('objectSize', c_double),
    ('origin', triplet),
    ('normal', triplet),
    ('r2', c_double),
    ('material', POINTER(Material_table)),
    ('next_disk', POINTER(struct_Disk_struct)),
]

Disk = struct_Disk_struct# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 84

# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 88
class struct_BiLinearPatch_struct(Structure):
    pass

struct_BiLinearPatch_struct.__slots__ = [
    'objectSize',
    'corner',
    'cornerNormals',
    'normal',
    'vertex_normals',
    'local_coords',
    'material',
    'next_BiLinearPatch',
]
struct_BiLinearPatch_struct._fields_ = [
    ('objectSize', c_double),
    ('corner', triplet * int(4)),
    ('cornerNormals', triplet * int(4)),
    ('normal', triplet),
    ('vertex_normals', POINTER(Vertex_normals)),
    ('local_coords', POINTER(Vertex_locals)),
    ('material', POINTER(Material_table)),
    ('next_BiLinearPatch', POINTER(struct_BiLinearPatch_struct)),
]

BiLinearPatch = struct_BiLinearPatch_struct# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 100

# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 104
class struct_Cylinder_struct(Structure):
    pass

struct_Cylinder_struct.__slots__ = [
    'objectSize',
    'origin',
    'normal',
    'height',
    'store_h',
    'r2',
    'ends',
    'material',
    'next_cylinder',
]
struct_Cylinder_struct._fields_ = [
    ('objectSize', c_double),
    ('origin', triplet),
    ('normal', triplet),
    ('height', c_double),
    ('store_h', c_double),
    ('r2', c_double),
    ('ends', POINTER(Disk)),
    ('material', POINTER(Material_table)),
    ('next_cylinder', POINTER(struct_Cylinder_struct)),
]

Cylinder = struct_Cylinder_struct# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 117

# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 122
class struct_Ellipse_struct(Structure):
    pass

struct_Ellipse_struct.__slots__ = [
    'objectSize',
    'origin',
    'normal',
    'dimensions',
    'material',
    'next_ellipse',
]
struct_Ellipse_struct._fields_ = [
    ('objectSize', c_double),
    ('origin', triplet),
    ('normal', triplet),
    ('dimensions', triplet),
    ('material', POINTER(Material_table)),
    ('next_ellipse', POINTER(struct_Ellipse_struct)),
]

Ellipse = struct_Ellipse_struct# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 131

# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 137
class struct_Spheroid_struct(Structure):
    pass

struct_Spheroid_struct.__slots__ = [
    'objectSize',
    'origin',
    'tip',
    'radius',
    'naxes',
    'centre',
    'V1',
    'a2',
    'normal',
    'dimensions',
    'material',
    'next_spheroid',
]
struct_Spheroid_struct._fields_ = [
    ('objectSize', c_double),
    ('origin', triplet),
    ('tip', triplet),
    ('radius', c_double),
    ('naxes', triplet * int(3)),
    ('centre', triplet),
    ('V1', triplet),
    ('a2', c_double),
    ('normal', triplet),
    ('dimensions', triplet),
    ('material', POINTER(Material_table)),
    ('next_spheroid', POINTER(struct_Spheroid_struct)),
]

Spheroid = struct_Spheroid_struct# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 170

# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 183
class struct_anon_33(Structure):
    pass

struct_anon_33.__slots__ = [
    'offset',
    'extent',
    'inv_extent',
    'bbox_max',
    'bbox_min',
    'direction',
]
struct_anon_33._fields_ = [
    ('offset', triplet),
    ('extent', triplet),
    ('inv_extent', triplet),
    ('bbox_max', triplet),
    ('bbox_min', triplet),
    ('direction', String),
]

Dem_Attributes = struct_anon_33# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 183

# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 192
class struct_Sphere_struct(Structure):
    pass

struct_Sphere_struct.__slots__ = [
    'objectSize',
    'centre',
    'radius',
    'r2',
    'material',
    'next_sphere',
]
struct_Sphere_struct._fields_ = [
    ('objectSize', c_double),
    ('centre', triplet),
    ('radius', c_double),
    ('r2', c_double),
    ('material', POINTER(Material_table)),
    ('next_sphere', POINTER(struct_Sphere_struct)),
]

Sphere = struct_Sphere_struct# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 201

# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 203
class struct_D_Sphere_struct(Structure):
    pass

struct_D_Sphere_struct.__slots__ = [
    'centre',
    'radius',
    'r2',
    'material',
    'next_sphere',
]
struct_D_Sphere_struct._fields_ = [
    ('centre', triplet),
    ('radius', c_double),
    ('r2', c_double),
    ('material', POINTER(Material_table)),
    ('next_sphere', POINTER(struct_D_Sphere_struct)),
]

D_Sphere = struct_D_Sphere_struct# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 211

# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 219
class struct_anon_34(Structure):
    pass

struct_anon_34.__slots__ = [
    'spherical_cell_bounds_flag',
    'spherical_cell_bounds',
    'extent',
    'inv_extent',
    'offset',
]
struct_anon_34._fields_ = [
    ('spherical_cell_bounds_flag', c_int),
    ('spherical_cell_bounds', D_Facet * int(4)),
    ('extent', triplet),
    ('inv_extent', triplet),
    ('offset', triplet),
]

Partial_Spherical_Data = struct_anon_34# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 219

# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 225
class struct_Spherical_Dem_struct(Structure):
    pass

struct_Spherical_Dem_struct.__slots__ = [
    'objectSize',
    'data',
    'hd',
    'material',
    'base_sphere',
    'bounding_sphere',
    'partial_spherical_data',
    'next_dem',
]
struct_Spherical_Dem_struct._fields_ = [
    ('objectSize', c_double),
    ('data', Data),
    ('hd', struct_header),
    ('material', POINTER(Material_table)),
    ('base_sphere', D_Sphere),
    ('bounding_sphere', D_Sphere * int(2)),
    ('partial_spherical_data', Partial_Spherical_Data),
    ('next_dem', POINTER(struct_Spherical_Dem_struct)),
]

Spherical_Dem = struct_Spherical_Dem_struct# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 236

# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 248
class struct_anon_35(Structure):
    pass

struct_anon_35.__slots__ = [
    'origin',
    'direction',
    'ray_length',
    'sourceOfRay',
    'lengthOfRaySoFar',
    'rayLengthThroughObject',
]
struct_anon_35._fields_ = [
    ('origin', triplet),
    ('direction', triplet),
    ('ray_length', c_double),
    ('sourceOfRay', c_int),
    ('lengthOfRaySoFar', c_double),
    ('rayLengthThroughObject', c_double),
]

Ray = struct_anon_35# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 248

# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 253
class struct_Bezier4_struct(Structure):
    pass

struct_Bezier4_struct.__slots__ = [
    'objectSize',
    'coords',
    'material',
    'next_bezier4',
]
struct_Bezier4_struct._fields_ = [
    ('objectSize', c_double),
    ('coords', c_int * int(16)),
    ('material', POINTER(Material_table)),
    ('next_bezier4', POINTER(struct_Bezier4_struct)),
]

Bezier4 = struct_Bezier4_struct# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 260

# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 262
class struct_Facet_struct(Structure):
    pass

struct_Facet_struct.__slots__ = [
    'objectSize',
    'orientation',
    'base',
    'store',
    'dw',
    'size',
    'normal',
    'v',
    'vertex_normals',
    'local_coords',
    'material',
    'next_facet',
]
struct_Facet_struct._fields_ = [
    ('objectSize', c_double),
    ('orientation', c_char),
    ('base', pair),
    ('store', c_double * int(4)),
    ('dw', c_double),
    ('size', c_double),
    ('normal', triplet),
    ('v', triplet * int(3)),
    ('vertex_normals', POINTER(Vertex_normals)),
    ('local_coords', POINTER(Vertex_locals)),
    ('material', POINTER(Material_table)),
    ('next_facet', POINTER(struct_Facet_struct)),
]

fFacet = struct_Facet_struct# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 277

# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 279
class struct_Dem_struct(Structure):
    pass

struct_Dem_struct.__slots__ = [
    'objectSize',
    'dem_attributes',
    'repeatMode',
    'local',
    'facet',
    'data',
    'hd',
    'material',
    'next_dem',
]
struct_Dem_struct._fields_ = [
    ('objectSize', c_double),
    ('dem_attributes', Dem_Attributes),
    ('repeatMode', c_int),
    ('local', POINTER(triplet)),
    ('facet', fFacet),
    ('data', Data),
    ('hd', struct_header),
    ('material', POINTER(Material_table)),
    ('next_dem', POINTER(struct_Dem_struct)),
]

Dem = struct_Dem_struct# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 291

# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 339
class struct_Bbox(Structure):
    pass

# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 357
class struct_Clone(Structure):
    pass

# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 294
class struct_Content(Structure):
    pass

struct_Content.__slots__ = [
    'bbox',
    'no_of_bboxes',
    'clone',
    'no_of_clones',
    'ttriangle',
    'no_of_triangles',
    'bezier',
    'no_of_beziers',
    'sphere',
    'no_of_spheres',
    'cylinder',
    'no_of_cylinders',
    'ellipse',
    'no_of_ellipses',
    'spheroid',
    'no_of_spheroids',
    'disk',
    'no_of_disks',
    'dem',
    'no_of_dems',
    'spherical_dem',
    'no_of_spherical_dems',
    'plane',
    'no_of_planes',
    'blp',
    'no_of_blps',
]
struct_Content._fields_ = [
    ('bbox', POINTER(struct_Bbox)),
    ('no_of_bboxes', c_short),
    ('clone', POINTER(struct_Clone)),
    ('no_of_clones', c_short),
    ('ttriangle', POINTER(fFacet)),
    ('no_of_triangles', c_short),
    ('bezier', POINTER(Bezier4)),
    ('no_of_beziers', c_short),
    ('sphere', POINTER(Sphere)),
    ('no_of_spheres', c_short),
    ('cylinder', POINTER(Cylinder)),
    ('no_of_cylinders', c_short),
    ('ellipse', POINTER(Ellipse)),
    ('no_of_ellipses', c_short),
    ('spheroid', POINTER(Spheroid)),
    ('no_of_spheroids', c_short),
    ('disk', POINTER(Disk)),
    ('no_of_disks', c_short),
    ('dem', POINTER(Dem)),
    ('no_of_dems', c_short),
    ('spherical_dem', POINTER(Spherical_Dem)),
    ('no_of_spherical_dems', c_short),
    ('plane', POINTER(Plane)),
    ('no_of_planes', c_short),
    ('blp', POINTER(BiLinearPatch)),
    ('no_of_blps', c_short),
]

struct_Bbox.__slots__ = [
    'max',
    'min',
    'contents',
    'hit',
    'transform_flag',
    'define_flag',
    'm',
    'm_inverse',
    'm_inverse_transpose',
    'next_bbox',
]
struct_Bbox._fields_ = [
    ('max', triplet),
    ('min', triplet),
    ('contents', struct_Content),
    ('hit', POINTER(struct_Sort_Hit) * int(7)),
    ('transform_flag', c_char),
    ('define_flag', c_char),
    ('m', matrix4),
    ('m_inverse', matrix4),
    ('m_inverse_transpose', matrix4),
    ('next_bbox', POINTER(struct_Bbox)),
]

struct_Clone.__slots__ = [
    'bbox',
    'translation',
    'z_rotation_flag',
    'matrix',
    'next_clone',
]
struct_Clone._fields_ = [
    ('bbox', POINTER(struct_Bbox)),
    ('translation', triplet),
    ('z_rotation_flag', c_char),
    ('matrix', POINTER(c_double)),
    ('next_clone', POINTER(struct_Clone)),
]

BBox = struct_Bbox# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 406

Contents = struct_Content# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 407

Clones = struct_Clone# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 408

# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 420
class struct_anon_36(Structure):
    pass

struct_anon_36.__slots__ = [
    'group_bboxes',
    'group_names',
    'no_of_groups',
]
struct_anon_36._fields_ = [
    ('group_bboxes', POINTER(POINTER(BBox))),
    ('group_names', POINTER(POINTER(c_char))),
    ('no_of_groups', c_int),
]

Group = struct_anon_36# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 420

# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 456
class struct_anon_37(Structure):
    pass

struct_anon_37.__slots__ = [
    'RayLength',
    'intersection',
    'real_intersection',
    'normal',
    'matrix',
    'm',
    'triangleObj',
    'CylinderObj',
    'EllipseObj',
    'DiskObj',
    'BiLinearPatchObj',
    'SphereObj',
    'BezierObj',
    'PlaneObj',
    'DemObj',
    'SpheroidObj',
    'OutPut',
    'ancilliary',
    'ObjectType',
    'transform_flag',
    'clone_rotation',
    'cloneTranslation',
    'cloneTranslationFlag',
    'rayLengthThroughObject',
]
struct_anon_37._fields_ = [
    ('RayLength', c_double),
    ('intersection', triplet),
    ('real_intersection', triplet),
    ('normal', triplet),
    ('matrix', c_double * int(9)),
    ('m', matrix4),
    ('triangleObj', POINTER(fFacet)),
    ('CylinderObj', POINTER(Cylinder)),
    ('EllipseObj', POINTER(Ellipse)),
    ('DiskObj', POINTER(Disk)),
    ('BiLinearPatchObj', POINTER(BiLinearPatch)),
    ('SphereObj', POINTER(Sphere)),
    ('BezierObj', POINTER(Bezier4)),
    ('PlaneObj', POINTER(Plane)),
    ('DemObj', POINTER(Dem)),
    ('SpheroidObj', POINTER(Spheroid)),
    ('OutPut', POINTER(c_double)),
    ('ancilliary', c_double * int(4)),
    ('ObjectType', c_char),
    ('transform_flag', c_char),
    ('clone_rotation', c_char),
    ('cloneTranslation', triplet),
    ('cloneTranslationFlag', c_char),
    ('rayLengthThroughObject', c_double),
]

ObjectList = struct_anon_37# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 456

# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 465
class struct_anon_38(Structure):
    pass

struct_anon_38.__slots__ = [
    'sigma_f',
    'sigma_f_sqr',
]
struct_anon_38._fields_ = [
    ('sigma_f', c_double),
    ('sigma_f_sqr', c_double),
]

PixelCondition = struct_anon_38# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 465

# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 470
class struct_anon_39(Structure):
    pass

struct_anon_39.__slots__ = [
    'variance',
    'no_of_rays_per_pixel',
]
struct_anon_39._fields_ = [
    ('variance', c_double),
    ('no_of_rays_per_pixel', c_int),
]

PixelVarianceLimits = struct_anon_39# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 470

# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 476
class struct_BOX(Structure):
    pass

struct_BOX.__slots__ = [
    'no_of_boxes',
    'contents',
    'next_box',
]
struct_BOX._fields_ = [
    ('no_of_boxes', c_int),
    ('contents', POINTER(struct_BOX)),
    ('next_box', POINTER(struct_BOX)),
]

Box = struct_BOX# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 482

# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 511
class struct_anon_40(Structure):
    pass

struct_anon_40.__slots__ = [
    'location',
    'localNormal',
    'fromLocation',
    'fromvector',
    'tovector',
    'fromvectorLength',
    'interactionType',
    'nSuns',
    'hitSun',
    'sunvector',
    'sunInteractionType',
    'lambertian',
    'distanceBackToViewer',
    'hitSky',
    'skyvector',
    'material',
    'materialIndex',
]
struct_anon_40._fields_ = [
    ('location', triplet),
    ('localNormal', triplet),
    ('fromLocation', triplet),
    ('fromvector', triplet),
    ('tovector', triplet),
    ('fromvectorLength', c_double),
    ('interactionType', c_int),
    ('nSuns', c_int),
    ('hitSun', POINTER(c_int)),
    ('sunvector', POINTER(triplet)),
    ('sunInteractionType', POINTER(c_int)),
    ('lambertian', POINTER(c_double)),
    ('distanceBackToViewer', POINTER(c_double)),
    ('hitSky', c_int),
    ('skyvector', triplet),
    ('material', POINTER(Material_table)),
    ('materialIndex', c_int),
]

HitPoint = struct_anon_40# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 511

# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 527
class struct_anon_41(Structure):
    pass

struct_anon_41.__slots__ = [
    'direct',
    'diffuse',
    'directI',
    'diffuseI',
    'sunlit',
    'absorptance',
    'diffuseRadiance',
    'directRadiance',
    'diffuseIllumination',
    'directIllumination',
    'distance',
]
struct_anon_41._fields_ = [
    ('direct', POINTER(POINTER(POINTER(c_double)))),
    ('diffuse', POINTER(POINTER(POINTER(c_double)))),
    ('directI', POINTER(POINTER(POINTER(c_double)))),
    ('diffuseI', POINTER(POINTER(POINTER(c_double)))),
    ('sunlit', POINTER(c_double)),
    ('absorptance', POINTER(POINTER(c_double))),
    ('diffuseRadiance', POINTER(POINTER(c_double))),
    ('directRadiance', POINTER(POINTER(c_double))),
    ('diffuseIllumination', POINTER(POINTER(c_double))),
    ('directIllumination', POINTER(POINTER(c_double))),
    ('distance', c_double),
]

PStats = struct_anon_41# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 527

# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 537
for _lib in _libs.values():
    try:
        doTestFrame = (c_int).in_dll(_lib, "doTestFrame")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 538
for _lib in _libs.values():
    try:
        whichTestFrame = (c_int).in_dll(_lib, "whichTestFrame")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 540
for _lib in _libs.values():
    try:
        nBins = (c_int).in_dll(_lib, "nBins")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 540
for _lib in _libs.values():
    try:
        nBands = (c_int).in_dll(_lib, "nBands")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 540
for _lib in _libs.values():
    try:
        nRPP = (c_int).in_dll(_lib, "nRPP")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 541
for _lib in _libs.values():
    try:
        argc = (c_int).in_dll(_lib, "argc")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 542
for _lib in _libs.values():
    try:
        argv = (POINTER(POINTER(c_char))).in_dll(_lib, "argv")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/sensor_rsr.h: 9
class struct_anon_42(Structure):
    pass

struct_anon_42.__slots__ = [
    'rsr',
    'no_of_samples',
]
struct_anon_42._fields_ = [
    ('rsr', (c_double * int(2)) * int(2)),
    ('no_of_samples', c_int),
]

Spectral_Data = struct_anon_42# /Users/plewis/Documents/GitHub/librat/src/sensor_rsr.h: 9

# /Users/plewis/Documents/GitHub/librat/src/sensor_rsr.h: 15
class struct_anon_43(Structure):
    pass

struct_anon_43.__slots__ = [
    'sensor_rsr',
    'no_of_wavebands',
]
struct_anon_43._fields_ = [
    ('sensor_rsr', POINTER(Spectral_Data)),
    ('no_of_wavebands', c_int),
]

Sensor_Wavebands = struct_anon_43# /Users/plewis/Documents/GitHub/librat/src/sensor_rsr.h: 15

# /Users/plewis/Documents/GitHub/librat/src/sensor_rsr.h: 18
if _libs["libratlib.so"].has("dummy_read_spectral_file", "cdecl"):
    dummy_read_spectral_file = _libs["libratlib.so"].get("dummy_read_spectral_file", "cdecl")
    dummy_read_spectral_file.argtypes = [POINTER(Sensor_Wavebands)]
    dummy_read_spectral_file.restype = None

# /Users/plewis/Documents/GitHub/librat/src/sensor_rsr.h: 19
if _libs["libratlib.so"].has("read_spectral_file", "cdecl"):
    read_spectral_file = _libs["libratlib.so"].get("read_spectral_file", "cdecl")
    read_spectral_file.argtypes = [c_int, POINTER(POINTER(c_char)), c_int, POINTER(c_int), POINTER(Sensor_Wavebands), String]
    read_spectral_file.restype = None

# /Users/plewis/Documents/GitHub/librat/src/sensor_rsr.h: 20
if _libs["libratlib.so"].has("quantise_sky_waveband", "cdecl"):
    quantise_sky_waveband = _libs["libratlib.so"].get("quantise_sky_waveband", "cdecl")
    quantise_sky_waveband.argtypes = [POINTER(struct_header), c_double, c_int, c_double, c_double]
    quantise_sky_waveband.restype = c_int

# /Users/plewis/Documents/GitHub/librat/src/camera.h: 29
if _libs["libratlib.so"].has("read_data_from_line", "cdecl"):
    read_data_from_line = _libs["libratlib.so"].get("read_data_from_line", "cdecl")
    read_data_from_line.argtypes = [String, String, c_int, POINTER(c_double), POINTER(c_int)]
    if sizeof(c_int) == sizeof(c_void_p):
        read_data_from_line.restype = ReturnString
    else:
        read_data_from_line.restype = String
        read_data_from_line.errcheck = ReturnString

# /Users/plewis/Documents/GitHub/librat/src/camera.h: 32
if _libs["libratlib.so"].has("calculate_sun_elevation_axis", "cdecl"):
    calculate_sun_elevation_axis = _libs["libratlib.so"].get("calculate_sun_elevation_axis", "cdecl")
    calculate_sun_elevation_axis.argtypes = [POINTER(triplet), POINTER(triplet)]
    calculate_sun_elevation_axis.restype = None

# /Users/plewis/Documents/GitHub/librat/src/camera.h: 33
if _libs["libratlib.so"].has("calculate_pixel_variance", "cdecl"):
    calculate_pixel_variance = _libs["libratlib.so"].get("calculate_pixel_variance", "cdecl")
    calculate_pixel_variance.argtypes = [c_int, POINTER(c_double), POINTER(PixelCondition)]
    calculate_pixel_variance.restype = None

# /Users/plewis/Documents/GitHub/librat/src/camera.h: 37
if _libs["libratlib.so"].has("clearIntArray", "cdecl"):
    clearIntArray = _libs["libratlib.so"].get("clearIntArray", "cdecl")
    clearIntArray.argtypes = [POINTER(c_int), c_int]
    clearIntArray.restype = None

# /Users/plewis/Documents/GitHub/librat/src/camera.h: 38
if _libs["libratlib.so"].has("clearDoubleArray", "cdecl"):
    clearDoubleArray = _libs["libratlib.so"].get("clearDoubleArray", "cdecl")
    clearDoubleArray.argtypes = [POINTER(c_double), c_int]
    clearDoubleArray.restype = None

# /Users/plewis/Documents/GitHub/librat/src/camera.h: 39
if _libs["libratlib.so"].has("clearInt_Array", "cdecl"):
    clearInt_Array = _libs["libratlib.so"].get("clearInt_Array", "cdecl")
    clearInt_Array.argtypes = [POINTER(POINTER(c_int)), c_int, c_int]
    clearInt_Array.restype = None

# /Users/plewis/Documents/GitHub/librat/src/camera.h: 40
if _libs["libratlib.so"].has("clearDouble_Array", "cdecl"):
    clearDouble_Array = _libs["libratlib.so"].get("clearDouble_Array", "cdecl")
    clearDouble_Array.argtypes = [POINTER(POINTER(c_double)), c_int, c_int]
    clearDouble_Array.restype = None

# /Users/plewis/Documents/GitHub/librat/src/camera.h: 41
if _libs["libratlib.so"].has("clearInt__Array", "cdecl"):
    clearInt__Array = _libs["libratlib.so"].get("clearInt__Array", "cdecl")
    clearInt__Array.argtypes = [POINTER(POINTER(POINTER(c_int))), c_int, c_int, c_int]
    clearInt__Array.restype = None

# /Users/plewis/Documents/GitHub/librat/src/camera.h: 42
if _libs["libratlib.so"].has("clearDouble__Array", "cdecl"):
    clearDouble__Array = _libs["libratlib.so"].get("clearDouble__Array", "cdecl")
    clearDouble__Array.argtypes = [POINTER(POINTER(POINTER(c_double))), c_int, c_int, c_int]
    clearDouble__Array.restype = None

# /Users/plewis/Documents/GitHub/librat/src/camera.h: 49
if _libs["libratlib.so"].has("clearvector", "cdecl"):
    clearvector = _libs["libratlib.so"].get("clearvector", "cdecl")
    clearvector.argtypes = [POINTER(triplet)]
    clearvector.restype = None

# /Users/plewis/Documents/GitHub/librat/src/camera.h: 50
if _libs["libratlib.so"].has("clearAHitPoint", "cdecl"):
    clearAHitPoint = _libs["libratlib.so"].get("clearAHitPoint", "cdecl")
    clearAHitPoint.argtypes = [POINTER(HitPoint)]
    clearAHitPoint.restype = None

# /Users/plewis/Documents/GitHub/librat/src/camera.h: 51
if _libs["libratlib.so"].has("clearHitPoint", "cdecl"):
    clearHitPoint = _libs["libratlib.so"].get("clearHitPoint", "cdecl")
    clearHitPoint.argtypes = [c_int, POINTER(HitPoint)]
    clearHitPoint.restype = None

# /Users/plewis/Documents/GitHub/librat/src/camera.h: 52
if _libs["libratlib.so"].has("gaussianFunction", "cdecl"):
    gaussianFunction = _libs["libratlib.so"].get("gaussianFunction", "cdecl")
    gaussianFunction.argtypes = [c_double]
    gaussianFunction.restype = c_double

# /Users/plewis/Documents/GitHub/librat/src/reflectance.h: 8
if _libs["libratlib.so"].has("rotateBymatrix", "cdecl"):
    rotateBymatrix = _libs["libratlib.so"].get("rotateBymatrix", "cdecl")
    rotateBymatrix.argtypes = [triplet, POINTER(c_double), c_int]
    rotateBymatrix.restype = triplet

# /Users/plewis/Documents/GitHub/librat/src/reflectance.h: 9
if _libs["libratlib.so"].has("calculate_diffuse_sample_ray_direction", "cdecl"):
    calculate_diffuse_sample_ray_direction = _libs["libratlib.so"].get("calculate_diffuse_sample_ray_direction", "cdecl")
    calculate_diffuse_sample_ray_direction.argtypes = [POINTER(triplet)]
    calculate_diffuse_sample_ray_direction.restype = triplet

# /Users/plewis/Documents/GitHub/librat/src/reflectance.h: 10
if _libs["libratlib.so"].has("setReflectanceValue", "cdecl"):
    setReflectanceValue = _libs["libratlib.so"].get("setReflectanceValue", "cdecl")
    setReflectanceValue.argtypes = [POINTER(c_int), c_int, POINTER(c_double), POINTER(c_double), POINTER(c_double), c_double, c_int]
    setReflectanceValue.restype = None

# /Users/plewis/Documents/GitHub/librat/src/reflectance.h: 16
if _libs["libratlib.so"].has("apply_bump_map", "cdecl"):
    apply_bump_map = _libs["libratlib.so"].get("apply_bump_map", "cdecl")
    apply_bump_map.argtypes = [POINTER(Material_table), POINTER(triplet), POINTER(c_double)]
    apply_bump_map.restype = c_int

# /Users/plewis/Documents/GitHub/librat/src/reflectance.h: 17
if _libs["libratlib.so"].has("apply_material_map", "cdecl"):
    apply_material_map = _libs["libratlib.so"].get("apply_material_map", "cdecl")
    apply_material_map.argtypes = [POINTER(Material_table), c_int]
    apply_material_map.restype = c_int

# /Users/plewis/Documents/GitHub/librat/src/reflectance.h: 18
if _libs["libratlib.so"].has("calculate_local_coords", "cdecl"):
    calculate_local_coords = _libs["libratlib.so"].get("calculate_local_coords", "cdecl")
    calculate_local_coords.argtypes = [POINTER(Material_table), POINTER(ObjectList), c_int, POINTER(c_double), c_int]
    calculate_local_coords.restype = None

# /Users/plewis/Documents/GitHub/librat/src/volume.h: 23
class struct_anon_44(Structure):
    pass

struct_anon_44.__slots__ = [
    'uL',
    'l',
    'lad',
    'material',
    's',
    'm',
    'G',
    'Glut',
    'nSamples',
    'VOLUME_nZeniths',
    'VOLUME_nZenithsFreq',
]
struct_anon_44._fields_ = [
    ('uL', c_double),
    ('l', c_double),
    ('lad', CFUNCTYPE(UNCHECKED(triplet), POINTER(None))),
    ('material', c_int),
    ('s', triplet),
    ('m', c_double),
    ('G', c_double),
    ('Glut', POINTER(c_double)),
    ('nSamples', c_int),
    ('VOLUME_nZeniths', c_int),
    ('VOLUME_nZenithsFreq', POINTER(c_float)),
]

Volume = struct_anon_44# /Users/plewis/Documents/GitHub/librat/src/volume.h: 23

# /Users/plewis/Documents/GitHub/librat/src/mtllib.h: 11
if _libs["libratlib.so"].has("getNdefaultMaterials", "cdecl"):
    getNdefaultMaterials = _libs["libratlib.so"].get("getNdefaultMaterials", "cdecl")
    getNdefaultMaterials.argtypes = []
    getNdefaultMaterials.restype = c_int

# /Users/plewis/Documents/GitHub/librat/src/mtllib.h: 14
if _libs["libratlib.so"].has("preprocess_bumpmap_data", "cdecl"):
    preprocess_bumpmap_data = _libs["libratlib.so"].get("preprocess_bumpmap_data", "cdecl")
    preprocess_bumpmap_data.argtypes = [POINTER(Material_table)]
    preprocess_bumpmap_data.restype = None

# /Users/plewis/Documents/GitHub/librat/src/mtllib.h: 15
if _libs["libratlib.so"].has("find_number_of_materials_in_material_map", "cdecl"):
    find_number_of_materials_in_material_map = _libs["libratlib.so"].get("find_number_of_materials_in_material_map", "cdecl")
    find_number_of_materials_in_material_map.argtypes = [POINTER(ImageMap)]
    find_number_of_materials_in_material_map.restype = c_int

# /Users/plewis/Documents/GitHub/librat/src/mtllib.h: 17
if _libs["libratlib.so"].has("read_material_map_file", "cdecl"):
    read_material_map_file = _libs["libratlib.so"].get("read_material_map_file", "cdecl")
    read_material_map_file.argtypes = [POINTER(Material_table), String, POINTER(c_int), POINTER(POINTER(c_char))]
    read_material_map_file.restype = None

# /Users/plewis/Documents/GitHub/librat/src/mtllib.h: 20
if _libs["libratlib.so"].has("is_double", "cdecl"):
    is_double = _libs["libratlib.so"].get("is_double", "cdecl")
    is_double.argtypes = [String, POINTER(c_double)]
    is_double.restype = c_int

# /Users/plewis/Documents/GitHub/librat/src/mtllib.h: 21
if _libs["libratlib.so"].has("sortRPVData", "cdecl"):
    sortRPVData = _libs["libratlib.so"].get("sortRPVData", "cdecl")
    sortRPVData.argtypes = [String, String, POINTER(c_int), c_int, POINTER(POINTER(c_char))]
    sortRPVData.restype = POINTER(c_double)

# /Users/plewis/Documents/GitHub/librat/src/mtllib.h: 25
if _libs["libratlib.so"].has("spherical", "cdecl"):
    spherical = _libs["libratlib.so"].get("spherical", "cdecl")
    spherical.argtypes = [POINTER(Volume)]
    spherical.restype = triplet

# /Users/plewis/Documents/GitHub/librat/src/mtllib.h: 26
if _libs["libratlib.so"].has("elliptical", "cdecl"):
    elliptical = _libs["libratlib.so"].get("elliptical", "cdecl")
    elliptical.argtypes = [POINTER(Volume)]
    elliptical.restype = triplet

# /Users/plewis/Documents/GitHub/librat/src/mtllib.h: 27
if _libs["libratlib.so"].has("readFileEqual", "cdecl"):
    readFileEqual = _libs["libratlib.so"].get("readFileEqual", "cdecl")
    readFileEqual.argtypes = [POINTER(Volume)]
    readFileEqual.restype = triplet

# /Users/plewis/Documents/GitHub/librat/src/reflectance_functions.h: 23
class struct_anon_45(Structure):
    pass

struct_anon_45.__slots__ = [
    'result',
    'direct_depth_result',
    'diffuse_depth_result',
    'max_no_of_ray_samples',
    'no_of_time_bins',
    'startBinCount',
    'endBinCount',
    'binStep',
    'direct_start',
    'diffuse_reflectance_start',
    'diffuse_transmittance_start',
    'wavelength',
    'no_of_diffuse_reflectance_samples',
    'no_of_diffuse_transmittance_samples',
    'max_diffuse_sampling_depth',
    'no_of_shadow_rays',
    'sample_no',
    'nBands',
]
struct_anon_45._fields_ = [
    ('result', POINTER(c_double)),
    ('direct_depth_result', POINTER(c_double)),
    ('diffuse_depth_result', POINTER(c_double)),
    ('max_no_of_ray_samples', c_int),
    ('no_of_time_bins', c_int),
    ('startBinCount', c_double),
    ('endBinCount', c_double),
    ('binStep', c_double),
    ('direct_start', c_int),
    ('diffuse_reflectance_start', c_int),
    ('diffuse_transmittance_start', c_int),
    ('wavelength', POINTER(c_int)),
    ('no_of_diffuse_reflectance_samples', c_int),
    ('no_of_diffuse_transmittance_samples', c_int),
    ('max_diffuse_sampling_depth', c_int),
    ('no_of_shadow_rays', c_int),
    ('sample_no', c_int),
    ('nBands', c_int),
]

Samples = struct_anon_45# /Users/plewis/Documents/GitHub/librat/src/reflectance_functions.h: 23

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 27
class struct_anon_46(Structure):
    pass

struct_anon_46.__slots__ = [
    'zoom',
    'aperture',
    'focal_length',
    'format',
    'roll_angle',
    'camera_position',
    'look_at',
    'frame',
    'camera_type',
    'rowextent',
    'colextent',
    'col_fwhm',
    'row_fwhm',
]
struct_anon_46._fields_ = [
    ('zoom', c_double),
    ('aperture', c_double),
    ('focal_length', c_double),
    ('format', pair),
    ('roll_angle', c_double),
    ('camera_position', triplet),
    ('look_at', triplet),
    ('frame', c_int),
    ('camera_type', c_int),
    ('rowextent', c_double),
    ('colextent', c_double),
    ('col_fwhm', c_double),
    ('row_fwhm', c_double),
]

Camera_ip = struct_anon_46# /Users/plewis/Documents/GitHub/librat/src/bags.h: 27

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 52
class struct_anon_47(Structure):
    pass

struct_anon_47.__slots__ = [
    'u',
    'v',
    'format',
    'focal_length',
    'ppoint',
    'paxis',
    'aperture',
    'focal_point',
    'pixel_fov_factor',
    'magnification_factor',
    'watch_point',
    'U',
    'V',
    'aperture_fov_factor',
    'sigma_row',
    'sigma_col',
    'camera_type',
    'spherical_reduction',
]
struct_anon_47._fields_ = [
    ('u', triplet),
    ('v', triplet),
    ('format', pair),
    ('focal_length', c_double),
    ('ppoint', triplet),
    ('paxis', triplet),
    ('aperture', c_double),
    ('focal_point', triplet),
    ('pixel_fov_factor', c_double),
    ('magnification_factor', c_double),
    ('watch_point', triplet),
    ('U', c_double),
    ('V', c_double),
    ('aperture_fov_factor', c_double),
    ('sigma_row', c_double),
    ('sigma_col', c_double),
    ('camera_type', c_int),
    ('spherical_reduction', c_double),
]

Camera_op = struct_anon_47# /Users/plewis/Documents/GitHub/librat/src/bags.h: 52

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 61
class struct_anon_48(Structure):
    pass

struct_anon_48.__slots__ = [
    'restart',
    'from',
    'to',
    'from_sample',
    'to_sample',
    'recover',
]
struct_anon_48._fields_ = [
    ('restart', c_int),
    ('from', c_int),
    ('to', c_int),
    ('from_sample', c_int),
    ('to_sample', c_int),
    ('recover', c_int),
]

Restart = struct_anon_48# /Users/plewis/Documents/GitHub/librat/src/bags.h: 61

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 66
for _lib in _libs.values():
    try:
        sun_magnitude = (POINTER(c_double)).in_dll(_lib, "sun_magnitude")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 67
for _lib in _libs.values():
    try:
        sky_black = (c_int).in_dll(_lib, "sky_black")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 68
for _lib in _libs.values():
    try:
        sun_Ptr = (POINTER(triplet)).in_dll(_lib, "sun_Ptr")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 69
for _lib in _libs.values():
    try:
        sky_flag = (c_int).in_dll(_lib, "sky_flag")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 70
for _lib in _libs.values():
    try:
        sun_fov = (c_double).in_dll(_lib, "sun_fov")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 71
for _lib in _libs.values():
    try:
        theta_sun = (c_double).in_dll(_lib, "theta_sun")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 72
for _lib in _libs.values():
    try:
        phi_gap = (c_double).in_dll(_lib, "phi_gap")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 73
for _lib in _libs.values():
    try:
        sun_elevation_axis = (triplet).in_dll(_lib, "sun_elevation_axis")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 74
for _lib in _libs.values():
    try:
        direct_file = (String).in_dll(_lib, "direct_file")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 75
for _lib in _libs.values():
    try:
        position = (POINTER(Camera_op)).in_dll(_lib, "position")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 81
for _lib in _libs.values():
    try:
        name = (String).in_dll(_lib, "name")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 82
for _lib in _libs.values():
    try:
        camera_ip = (Camera_ip).in_dll(_lib, "camera_ip")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 83
for _lib in _libs.values():
    try:
        camera_op = (Camera_op).in_dll(_lib, "camera_op")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 120
for _lib in _libs.values():
    try:
        vertexStore = (c_int).in_dll(_lib, "vertexStore")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 121
for _lib in _libs.values():
    try:
        angleTol = (c_double).in_dll(_lib, "angleTol")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 121
for _lib in _libs.values():
    try:
        distanceTol = (c_double).in_dll(_lib, "distanceTol")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 121
for _lib in _libs.values():
    try:
        sizeTol = (c_double).in_dll(_lib, "sizeTol")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 123
for _lib in _libs.values():
    try:
        distanceStore = (POINTER(c_double)).in_dll(_lib, "distanceStore")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 124
for _lib in _libs.values():
    try:
        reflectanceStore = (POINTER(c_double)).in_dll(_lib, "reflectanceStore")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 125
for _lib in _libs.values():
    try:
        doInfinite = (c_int).in_dll(_lib, "doInfinite")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 126
for _lib in _libs.values():
    try:
        infiniteOffset = (triplet).in_dll(_lib, "infiniteOffset")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 127
for _lib in _libs.values():
    try:
        blackSky = (c_int).in_dll(_lib, "blackSky")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 128
for _lib in _libs.values():
    try:
        rtRatio = (c_double).in_dll(_lib, "rtRatio")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 129
for _lib in _libs.values():
    try:
        reverse = (c_int).in_dll(_lib, "reverse")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 130
for _lib in _libs.values():
    try:
        photonSpread = (c_int).in_dll(_lib, "photonSpread")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 131
for _lib in _libs.values():
    try:
        lidar = (c_int).in_dll(_lib, "lidar")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 132
for _lib in _libs.values():
    try:
        sz = (c_double).in_dll(_lib, "sz")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 133
for _lib in _libs.values():
    try:
        sunlocation = (POINTER(c_double)).in_dll(_lib, "sunlocation")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 134
for _lib in _libs.values():
    try:
        sunrayangle = (POINTER(c_double)).in_dll(_lib, "sunrayangle")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 135
for _lib in _libs.values():
    try:
        nSuns = (c_int).in_dll(_lib, "nSuns")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 136
for _lib in _libs.values():
    try:
        illuminationvector = (POINTER(triplet)).in_dll(_lib, "illuminationvector")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 137
for _lib in _libs.values():
    try:
        lengthToSun = (POINTER(c_double)).in_dll(_lib, "lengthToSun")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 138
for _lib in _libs.values():
    try:
        isSunTest = (c_int).in_dll(_lib, "isSunTest")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 139
for _lib in _libs.values():
    try:
        rowsandcols = (POINTER(c_int)).in_dll(_lib, "rowsandcols")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 153
class struct_anon_49(Structure):
    pass

struct_anon_49.__slots__ = [
    'lambda_min_Ptr',
    'lambda_width_Ptr',
    'sensor_wavebands',
    'theta_gap',
    'rsr_filename',
    'rsr_flag',
    'op_image_file',
    'sensor_filenames',
    'wavefront_file',
    'lambda',
]
struct_anon_49._fields_ = [
    ('lambda_min_Ptr', POINTER(c_double)),
    ('lambda_width_Ptr', POINTER(c_double)),
    ('sensor_wavebands', POINTER(Sensor_Wavebands)),
    ('theta_gap', c_double),
    ('rsr_filename', POINTER(POINTER(c_char))),
    ('rsr_flag', c_int),
    ('op_image_file', String),
    ('sensor_filenames', POINTER(POINTER(c_char))),
    ('wavefront_file', String),
    ('lambda', POINTER(c_double)),
]

WavebandBag = struct_anon_49# /Users/plewis/Documents/GitHub/librat/src/bags.h: 153

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 163
class struct_anon_50(Structure):
    pass

struct_anon_50.__slots__ = [
    'diffuse_reflectance',
    'diffuse_transmittance',
    'proportion',
    'diffuse_reflectance_flag',
    'diffuse_transmittance_flag',
]
struct_anon_50._fields_ = [
    ('diffuse_reflectance', POINTER(c_double)),
    ('diffuse_transmittance', POINTER(c_double)),
    ('proportion', c_double),
    ('diffuse_reflectance_flag', c_int),
    ('diffuse_transmittance_flag', c_int),
]

Standard_Reflectance_Model = struct_anon_50# /Users/plewis/Documents/GitHub/librat/src/bags.h: 163

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 184
class struct_anon_51(Structure):
    pass

struct_anon_51.__slots__ = [
    'no_of_samples',
    'wavelength',
    'srm',
    'volume',
]
struct_anon_51._fields_ = [
    ('no_of_samples', c_int),
    ('wavelength', POINTER(c_double)),
    ('srm', Standard_Reflectance_Model),
    ('volume', Volume),
]

Standard_Material_List = struct_anon_51# /Users/plewis/Documents/GitHub/librat/src/bags.h: 184

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 198
for _lib in _libs.values():
    try:
        samples = (POINTER(Samples)).in_dll(_lib, "samples")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 199
for _lib in _libs.values():
    try:
        material_lut = (POINTER(Standard_Material_List)).in_dll(_lib, "material_lut")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 200
for _lib in _libs.values():
    try:
        material_names = (POINTER(POINTER(c_char))).in_dll(_lib, "material_names")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 226
class struct_anon_52(Structure):
    pass

struct_anon_52.__slots__ = [
    'index',
    'name',
    'used',
    'type',
    'typeName',
    'nWavelengths',
    'wavelengths',
    'diffuse_reflectance',
    'diffuse_transmittance',
]
struct_anon_52._fields_ = [
    ('index', c_int),
    ('name', String),
    ('used', c_int),
    ('type', c_int),
    ('typeName', String),
    ('nWavelengths', c_int),
    ('wavelengths', POINTER(c_double)),
    ('diffuse_reflectance', POINTER(c_double)),
    ('diffuse_transmittance', POINTER(c_double)),
]

RATmaterials = struct_anon_52# /Users/plewis/Documents/GitHub/librat/src/bags.h: 226

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 242
class struct_anon_53(Structure):
    pass

struct_anon_53.__slots__ = [
    'n',
    'thisRTD',
    'intersectionPoints',
    'rayLengths',
    'ratmat',
    'diffusePathIntersectType',
    'directPathIntersectType',
    'hitSun',
    'hitSky',
    'diffuseRadiance',
    'directRadiance',
    'lengthToSun',
    'angleToSun',
]
struct_anon_53._fields_ = [
    ('n', c_int),
    ('thisRTD', c_int),
    ('intersectionPoints', POINTER(POINTER(c_double))),
    ('rayLengths', POINTER(c_double)),
    ('ratmat', POINTER(c_int)),
    ('diffusePathIntersectType', POINTER(c_int)),
    ('directPathIntersectType', POINTER(c_int)),
    ('hitSun', POINTER(c_int)),
    ('hitSky', POINTER(c_int)),
    ('diffuseRadiance', POINTER(c_double)),
    ('directRadiance', POINTER(c_double)),
    ('lengthToSun', POINTER(c_double)),
    ('angleToSun', POINTER(c_double)),
]

RATtree = struct_anon_53# /Users/plewis/Documents/GitHub/librat/src/bags.h: 242

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 249
class struct_anon_54(Structure):
    pass

struct_anon_54.__slots__ = [
    'nothing',
]
struct_anon_54._fields_ = [
    ('nothing', c_int),
]

RATvol = struct_anon_54# /Users/plewis/Documents/GitHub/librat/src/bags.h: 249

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 258
for _lib in _libs.values():
    try:
        name = (String).in_dll(_lib, "name")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 280
for _lib in _libs.values():
    try:
        nd = (POINTER(c_float)).in_dll(_lib, "nd")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 281
for _lib in _libs.values():
    try:
        nsamps = (c_int * int(3)).in_dll(_lib, "nsamps")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 283
for _lib in _libs.values():
    try:
        otherStuff = (POINTER(None)).in_dll(_lib, "otherStuff")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 284
for _lib in _libs.values():
    try:
        rawfilename = (String).in_dll(_lib, "rawfilename")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 285
for _lib in _libs.values():
    try:
        datafilename = (String).in_dll(_lib, "datafilename")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 294
for _lib in _libs.values():
    try:
        wavebandbag = (POINTER(WavebandBag)).in_dll(_lib, "wavebandbag")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 296
for _lib in _libs.values():
    try:
        material_name = (POINTER(POINTER(c_char))).in_dll(_lib, "material_name")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 297
for _lib in _libs.values():
    try:
        material_table = (POINTER(Material_table)).in_dll(_lib, "material_table")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 298
for _lib in _libs.values():
    try:
        PRAT_MAX_MATERIALS = (c_int).in_dll(_lib, "PRAT_MAX_MATERIALS")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 299
for _lib in _libs.values():
    try:
        current_mtl = (POINTER(Material_table)).in_dll(_lib, "current_mtl")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 300
for _lib in _libs.values():
    try:
        m_inv_reverse = (matrix4).in_dll(_lib, "m_inv_reverse")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 300
for _lib in _libs.values():
    try:
        m_inverse_fwd = (matrix4).in_dll(_lib, "m_inverse_fwd")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 301
for _lib in _libs.values():
    try:
        max_number_of_groups = (c_int).in_dll(_lib, "max_number_of_groups")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 303
for _lib in _libs.values():
    try:
        samples = (Samples).in_dll(_lib, "samples")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 304
for _lib in _libs.values():
    try:
        group = (Group).in_dll(_lib, "group")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 305
for _lib in _libs.values():
    try:
        sensor_wavebands = (Sensor_Wavebands).in_dll(_lib, "sensor_wavebands")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 306
for _lib in _libs.values():
    try:
        timer = (c_int).in_dll(_lib, "timer")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 307
for _lib in _libs.values():
    try:
        level = (c_int).in_dll(_lib, "level")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 308
for _lib in _libs.values():
    try:
        vertices = (VeRtIcEs).in_dll(_lib, "vertices")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 308
for _lib in _libs.values():
    try:
        normals = (VeRtIcEs).in_dll(_lib, "normals")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 308
for _lib in _libs.values():
    try:
        locals = (VeRtIcEs).in_dll(_lib, "locals")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 309
try:
    bbox = (POINTER(BBox)).in_dll(_libs["libratlib.so"], "bbox")
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 310
for _lib in _libs.values():
    try:
        mmap_flag = (c_int).in_dll(_lib, "mmap_flag")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 312
for _lib in _libs.values():
    try:
        lidar = (c_int).in_dll(_lib, "lidar")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 314
for _lib in _libs.values():
    try:
        globalArgc = (c_int).in_dll(_lib, "globalArgc")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 315
for _lib in _libs.values():
    try:
        globalArgv = (POINTER(POINTER(c_char))).in_dll(_lib, "globalArgv")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 319
for _lib in _libs.values():
    try:
        globalnBands = (c_int).in_dll(_lib, "globalnBands")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 320
for _lib in _libs.values():
    try:
        globalOver = (c_double).in_dll(_lib, "globalOver")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 321
for _lib in _libs.values():
    try:
        globalDoLidar = (c_int).in_dll(_lib, "globalDoLidar")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 322
for _lib in _libs.values():
    try:
        globalnRTD = (c_int).in_dll(_lib, "globalnRTD")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 324
for _lib in _libs.values():
    try:
        facets = (POINTER(fFacet)).in_dll(_lib, "facets")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 325
for _lib in _libs.values():
    try:
        nFacets = (c_int).in_dll(_lib, "nFacets")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 325
for _lib in _libs.values():
    try:
        whichFacet = (c_int).in_dll(_lib, "whichFacet")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 325
for _lib in _libs.values():
    try:
        nFacetBlock = (c_int).in_dll(_lib, "nFacetBlock")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 326
for _lib in _libs.values():
    try:
        ratTree = (POINTER(RATtree)).in_dll(_lib, "ratTree")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 327
for _lib in _libs.values():
    try:
        nSuns = (c_int).in_dll(_lib, "nSuns")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 330
for _lib in _libs.values():
    try:
        ratMat = (POINTER(RATmaterials)).in_dll(_lib, "ratMat")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 331
for _lib in _libs.values():
    try:
        nMaterials = (POINTER(c_int)).in_dll(_lib, "nMaterials")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 332
for _lib in _libs.values():
    try:
        ratMatUsed = (POINTER(RATmaterials)).in_dll(_lib, "ratMatUsed")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 333
for _lib in _libs.values():
    try:
        nMaterialsUsed = (POINTER(c_int)).in_dll(_lib, "nMaterialsUsed")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 334
for _lib in _libs.values():
    try:
        materialTranslatorLut = (POINTER(c_int)).in_dll(_lib, "materialTranslatorLut")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 337
for _lib in _libs.values():
    try:
        nObjectTypes = (c_int).in_dll(_lib, "nObjectTypes")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 338
for _lib in _libs.values():
    try:
        objectTypes = (POINTER(c_int)).in_dll(_lib, "objectTypes")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 339
for _lib in _libs.values():
    try:
        objectTypeCapacity = (c_int).in_dll(_lib, "objectTypeCapacity")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 340
for _lib in _libs.values():
    try:
        objectMaterialNames = (POINTER(POINTER(c_char))).in_dll(_lib, "objectMaterialNames")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 341
for _lib in _libs.values():
    try:
        minObjectSize = (POINTER(c_double)).in_dll(_lib, "minObjectSize")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 342
for _lib in _libs.values():
    try:
        maxObjectSize = (POINTER(c_double)).in_dll(_lib, "maxObjectSize")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 345
for _lib in _libs.values():
    try:
        ratvol = (POINTER(RATvol)).in_dll(_lib, "ratvol")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 349
for _lib in _libs.values():
    try:
        nratObjs = (c_int).in_dll(_lib, "nratObjs")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 350
for _lib in _libs.values():
    try:
        hit_camera = (c_int).in_dll(_lib, "hit_camera")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 352
for _lib in _libs.values():
    try:
        nstore = (c_int).in_dll(_lib, "nstore")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 353
for _lib in _libs.values():
    try:
        store = (POINTER(POINTER(None))).in_dll(_lib, "store")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 355
for _lib in _libs.values():
    try:
        matrix = (c_double * int(16)).in_dll(_lib, "matrix")
        break
    except:
        pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 365
class struct_anon_55(Structure):
    pass

struct_anon_55.__slots__ = [
    'lambertian',
    'timer',
    'reflectance_type',
    'no_of_diffuse_reflectance_samples',
    'no_of_diffuse_transmittance_samples',
    'look',
    'normal',
    'illumination',
    'plane_normal',
]
struct_anon_55._fields_ = [
    ('lambertian', c_double),
    ('timer', c_int),
    ('reflectance_type', c_int),
    ('no_of_diffuse_reflectance_samples', c_int),
    ('no_of_diffuse_transmittance_samples', c_int),
    ('look', POINTER(triplet)),
    ('normal', POINTER(triplet)),
    ('illumination', POINTER(triplet)),
    ('plane_normal', POINTER(triplet)),
]

CurrentReflectanceBag = struct_anon_55# /Users/plewis/Documents/GitHub/librat/src/bags.h: 365

# ./rat.h: 5
if _libs["libratlib.so"].has("sphereTesselate", "cdecl"):
    sphereTesselate = _libs["libratlib.so"].get("sphereTesselate", "cdecl")
    sphereTesselate.argtypes = [c_int, POINTER(c_int)]
    sphereTesselate.restype = POINTER(c_double)

# ./rat.h: 6
if _libs["libratlib.so"].has("facetNormal", "cdecl"):
    facetNormal = _libs["libratlib.so"].get("facetNormal", "cdecl")
    facetNormal.argtypes = [POINTER(c_double), POINTER(struct_Facet_struct)]
    facetNormal.restype = c_double

# ./rat.h: 17
for _lib in _libs.values():
    if not _lib.has("RATsignalInterrupt", "cdecl"):
        continue
    RATsignalInterrupt = _lib.get("RATsignalInterrupt", "cdecl")
    RATsignalInterrupt.argtypes = [c_int]
    RATsignalInterrupt.restype = None
    break

# ./ratFront.h: 39
class struct_RATorderStore(Structure):
    pass

struct_RATorderStore.__slots__ = [
    'r',
    'nelements',
    'code',
    'index',
]
struct_RATorderStore._fields_ = [
    ('r', POINTER(struct_RATorderStore)),
    ('nelements', c_int),
    ('code', POINTER(c_int)),
    ('index', c_int),
]

# ./ratFront.h: 54
for _lib in _libs.values():
    try:
        m = (c_int).in_dll(_lib, "m")
        break
    except:
        pass

# ./ratFront.h: 54
for _lib in _libs.values():
    try:
        n = (c_int).in_dll(_lib, "n")
        break
    except:
        pass

# ./ratFront.h: 54
for _lib in _libs.values():
    try:
        nbands = (c_int).in_dll(_lib, "nbands")
        break
    except:
        pass

# ./ratFront.h: 55
for _lib in _libs.values():
    try:
        ratMat = (POINTER(RATmaterials)).in_dll(_lib, "ratMat")
        break
    except:
        pass

# ./ratFront.h: 56
try:
    counter = (POINTER(c_int)).in_dll(_libs["libratlib.so"], "counter")
except:
    pass

# ./ratFront.h: 57
for _lib in _libs.values():
    try:
        r = (POINTER(POINTER(struct_RATorderStore))).in_dll(_lib, "r")
        break
    except:
        pass

# ./ratFront.h: 58
for _lib in _libs.values():
    try:
        nRatorders = (c_int).in_dll(_lib, "nRatorders")
        break
    except:
        pass

# ./ratFront.h: 59
try:
    mode = (c_int).in_dll(_libs["libratlib.so"], "mode")
except:
    pass

# ./ratFront.h: 66
for _lib in _libs.values():
    try:
        wavebandFiles = (POINTER(POINTER(c_char))).in_dll(_lib, "wavebandFiles")
        break
    except:
        pass

# ./ratFront.h: 67
for _lib in _libs.values():
    try:
        nWavebandFiles = (c_int).in_dll(_lib, "nWavebandFiles")
        break
    except:
        pass

# ./ratFront.h: 69
for _lib in _libs.values():
    try:
        wavebandbag = (POINTER(WavebandBag)).in_dll(_lib, "wavebandbag")
        break
    except:
        pass

# ./ratFront.h: 71
for _lib in _libs.values():
    try:
        inputImage = (String).in_dll(_lib, "inputImage")
        break
    except:
        pass

# ./ratFront.h: 72
for _lib in _libs.values():
    try:
        sampleCountImage = (String).in_dll(_lib, "sampleCountImage")
        break
    except:
        pass

# ./ratFront.h: 73
for _lib in _libs.values():
    try:
        outputImage = (String).in_dll(_lib, "outputImage")
        break
    except:
        pass

# ./ratFront.h: 74
for _lib in _libs.values():
    try:
        gridMap = (String).in_dll(_lib, "gridMap")
        break
    except:
        pass

# ./ratFront.h: 75
for _lib in _libs.values():
    try:
        PSFRead = (c_int).in_dll(_lib, "PSFRead")
        break
    except:
        pass

# ./ratFront.h: 76
for _lib in _libs.values():
    try:
        modulationMap = (String).in_dll(_lib, "modulationMap")
        break
    except:
        pass

# ./ratFront.h: 79
for _lib in _libs.values():
    try:
        nScatteredImages = (c_int).in_dll(_lib, "nScatteredImages")
        break
    except:
        pass

# ./ratFront.h: 80
for _lib in _libs.values():
    try:
        scatteredImages = (POINTER(POINTER(c_char))).in_dll(_lib, "scatteredImages")
        break
    except:
        pass

# ./ratFront.h: 81
for _lib in _libs.values():
    try:
        scatterStart = (POINTER(c_int)).in_dll(_lib, "scatterStart")
        break
    except:
        pass

# ./ratFront.h: 82
for _lib in _libs.values():
    try:
        scatterEnd = (POINTER(c_int)).in_dll(_lib, "scatterEnd")
        break
    except:
        pass

# ./ratFront.h: 83
for _lib in _libs.values():
    try:
        scatterResultImage = (POINTER(POINTER(c_double))).in_dll(_lib, "scatterResultImage")
        break
    except:
        pass

# ./ratFront.h: 85
for _lib in _libs.values():
    try:
        resultImage = (String).in_dll(_lib, "resultImage")
        break
    except:
        pass

# ./ratFront.h: 86
for _lib in _libs.values():
    try:
        resultPsfImage = (String).in_dll(_lib, "resultPsfImage")
        break
    except:
        pass

# ./ratFront.h: 88
for _lib in _libs.values():
    try:
        name = (String).in_dll(_lib, "name")
        break
    except:
        pass

# ./ratFront.h: 89
for _lib in _libs.values():
    try:
        filename = (String).in_dll(_lib, "filename")
        break
    except:
        pass

# ./ratFront.h: 91
for _lib in _libs.values():
    try:
        nBands = (c_int).in_dll(_lib, "nBands")
        break
    except:
        pass

# ./ratFront.h: 92
for _lib in _libs.values():
    try:
        nBins = (c_int).in_dll(_lib, "nBins")
        break
    except:
        pass

# ./ratFront.h: 93
for _lib in _libs.values():
    try:
        nOrders = (c_int).in_dll(_lib, "nOrders")
        break
    except:
        pass

# ./ratFront.h: 94
for _lib in _libs.values():
    try:
        binStart = (c_double).in_dll(_lib, "binStart")
        break
    except:
        pass

# ./ratFront.h: 95
for _lib in _libs.values():
    try:
        binStep = (c_double).in_dll(_lib, "binStep")
        break
    except:
        pass

# ./ratFront.h: 96
for _lib in _libs.values():
    try:
        binMax = (c_double).in_dll(_lib, "binMax")
        break
    except:
        pass

# ./ratFront.h: 97
for _lib in _libs.values():
    try:
        imagedata = (POINTER(c_double)).in_dll(_lib, "imagedata")
        break
    except:
        pass

# ./ratFront.h: 98
for _lib in _libs.values():
    try:
        psfdata = (POINTER(c_double)).in_dll(_lib, "psfdata")
        break
    except:
        pass

# ./ratFront.h: 99
for _lib in _libs.values():
    try:
        radiance = (POINTER(c_double)).in_dll(_lib, "radiance")
        break
    except:
        pass

# ./ratFront.h: 100
for _lib in _libs.values():
    try:
        directRadiance = (POINTER(POINTER(POINTER(c_double)))).in_dll(_lib, "directRadiance")
        break
    except:
        pass

# ./ratFront.h: 101
for _lib in _libs.values():
    try:
        diffuseRadiance = (POINTER(POINTER(POINTER(c_double)))).in_dll(_lib, "diffuseRadiance")
        break
    except:
        pass

# ./ratFront.h: 102
for _lib in _libs.values():
    try:
        radianceNorm = (c_double * int(2)).in_dll(_lib, "radianceNorm")
        break
    except:
        pass

# ./ratFront.h: 105
for _lib in _libs.values():
    try:
        resultIntegralFormat = (c_int).in_dll(_lib, "resultIntegralFormat")
        break
    except:
        pass

# ./ratFront.h: 106
for _lib in _libs.values():
    try:
        resultIntegralMode = (c_int).in_dll(_lib, "resultIntegralMode")
        break
    except:
        pass

# ./ratFront.h: 107
for _lib in _libs.values():
    try:
        resultIntegralFilename = (String).in_dll(_lib, "resultIntegralFilename")
        break
    except:
        pass

# ./ratFront.h: 109
for _lib in _libs.values():
    try:
        resultIntegralFilenameDiffuse = (String).in_dll(_lib, "resultIntegralFilenameDiffuse")
        break
    except:
        pass

# ./ratFront.h: 112
for _lib in _libs.values():
    try:
        resultIntegralCompress = (c_int).in_dll(_lib, "resultIntegralCompress")
        break
    except:
        pass

# ./ratFront.h: 115
for _lib in _libs.values():
    try:
        cameraType = (c_int).in_dll(_lib, "cameraType")
        break
    except:
        pass

# ./ratFront.h: 117
for _lib in _libs.values():
    try:
        nPixels = (c_int).in_dll(_lib, "nPixels")
        break
    except:
        pass

# ./ratFront.h: 118
for _lib in _libs.values():
    try:
        rpp = (c_int).in_dll(_lib, "rpp")
        break
    except:
        pass

# ./ratFront.h: 121
for _lib in _libs.values():
    try:
        focalPoint = (triplet).in_dll(_lib, "focalPoint")
        break
    except:
        pass

# ./ratFront.h: 123
for _lib in _libs.values():
    try:
        principalPoint = (triplet).in_dll(_lib, "principalPoint")
        break
    except:
        pass

# ./ratFront.h: 124
for _lib in _libs.values():
    try:
        cameraX = (triplet).in_dll(_lib, "cameraX")
        break
    except:
        pass

# ./ratFront.h: 124
for _lib in _libs.values():
    try:
        cameraY = (triplet).in_dll(_lib, "cameraY")
        break
    except:
        pass

# ./ratFront.h: 126
for _lib in _libs.values():
    try:
        imagingPlaneDimensions = (c_double * int(2)).in_dll(_lib, "imagingPlaneDimensions")
        break
    except:
        pass

# ./ratFront.h: 129
for _lib in _libs.values():
    try:
        psf = (POINTER(c_double)).in_dll(_lib, "psf")
        break
    except:
        pass

# ./ratFront.h: 130
for _lib in _libs.values():
    try:
        nPsf = (c_int * int(2)).in_dll(_lib, "nPsf")
        break
    except:
        pass

# ./ratFront.h: 131
for _lib in _libs.values():
    try:
        psfmap = (POINTER(c_int) * int(2)).in_dll(_lib, "psfmap")
        break
    except:
        pass

# ./ratFront.h: 132
for _lib in _libs.values():
    try:
        psfSampleCount = (POINTER(c_int)).in_dll(_lib, "psfSampleCount")
        break
    except:
        pass

# ./ratFront.h: 133
for _lib in _libs.values():
    try:
        psfModulation = (POINTER(c_double)).in_dll(_lib, "psfModulation")
        break
    except:
        pass

# ./ratFront.h: 136
for _lib in _libs.values():
    try:
        pulse = (POINTER(c_double)).in_dll(_lib, "pulse")
        break
    except:
        pass

# ./ratFront.h: 137
for _lib in _libs.values():
    try:
        nPulse = (c_int).in_dll(_lib, "nPulse")
        break
    except:
        pass

# ./ratFront.h: 138
for _lib in _libs.values():
    try:
        pulseResolution = (c_double).in_dll(_lib, "pulseResolution")
        break
    except:
        pass

# ./ratFront.h: 139
for _lib in _libs.values():
    try:
        pulseStart = (c_double).in_dll(_lib, "pulseStart")
        break
    except:
        pass

# ./ratFront.h: 140
for _lib in _libs.values():
    try:
        pulseType = (String).in_dll(_lib, "pulseType")
        break
    except:
        pass

# ./ratFront.h: 141
for _lib in _libs.values():
    try:
        pulseSD = (c_double).in_dll(_lib, "pulseSD")
        break
    except:
        pass

# ./ratFront.h: 142
for _lib in _libs.values():
    try:
        pulseLength = (c_double).in_dll(_lib, "pulseLength")
        break
    except:
        pass

# ./ratFront.h: 143
for _lib in _libs.values():
    try:
        pulseSamples = (c_int).in_dll(_lib, "pulseSamples")
        break
    except:
        pass

# ./ratFront.h: 144
for _lib in _libs.values():
    try:
        pulseIPFile = (String).in_dll(_lib, "pulseIPFile")
        break
    except:
        pass

# ./ratFront.h: 145
for _lib in _libs.values():
    try:
        pulseOPFile = (String).in_dll(_lib, "pulseOPFile")
        break
    except:
        pass

# ./ratFront.h: 146
for _lib in _libs.values():
    try:
        pulseSample = (POINTER(c_double)).in_dll(_lib, "pulseSample")
        break
    except:
        pass

# ./ratFront.h: 147
for _lib in _libs.values():
    try:
        pulseStep = (c_double).in_dll(_lib, "pulseStep")
        break
    except:
        pass

# ./ratFront.h: 148
for _lib in _libs.values():
    try:
        cumulativePulse = (POINTER(c_double)).in_dll(_lib, "cumulativePulse")
        break
    except:
        pass

# ./ratFront.h: 151
for _lib in _libs.values():
    try:
        nS = (c_int * int(2)).in_dll(_lib, "nS")
        break
    except:
        pass

# ./ratFront.h: 153
for _lib in _libs.values():
    try:
        focalAxis = (triplet).in_dll(_lib, "focalAxis")
        break
    except:
        pass

# ./ratFront.h: 154
for _lib in _libs.values():
    try:
        focalLength = (c_double).in_dll(_lib, "focalLength")
        break
    except:
        pass

# ./ratFront.h: 156
for _lib in _libs.values():
    try:
        twist = (c_double).in_dll(_lib, "twist")
        break
    except:
        pass

# ./ratFront.h: 156
for _lib in _libs.values():
    try:
        zenith = (c_double).in_dll(_lib, "zenith")
        break
    except:
        pass

# ./ratFront.h: 156
for _lib in _libs.values():
    try:
        azimuth = (c_double).in_dll(_lib, "azimuth")
        break
    except:
        pass

# ./ratFront.h: 158
for _lib in _libs.values():
    try:
        fov = (c_double * int(2)).in_dll(_lib, "fov")
        break
    except:
        pass

# ./ratFront.h: 159
for _lib in _libs.values():
    try:
        fovMax = (c_double).in_dll(_lib, "fovMax")
        break
    except:
        pass

# ./ratFront.h: 160
for _lib in _libs.values():
    try:
        aspectRatio = (c_double).in_dll(_lib, "aspectRatio")
        break
    except:
        pass

# ./ratFront.h: 173
class struct_anon_57(Structure):
    pass

struct_anon_57.__slots__ = [
    'nBands',
    'doSumImage',
    'hit',
    'sumImage',
    'sumRayLength',
    'nScatteredImages',
    'scatteredImages',
]
struct_anon_57._fields_ = [
    ('nBands', c_int),
    ('doSumImage', c_int),
    ('hit', POINTER(c_int)),
    ('sumImage', POINTER(c_double)),
    ('sumRayLength', POINTER(c_double)),
    ('nScatteredImages', c_int),
    ('scatteredImages', POINTER(POINTER(c_double))),
]

RATsample = struct_anon_57# ./ratFront.h: 173

# ./ratFront.h: 176
if _libs["libratlib.so"].has("RATreadSpectra", "cdecl"):
    RATreadSpectra = _libs["libratlib.so"].get("RATreadSpectra", "cdecl")
    RATreadSpectra.argtypes = [String]
    RATreadSpectra.restype = POINTER(POINTER(c_double))

# ./ratFront.h: 180
if _libs["libratlib.so"].has("RATputImage", "cdecl"):
    RATputImage = _libs["libratlib.so"].get("RATputImage", "cdecl")
    RATputImage.argtypes = [String, POINTER(None), c_int, c_int, c_int, c_int]
    RATputImage.restype = c_int

# ./ratFront.h: 187
if _libs["libratlib.so"].has("RATgetImage", "cdecl"):
    RATgetImage = _libs["libratlib.so"].get("RATgetImage", "cdecl")
    RATgetImage.argtypes = [String, POINTER(c_int), POINTER(c_int), POINTER(c_int), POINTER(c_int), POINTER(c_int)]
    RATgetImage.restype = POINTER(c_ubyte)
    RATgetImage.errcheck = lambda v,*a : cast(v, c_void_p)

# ./ratFront.h: 188
if _libs["libratlib.so"].has("RATgetImageDouble", "cdecl"):
    RATgetImageDouble = _libs["libratlib.so"].get("RATgetImageDouble", "cdecl")
    RATgetImageDouble.argtypes = [String, POINTER(c_int), POINTER(c_int), POINTER(c_int), POINTER(c_int)]
    RATgetImageDouble.restype = POINTER(c_ubyte)
    RATgetImageDouble.errcheck = lambda v,*a : cast(v, c_void_p)

# ./ratFront.h: 189
if _libs["libratlib.so"].has("RATgenerateGaussian", "cdecl"):
    RATgenerateGaussian = _libs["libratlib.so"].get("RATgenerateGaussian", "cdecl")
    RATgenerateGaussian.argtypes = [String, c_int, c_int, c_double, c_double, c_double, c_double, c_double, c_double]
    RATgenerateGaussian.restype = POINTER(c_double)

# ./libhipl.h: 27
if _libs["libratlib.so"].has("fix_desc", "cdecl"):
    fix_desc = _libs["libratlib.so"].get("fix_desc", "cdecl")
    fix_desc.argtypes = [String]
    if sizeof(c_int) == sizeof(c_void_p):
        fix_desc.restype = ReturnString
    else:
        fix_desc.restype = String
        fix_desc.errcheck = ReturnString

# ./imagelib.h: 39
class struct_anon_58(Structure):
    pass

struct_anon_58.__slots__ = [
    'statsSet',
    'min',
    'max',
]
struct_anon_58._fields_ = [
    ('statsSet', c_int),
    ('min', c_double * int(1280)),
    ('max', c_double * int(1280)),
]

Stats = struct_anon_58# ./imagelib.h: 39

# ./imagelib.h: 120
for _lib in _libs.values():
    try:
        verbose = (c_int).in_dll(_lib, "verbose")
        break
    except:
        pass

# ./imagelib.h: 121
for _lib in _libs.values():
    try:
        allocatedFlag = (c_int).in_dll(_lib, "allocatedFlag")
        break
    except:
        pass

# ./imagelib.h: 123
for _lib in _libs.values():
    try:
        openFlag = (c_int).in_dll(_lib, "openFlag")
        break
    except:
        pass

# ./imagelib.h: 125
try:
    mmap = (c_int).in_dll(_libs["libratlib.so"], "mmap")
except:
    pass

# ./imagelib.h: 127
for _lib in _libs.values():
    try:
        image_format = (c_int).in_dll(_lib, "image_format")
        break
    except:
        pass

# ./imagelib.h: 128
for _lib in _libs.values():
    try:
        fd = (c_int).in_dll(_lib, "fd")
        break
    except:
        pass

# ./imagelib.h: 187
if _libs["libratlib.so"].has("getDataSize", "cdecl"):
    getDataSize = _libs["libratlib.so"].get("getDataSize", "cdecl")
    getDataSize.argtypes = [c_int]
    getDataSize.restype = c_int

# ./prat.h: 107
if _libs["libratlib.so"].has("is_double", "cdecl"):
    is_double = _libs["libratlib.so"].get("is_double", "cdecl")
    is_double.argtypes = [String, POINTER(c_double)]
    is_double.restype = c_int

# /Users/plewis/Documents/GitHub/librat/src/rat.h: 5
if _libs["libratlib.so"].has("sphereTesselate", "cdecl"):
    sphereTesselate = _libs["libratlib.so"].get("sphereTesselate", "cdecl")
    sphereTesselate.argtypes = [c_int, POINTER(c_int)]
    sphereTesselate.restype = POINTER(c_double)

# /Users/plewis/Documents/GitHub/librat/src/rat.h: 6
if _libs["libratlib.so"].has("facetNormal", "cdecl"):
    facetNormal = _libs["libratlib.so"].get("facetNormal", "cdecl")
    facetNormal.argtypes = [POINTER(c_double), POINTER(struct_Facet_struct)]
    facetNormal.restype = c_double

# /Users/plewis/Documents/GitHub/librat/src/rat.h: 17
for _lib in _libs.values():
    if not _lib.has("RATsignalInterrupt", "cdecl"):
        continue
    RATsignalInterrupt = _lib.get("RATsignalInterrupt", "cdecl")
    RATsignalInterrupt.argtypes = [c_int]
    RATsignalInterrupt.restype = None
    break

# /Users/plewis/Documents/GitHub/librat/src/hipl_format.h: 49
try:
    PFBYTE = 0
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/hipl_format.h: 50
try:
    PFSHORT = 1
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/hipl_format.h: 51
try:
    PFINT = 2
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/hipl_format.h: 52
try:
    PFFLOAT = 3
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/hipl_format.h: 53
try:
    PFCOMPLEX = 4
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/hipl_format.h: 54
try:
    PFASCII = 5
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/hipl_format.h: 56
try:
    MSBFIRST = 1
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/hipl_format.h: 57
try:
    LSBFIRST = 2
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/hipl_format.h: 59
try:
    PFMSBF = 30
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/hipl_format.h: 60
try:
    PFLSBF = 31
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/hipl_format.h: 63
try:
    TRUE = 1
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/hipl_format.h: 67
try:
    FALSE = 0
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/hipl_format.h: 70
def desc_massage(A):
    return (fix_desc (A))

# /Users/plewis/Documents/GitHub/librat/src/hipl_format.h: 75
def halloc(A, B):
    return (calloc (A, B))

# /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/sys/param.h: 215
def MIN(a, b):
    return (a < b) and a or b

# /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/sys/param.h: 218
def MAX(a, b):
    return (a > b) and a or b

# /Users/plewis/Documents/GitHub/librat/src/useful3.h: 21
def MMAX(X, Y):
    return (MAX (X, Y))

# /Users/plewis/Documents/GitHub/librat/src/useful3.h: 22
def MMIN(X, Y):
    return (MIN (X, Y))

# /Users/plewis/Documents/GitHub/librat/src/useful3.h: 25
def ABS(X):
    return (X < 0) and (-X) or X

# /Users/plewis/Documents/GitHub/librat/src/useful3.h: 26
def AVERAGE(X, Y):
    return ((X + Y) / 2.0)

# /Users/plewis/Documents/GitHub/librat/src/useful3.h: 30
def DTOR(alpha):
    return ((alpha * M_PI) / 180.0)

# /Users/plewis/Documents/GitHub/librat/src/useful3.h: 31
def RTOD(alpha):
    return ((alpha * 180.0) / M_PI)

# /Users/plewis/Documents/GitHub/librat/src/useful3.h: 32
try:
    FATAL = 1
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/useful3.h: 33
try:
    NON_FATAL = 0
except:
    pass

# /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/math.h: 692
try:
    M_PI = 3.141592653589793
except:
    pass

# /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/math.h: 693
try:
    M_PI_2 = 1.5707963267948966
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/wavefront.h: 13
try:
    BIG = 1000000000.0
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/wavefront.h: 14
try:
    RAT_N_BLOCKS = 1000
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/intersect_objects.h: 12
try:
    BBOX = 0
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/intersect_objects.h: 13
try:
    FO = 1
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/intersect_objects.h: 14
try:
    SPHERE = 2
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/intersect_objects.h: 15
try:
    CYLINDER = 3
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/intersect_objects.h: 16
try:
    BEZIER = 4
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/intersect_objects.h: 17
try:
    PLANE = 5
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/intersect_objects.h: 18
try:
    CLONE = 6
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/intersect_objects.h: 19
try:
    DEM = 7
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/intersect_objects.h: 20
try:
    SPHERICAL_DEM = 8
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/intersect_objects.h: 21
try:
    ELLIPSE = 9
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/intersect_objects.h: 22
try:
    VOLUMETRIC_SPHERE = 10
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/intersect_objects.h: 23
try:
    VOLUMETRIC_CYLINDER = 11
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/intersect_objects.h: 24
try:
    VOLUMETRIC_ELLIPSE = 12
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/intersect_objects.h: 25
try:
    VOLUMETRIC_SPHEROID = 25
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/intersect_objects.h: 26
try:
    CLOSED_CYLINDER = 13
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/intersect_objects.h: 27
try:
    DISK = 14
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/intersect_objects.h: 28
try:
    BILINEARPATCH = 15
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/intersect_objects.h: 29
try:
    SPHEROID = 26
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/intersect_objects.h: 31
try:
    MAX_OBJECT_TYPES = 16
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/intersect_objects.h: 33
try:
    MAX_TREE_DEPTH = 20
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/materials.h: 43
try:
    SRM = 0
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/materials.h: 44
try:
    BUMPMAP = 1
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/materials.h: 45
try:
    BRDF_LUT = 2
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/materials.h: 46
try:
    PVD_BRDF_MODEL = 3
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/materials.h: 47
try:
    HIPL_TEXTURE_MAP = 4
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/materials.h: 48
try:
    AHMADD_BRDF_MODEL = 5
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/materials.h: 49
try:
    BEGUE_BRDF_MODEL = 6
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/materials.h: 50
try:
    PROSPECT_REFLECTANCE_TRANSMITTANCE_MODEL = 7
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/materials.h: 51
try:
    PROSPECT_REFLECTANCE_MODEL = 8
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/materials.h: 52
try:
    VOLUMETRIC = 9
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/materials.h: 53
try:
    M_TRANSPARENT = 10
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/materials.h: 54
try:
    RPV = 11
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/materials.h: 56
try:
    REFLECTANCE = 0
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/materials.h: 57
try:
    TRANSMITTANCE = 1
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 185
try:
    TRIANGULAR_REPEAT = 1
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 186
try:
    SQUARE_REPEAT = 0
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 238
try:
    VIEWER = 0
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 239
try:
    ILLUMINATION = 1
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/sensor_rsr.h: 4
try:
    MAX_NO_OF_RSR_SAMPLES = 2
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/sensor_rsr.h: 5
try:
    MAX_NO_RSR_SAMPS = 2
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/camera.h: 10
try:
    ALBEDO = 0
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/camera.h: 11
try:
    SPHERICAL = 1
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/camera.h: 12
try:
    PLANAR = 2
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/camera.h: 13
try:
    PLANAR2 = 3
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/globals.h: 1
try:
    RAY_LENGTH_TOL = 1e-05
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/mtllib.h: 4
try:
    DEFAULT_PRAT_MAX_MATERIALS = 1024
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 1
try:
    FILE_END = 9
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 3
try:
    UNKNOWN_TYPE = 0
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 4
try:
    OPEN_BBOX_RET = 100
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 5
try:
    OPEN_BBOX = '!{'
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 6
try:
    CLOSE_BBOX_RET = 1
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 7
try:
    CLOSE_BBOX = '!}'
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 8
try:
    GROUP_RET = 2
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 9
try:
    GROUP = 'g'
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 10
try:
    VERTEX_RET = 3
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 11
try:
    VERTEX = 'v'
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 12
try:
    LOCAL_RET = 4
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 13
try:
    LOCAL = '!local'
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 14
try:
    NORMAL_RET = 5
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 15
try:
    NORMAL1 = '!n'
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 16
try:
    NORMAL2 = '!normal'
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 17
try:
    DEFINE_RET = 6
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 18
try:
    DEFINE1 = '!define'
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 19
try:
    DEFINE2 = '#define'
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 20
try:
    TRIANGLE_RET = 7
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 21
try:
    TRIANGLE1 = 'f'
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 22
try:
    TRIANGLE2 = 'fo'
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 23
try:
    SPHERE_RET = 8
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 24
try:
    SPH = 'sph'
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 25
try:
    ELLIPSE_RET = 17
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 26
try:
    ELL = 'ell'
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 27
try:
    SPHER_RET = 26
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 28
try:
    SPHER = 'spheroid'
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 29
try:
    CYLINDER_RET = 9
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 30
try:
    CYL = 'cyl'
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 31
try:
    CLONE_RET = 10
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 32
try:
    CLONE1 = 'clone'
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 33
try:
    CLONE2 = '!clone'
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 34
try:
    CLONE3 = '#clone'
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 35
try:
    DEM_RET = 11
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 36
try:
    DEM1 = 'DEM'
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 37
try:
    DEM2 = '!DEM'
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 38
try:
    DEM3 = '#DEM'
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 39
try:
    DEM4 = 'dem'
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 40
try:
    DEM5 = '!dem'
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 41
try:
    DEM6 = '#dem'
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 42
try:
    PLANE_RET = 12
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 43
try:
    PLANE1 = 'PLANE'
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 44
try:
    PLANE2 = '!PLANE'
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 45
try:
    PLANE3 = '#PLANE'
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 46
try:
    PLANE4 = 'plane'
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 47
try:
    PLANE5 = '!plane'
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 48
try:
    PLANE6 = '#plane'
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 49
try:
    USEMTL_RET = 13
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 50
try:
    USEMTL = 'usemtl'
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 51
try:
    MTLLIB_RET = 14
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 52
try:
    MTLLIB = 'mtllib'
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 53
try:
    TRANS_RET = 15
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 56
try:
    TRANS = 'transformation_matrix'
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 57
try:
    SPHERICAL_DEM_RET = 16
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 58
try:
    SPHERICAL_DEM1 = 'SPHERICAL_DEM'
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 59
try:
    SPHERICAL_DEM2 = '!SPHERICAL_DEM'
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 60
try:
    SPHERICAL_DEM3 = '#SPHERICAL_DEM'
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 61
try:
    SPHERICAL_DEM4 = 'spherical_dem'
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 62
try:
    SPHERICAL_DEM5 = '!spherical_dem'
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 63
try:
    SPHERICAL_DEM6 = '#spherical_dem'
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 64
try:
    VOLUME_SPHERE_RET = 18
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 65
try:
    VOLUME_SP = 'vsph'
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 66
try:
    CLOSED_CYL = 'ccyl'
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 67
try:
    CLOSED_CYL_RET = 19
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 68
try:
    VOLUME_CYL_RET = 20
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 69
try:
    VOLUME_CYL = 'vcyl'
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 70
try:
    DISK_RET = 21
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 71
try:
    DISK1 = 'disc'
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 72
try:
    DISK2 = 'disk'
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 73
try:
    BILINEARPATCH_RET = 23
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 74
try:
    BILINEARPATCH0 = 'blp'
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 75
try:
    BILINEARPATCH1 = '#blp'
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 76
try:
    BILINEARPATCH2 = '!blp'
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 77
try:
    BILINEARPATCH3 = 'BiLinearPatch'
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 78
try:
    BILINEARPATCH4 = 'biLinearPatch'
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 79
try:
    BILINEARPATCH5 = '!BiLinearPatch'
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 80
try:
    BILINEARPATCH6 = '!biLinearPatch'
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 81
try:
    BILINEARPATCH7 = '#BiLinearPatch'
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 82
try:
    BILINEARPATCH8 = '#biLinearPatch'
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 83
try:
    VOLUME_ELLIPSE_RET = 22
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 84
try:
    VOLUME_ELLIPSE = 'vell'
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 85
try:
    VOLUME_SPHEROID_RET = 25
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/element_types.h: 86
try:
    VOLUME_SPHEROID = 'vspheroid'
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 1
try:
    MAX_BBOX_LIST = 128
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 203
try:
    MAX_STRING_LENGTH = 2048
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 204
try:
    SHORT_STRING_LENGTH = 1024
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 205
try:
    MAX_WAVEBANDS = 1024
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/bags.h: 206
try:
    MAX_SENSOR_FILES = 50
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/vector_macros.h: 1
def V_DOT(a, b):
    return (((((a.x).value) * ((b.x).value)) + (((a.y).value) * ((b.y).value))) + (((a.z).value) * ((b.z).value)))

# /Users/plewis/Documents/GitHub/librat/src/vector_macros.h: 2
def V_FACTOR(a, b):
    return (((a.x).value) * b)

# /Users/plewis/Documents/GitHub/librat/src/vector_macros.h: 3
def V_MOD(a):
    return (sqrt ((((((a.x).value) * ((a.x).value)) + (((a.y).value) * ((a.y).value))) + (((a.z).value) * ((a.z).value)))))

# ./bags.h: 1
try:
    MAX_BBOX_LIST = 128
except:
    pass

# ./globals.h: 1
try:
    RAY_LENGTH_TOL = 1e-05
except:
    pass

# ./element_types.h: 1
try:
    FILE_END = 9
except:
    pass

# ./element_types.h: 3
try:
    UNKNOWN_TYPE = 0
except:
    pass

# ./element_types.h: 4
try:
    OPEN_BBOX_RET = 100
except:
    pass

# ./element_types.h: 5
try:
    OPEN_BBOX = '!{'
except:
    pass

# ./element_types.h: 6
try:
    CLOSE_BBOX_RET = 1
except:
    pass

# ./element_types.h: 7
try:
    CLOSE_BBOX = '!}'
except:
    pass

# ./element_types.h: 8
try:
    GROUP_RET = 2
except:
    pass

# ./element_types.h: 9
try:
    GROUP = 'g'
except:
    pass

# ./element_types.h: 10
try:
    VERTEX_RET = 3
except:
    pass

# ./element_types.h: 11
try:
    VERTEX = 'v'
except:
    pass

# ./element_types.h: 12
try:
    LOCAL_RET = 4
except:
    pass

# ./element_types.h: 13
try:
    LOCAL = '!local'
except:
    pass

# ./element_types.h: 14
try:
    NORMAL_RET = 5
except:
    pass

# ./element_types.h: 15
try:
    NORMAL1 = '!n'
except:
    pass

# ./element_types.h: 16
try:
    NORMAL2 = '!normal'
except:
    pass

# ./element_types.h: 17
try:
    DEFINE_RET = 6
except:
    pass

# ./element_types.h: 18
try:
    DEFINE1 = '!define'
except:
    pass

# ./element_types.h: 19
try:
    DEFINE2 = '#define'
except:
    pass

# ./element_types.h: 20
try:
    TRIANGLE_RET = 7
except:
    pass

# ./element_types.h: 21
try:
    TRIANGLE1 = 'f'
except:
    pass

# ./element_types.h: 22
try:
    TRIANGLE2 = 'fo'
except:
    pass

# ./element_types.h: 23
try:
    SPHERE_RET = 8
except:
    pass

# ./element_types.h: 24
try:
    SPH = 'sph'
except:
    pass

# ./element_types.h: 25
try:
    ELLIPSE_RET = 17
except:
    pass

# ./element_types.h: 26
try:
    ELL = 'ell'
except:
    pass

# ./element_types.h: 27
try:
    SPHER_RET = 26
except:
    pass

# ./element_types.h: 28
try:
    SPHER = 'spheroid'
except:
    pass

# ./element_types.h: 29
try:
    CYLINDER_RET = 9
except:
    pass

# ./element_types.h: 30
try:
    CYL = 'cyl'
except:
    pass

# ./element_types.h: 31
try:
    CLONE_RET = 10
except:
    pass

# ./element_types.h: 32
try:
    CLONE1 = 'clone'
except:
    pass

# ./element_types.h: 33
try:
    CLONE2 = '!clone'
except:
    pass

# ./element_types.h: 34
try:
    CLONE3 = '#clone'
except:
    pass

# ./element_types.h: 35
try:
    DEM_RET = 11
except:
    pass

# ./element_types.h: 36
try:
    DEM1 = 'DEM'
except:
    pass

# ./element_types.h: 37
try:
    DEM2 = '!DEM'
except:
    pass

# ./element_types.h: 38
try:
    DEM3 = '#DEM'
except:
    pass

# ./element_types.h: 39
try:
    DEM4 = 'dem'
except:
    pass

# ./element_types.h: 40
try:
    DEM5 = '!dem'
except:
    pass

# ./element_types.h: 41
try:
    DEM6 = '#dem'
except:
    pass

# ./element_types.h: 42
try:
    PLANE_RET = 12
except:
    pass

# ./element_types.h: 43
try:
    PLANE1 = 'PLANE'
except:
    pass

# ./element_types.h: 44
try:
    PLANE2 = '!PLANE'
except:
    pass

# ./element_types.h: 45
try:
    PLANE3 = '#PLANE'
except:
    pass

# ./element_types.h: 46
try:
    PLANE4 = 'plane'
except:
    pass

# ./element_types.h: 47
try:
    PLANE5 = '!plane'
except:
    pass

# ./element_types.h: 48
try:
    PLANE6 = '#plane'
except:
    pass

# ./element_types.h: 49
try:
    USEMTL_RET = 13
except:
    pass

# ./element_types.h: 50
try:
    USEMTL = 'usemtl'
except:
    pass

# ./element_types.h: 51
try:
    MTLLIB_RET = 14
except:
    pass

# ./element_types.h: 52
try:
    MTLLIB = 'mtllib'
except:
    pass

# ./element_types.h: 53
try:
    TRANS_RET = 15
except:
    pass

# ./element_types.h: 56
try:
    TRANS = 'transformation_matrix'
except:
    pass

# ./element_types.h: 57
try:
    SPHERICAL_DEM_RET = 16
except:
    pass

# ./element_types.h: 58
try:
    SPHERICAL_DEM1 = 'SPHERICAL_DEM'
except:
    pass

# ./element_types.h: 59
try:
    SPHERICAL_DEM2 = '!SPHERICAL_DEM'
except:
    pass

# ./element_types.h: 60
try:
    SPHERICAL_DEM3 = '#SPHERICAL_DEM'
except:
    pass

# ./element_types.h: 61
try:
    SPHERICAL_DEM4 = 'spherical_dem'
except:
    pass

# ./element_types.h: 62
try:
    SPHERICAL_DEM5 = '!spherical_dem'
except:
    pass

# ./element_types.h: 63
try:
    SPHERICAL_DEM6 = '#spherical_dem'
except:
    pass

# ./element_types.h: 64
try:
    VOLUME_SPHERE_RET = 18
except:
    pass

# ./element_types.h: 65
try:
    VOLUME_SP = 'vsph'
except:
    pass

# ./element_types.h: 66
try:
    CLOSED_CYL = 'ccyl'
except:
    pass

# ./element_types.h: 67
try:
    CLOSED_CYL_RET = 19
except:
    pass

# ./element_types.h: 68
try:
    VOLUME_CYL_RET = 20
except:
    pass

# ./element_types.h: 69
try:
    VOLUME_CYL = 'vcyl'
except:
    pass

# ./element_types.h: 70
try:
    DISK_RET = 21
except:
    pass

# ./element_types.h: 71
try:
    DISK1 = 'disc'
except:
    pass

# ./element_types.h: 72
try:
    DISK2 = 'disk'
except:
    pass

# ./element_types.h: 73
try:
    BILINEARPATCH_RET = 23
except:
    pass

# ./element_types.h: 74
try:
    BILINEARPATCH0 = 'blp'
except:
    pass

# ./element_types.h: 75
try:
    BILINEARPATCH1 = '#blp'
except:
    pass

# ./element_types.h: 76
try:
    BILINEARPATCH2 = '!blp'
except:
    pass

# ./element_types.h: 77
try:
    BILINEARPATCH3 = 'BiLinearPatch'
except:
    pass

# ./element_types.h: 78
try:
    BILINEARPATCH4 = 'biLinearPatch'
except:
    pass

# ./element_types.h: 79
try:
    BILINEARPATCH5 = '!BiLinearPatch'
except:
    pass

# ./element_types.h: 80
try:
    BILINEARPATCH6 = '!biLinearPatch'
except:
    pass

# ./element_types.h: 81
try:
    BILINEARPATCH7 = '#BiLinearPatch'
except:
    pass

# ./element_types.h: 82
try:
    BILINEARPATCH8 = '#biLinearPatch'
except:
    pass

# ./element_types.h: 83
try:
    VOLUME_ELLIPSE_RET = 22
except:
    pass

# ./element_types.h: 84
try:
    VOLUME_ELLIPSE = 'vell'
except:
    pass

# ./element_types.h: 85
try:
    VOLUME_SPHEROID_RET = 25
except:
    pass

# ./element_types.h: 86
try:
    VOLUME_SPHEROID = 'vspheroid'
except:
    pass

# ./vector_macros.h: 1
def V_DOT(a, b):
    return (((((a.x).value) * ((b.x).value)) + (((a.y).value) * ((b.y).value))) + (((a.z).value) * ((b.z).value)))

# ./vector_macros.h: 2
def V_FACTOR(a, b):
    return (((a.x).value) * b)

# ./vector_macros.h: 3
def V_MOD(a):
    return (sqrt ((((((a.x).value) * ((a.x).value)) + (((a.y).value) * ((a.y).value))) + (((a.z).value) * ((a.z).value)))))

# ./frat.h: 16
try:
    TRANSPARENT = 10
except:
    pass

# ./rat.h: 2
try:
    MAX_SUNS = 180
except:
    pass

# ./ratFront.h: 11
try:
    ORTHOGRAPHIC = 1
except:
    pass

# ./ratFront.h: 12
try:
    PERSPECTIVE = 0
except:
    pass

# ./ratFront.h: 13
try:
    DIFFUSE = 2
except:
    pass

# ./ratFront.h: 14
try:
    MAXFIELDS = 5
except:
    pass

# ./ratFront.h: 16
try:
    RESULT_INTEGRAL_MODE_WAVEBAND = 0
except:
    pass

# ./ratFront.h: 17
try:
    RESULT_INTEGRAL_MODE_SCATTERINGORDER = 1
except:
    pass

# ./ratFront.h: 18
try:
    RESULT_INTEGRAL_MODE_DISTANCE = 2
except:
    pass

# ./ratFront.h: 19
try:
    RESULT_INTEGRAL_FORMAT_ASCII = 0
except:
    pass

# ./ratFront.h: 20
try:
    RESULT_INTEGRAL_FORMAT_BINARY = 1
except:
    pass

# ./imagelib.h: 7
try:
    TRUE = 1
except:
    pass

# ./imagelib.h: 8
try:
    FALSE = 0
except:
    pass

# ./imagelib.h: 9
try:
    BLANK_EDGES = 1
except:
    pass

# ./imagelib.h: 10
try:
    WRAPAROUND = 2
except:
    pass

# ./imagelib.h: 11
try:
    FATAL_ACCESS = 3
except:
    pass

# ./imagelib.h: 17
try:
    CALC_STATS_STDIN = 3
except:
    pass

# ./imagelib.h: 18
try:
    CALC_STATS = 5
except:
    pass

# ./imagelib.h: 20
try:
    BIL = 0
except:
    pass

# ./imagelib.h: 21
try:
    BSQ = 1
except:
    pass

# ./imagelib.h: 24
try:
    NO_FORMAT = (-1)
except:
    pass

# ./imagelib.h: 25
try:
    IMAGE_BYTE = 0
except:
    pass

# ./imagelib.h: 26
try:
    IMAGE_SHORT = 1
except:
    pass

# ./imagelib.h: 27
try:
    IMAGE_INT = 2
except:
    pass

# ./imagelib.h: 28
try:
    IMAGE_FLOAT = 3
except:
    pass

# ./imagelib.h: 29
try:
    NFORMATS = 4
except:
    pass

# ./imagelib.h: 32
try:
    MAXFRAMES = 1280
except:
    pass

# ./prat.h: 80
def EXPONENTIAL(X):
    return (X < (-300)) and 0 or (exp (X))

# ./prat.h: 81
def FSIGN(a):
    return (a < 0.0) and (-1) or 1

# ./prat.h: 86
try:
    pi = M_PI
except:
    pass

# ./prat.h: 87
try:
    PI_OVER_2 = M_PI_2
except:
    pass

# ./prat.h: 88
try:
    PI_2 = (2.0 * M_PI)
except:
    pass

# ./prat.h: 90
try:
    DIFFUSE = 2
except:
    pass

# ./prat.h: 93
try:
    TTOL = 0.001
except:
    pass

# ./prat.h: 94
try:
    BOXTOL = 1e-05
except:
    pass

# ./prat.h: 95
try:
    RAY_TOL = 0.01
except:
    pass

# ./prat.h: 96
try:
    UNITY_TOL = 0.001
except:
    pass

# ./prat.h: 101
try:
    IP = 1
except:
    pass

# ./prat.h: 102
try:
    OP = 2
except:
    pass

# ./prat.h: 104
try:
    OLD = 0
except:
    pass

# ./prat.h: 105
try:
    NEW = 1
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/rat.h: 2
try:
    MAX_SUNS = 180
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/image_formats.h: 5
try:
    N_FORMATS = 2
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/image_formats.h: 7
try:
    HIPSFORMAT = 1
except:
    pass

# /Users/plewis/Documents/GitHub/librat/src/image_formats.h: 8
try:
    ENVIFORMAT = 2
except:
    pass

header = struct_header# /Users/plewis/Documents/GitHub/librat/src/hipl_format.h: 26

Sort_Hit = struct_Sort_Hit# /Users/plewis/Documents/GitHub/librat/src/intersect_objects.h: 4

Material_table_struct = struct_Material_table_struct# /Users/plewis/Documents/GitHub/librat/src/materials.h: 10

Plane_struct = struct_Plane_struct# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 24

D_Facet_struct = struct_D_Facet_struct# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 61

Disk_struct = struct_Disk_struct# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 75

BiLinearPatch_struct = struct_BiLinearPatch_struct# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 88

Cylinder_struct = struct_Cylinder_struct# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 104

Ellipse_struct = struct_Ellipse_struct# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 122

Spheroid_struct = struct_Spheroid_struct# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 137

Sphere_struct = struct_Sphere_struct# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 192

D_Sphere_struct = struct_D_Sphere_struct# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 203

Spherical_Dem_struct = struct_Spherical_Dem_struct# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 225

Bezier4_struct = struct_Bezier4_struct# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 253

Facet_struct = struct_Facet_struct# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 262

Dem_struct = struct_Dem_struct# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 279

Bbox = struct_Bbox# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 339

Clone = struct_Clone# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 357

Content = struct_Content# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 294

BOX = struct_BOX# /Users/plewis/Documents/GitHub/librat/src/prat_objects.h: 476

RATorderStore = struct_RATorderStore# ./ratFront.h: 39

# No inserted files

# No prefix-stripping

