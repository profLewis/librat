r"""Wrapper for hipl_format.h

Generated with:
/Users/plewis/opt/anaconda3/bin/ctypesgen -L. -L/Users/plewis/librat/src -I. -I/Users/plewis/librat/src -llibratlib.so hipl_format.h define_float.h useful3.h vectors2.h 4D_vectors.h 2D_vectors.h wavefront.h hips.h allocate.h intersect_objects.h materials.h prat_objects.h sensor_rsr.h camera.h globals.h reflectance.h volume.h rpv.h mtllib.h element_types.h reflectance_functions.h bags.h vector_macros.h 3D_vectors_double.h prat.h materials.h frat.h rat.h ratFront.h imagelib.h image_formats.h -o librat.py

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

    def __init__(self, obj=b""):
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
_libdirs = ['.', '/Users/plewis/librat/src']

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

add_library_search_dirs(['.', '/Users/plewis/librat/src'])

# Begin libraries
_libs["libratlib.so"] = load_library("libratlib.so")

# 1 libraries
# End libraries

# No modules

# /Users/plewis/librat/src/hipl_format.h: 26
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

# /Users/plewis/librat/src/useful3.h: 29
if _libs["libratlib.so"].has("drand48", "cdecl"):
    drand48 = _libs["libratlib.so"].get("drand48", "cdecl")
    drand48.argtypes = []
    drand48.restype = c_double

# /Users/plewis/librat/src/useful3.h: 30
if _libs["libratlib.so"].has("srand48", "cdecl"):
    srand48 = _libs["libratlib.so"].get("srand48", "cdecl")
    srand48.argtypes = []
    srand48.restype = None

# /Users/plewis/librat/src/useful3.h: 32
for _lib in _libs.values():
    if not _lib.has("find_switch", "cdecl"):
        continue
    find_switch = _lib.get("find_switch", "cdecl")
    find_switch.argtypes = []
    find_switch.restype = c_int
    break

# /Users/plewis/librat/src/useful3.h: 33
for _lib in _libs.values():
    if not _lib.has("find_string", "cdecl"):
        continue
    find_string = _lib.get("find_string", "cdecl")
    find_string.argtypes = []
    find_string.restype = c_int
    break

# /Users/plewis/librat/src/useful3.h: 47
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

triplet = struct_anon_2# /Users/plewis/librat/src/useful3.h: 47

# /Users/plewis/librat/src/useful3.h: 50
class struct_anon_3(Structure):
    pass

struct_anon_3.__slots__ = [
    'x',
    'y',
    'z',
]
struct_anon_3._fields_ = [
    ('x', c_double),
    ('y', c_double),
    ('z', c_double),
]

Triplet = struct_anon_3# /Users/plewis/librat/src/useful3.h: 50

# /Users/plewis/librat/src/useful3.h: 56
class struct_anon_4(Structure):
    pass

struct_anon_4.__slots__ = [
    'data',
]
struct_anon_4._fields_ = [
    ('data', (c_double * int(3)) * int(3)),
]

Matrix3 = struct_anon_4# /Users/plewis/librat/src/useful3.h: 56

# /Users/plewis/librat/src/useful3.h: 63
class struct_anon_5(Structure):
    pass

struct_anon_5.__slots__ = [
    'data',
]
struct_anon_5._fields_ = [
    ('data', (c_double * int(3)) * int(3)),
]

matrix3 = struct_anon_5# /Users/plewis/librat/src/useful3.h: 63

# /Users/plewis/librat/src/useful3.h: 70
class struct_anon_6(Structure):
    pass

struct_anon_6.__slots__ = [
    'data',
]
struct_anon_6._fields_ = [
    ('data', (c_double * int(4)) * int(4)),
]

Matrix4 = struct_anon_6# /Users/plewis/librat/src/useful3.h: 70

# /Users/plewis/librat/src/useful3.h: 77
class struct_anon_7(Structure):
    pass

struct_anon_7.__slots__ = [
    'v',
]
struct_anon_7._fields_ = [
    ('v', c_double * int(4)),
]

Vector4 = struct_anon_7# /Users/plewis/librat/src/useful3.h: 77

# /Users/plewis/librat/src/useful3.h: 84
class struct_anon_8(Structure):
    pass

struct_anon_8.__slots__ = [
    'ox',
    'oy',
    'sx',
    'sy',
    'gx',
    'gy',
    'row',
    'col',
]
struct_anon_8._fields_ = [
    ('ox', c_double),
    ('oy', c_double),
    ('sx', c_double),
    ('sy', c_double),
    ('gx', c_double),
    ('gy', c_double),
    ('row', c_int),
    ('col', c_int),
]

image_descriptor = struct_anon_8# /Users/plewis/librat/src/useful3.h: 84

# /Users/plewis/librat/src/useful3.h: 91
class struct_anon_9(Structure):
    pass

struct_anon_9.__slots__ = [
    'b',
    'i',
]
struct_anon_9._fields_ = [
    ('b', String),
    ('i', POINTER(c_int)),
]

char_int = struct_anon_9# /Users/plewis/librat/src/useful3.h: 91

# /Users/plewis/librat/src/useful3.h: 96
class struct_anon_10(Structure):
    pass

struct_anon_10.__slots__ = [
    'flag',
    'Type',
    'no_of_parameters',
]
struct_anon_10._fields_ = [
    ('flag', POINTER(POINTER(c_char))),
    ('Type', String),
    ('no_of_parameters', String),
]

Data_type = struct_anon_10# /Users/plewis/librat/src/useful3.h: 96

# /Users/plewis/librat/src/useful3.h: 102
class struct_anon_11(Structure):
    pass

struct_anon_11.__slots__ = [
    'x',
    'y',
]
struct_anon_11._fields_ = [
    ('x', c_double),
    ('y', c_double),
]

Pair = struct_anon_11# /Users/plewis/librat/src/useful3.h: 102

# /Users/plewis/librat/src/useful3.h: 105
class struct_anon_12(Structure):
    pass

struct_anon_12.__slots__ = [
    'x',
    'y',
]
struct_anon_12._fields_ = [
    ('x', c_double),
    ('y', c_double),
]

pair = struct_anon_12# /Users/plewis/librat/src/useful3.h: 105

# /Users/plewis/librat/src/useful3.h: 111
class struct_anon_13(Structure):
    pass

struct_anon_13.__slots__ = [
    'x',
    'y',
]
struct_anon_13._fields_ = [
    ('x', c_int),
    ('y', c_int),
]

ipair = struct_anon_13# /Users/plewis/librat/src/useful3.h: 111

# /Users/plewis/librat/src/useful3.h: 114
class struct_anon_14(Structure):
    pass

struct_anon_14.__slots__ = [
    'column',
]
struct_anon_14._fields_ = [
    ('column', pair * int(2)),
]

matrix_2D = struct_anon_14# /Users/plewis/librat/src/useful3.h: 114

# /Users/plewis/librat/src/useful3.h: 117
class struct_anon_15(Structure):
    pass

struct_anon_15.__slots__ = [
    'column',
]
struct_anon_15._fields_ = [
    ('column', Pair * int(2)),
]

Matrix_2D = struct_anon_15# /Users/plewis/librat/src/useful3.h: 117

# /Users/plewis/librat/src/useful3.h: 120
class struct_anon_16(Structure):
    pass

struct_anon_16.__slots__ = [
    'vertex',
]
struct_anon_16._fields_ = [
    ('vertex', pair * int(3)),
]

triangle = struct_anon_16# /Users/plewis/librat/src/useful3.h: 120

# /Users/plewis/librat/src/useful3.h: 124
class struct_anon_17(Structure):
    pass

struct_anon_17.__slots__ = [
    'vertex',
]
struct_anon_17._fields_ = [
    ('vertex', Pair * int(3)),
]

Triangle = struct_anon_17# /Users/plewis/librat/src/useful3.h: 124

# /Users/plewis/librat/src/useful3.h: 126
for _lib in _libs.values():
    if not _lib.has("error", "cdecl"):
        continue
    error = _lib.get("error", "cdecl")
    error.argtypes = []
    error.restype = None
    break

# /Users/plewis/librat/src/vectors2.h: 3
if _libs["libratlib.so"].has("V_mod", "cdecl"):
    V_mod = _libs["libratlib.so"].get("V_mod", "cdecl")
    V_mod.argtypes = []
    V_mod.restype = c_double

# /Users/plewis/librat/src/vectors2.h: 3
if _libs["libratlib.so"].has("V_dot", "cdecl"):
    V_dot = _libs["libratlib.so"].get("V_dot", "cdecl")
    V_dot.argtypes = []
    V_dot.restype = c_double

# /Users/plewis/librat/src/vectors2.h: 3
if _libs["libratlib.so"].has("xy_angle", "cdecl"):
    xy_angle = _libs["libratlib.so"].get("xy_angle", "cdecl")
    xy_angle.argtypes = []
    xy_angle.restype = c_double

# /Users/plewis/librat/src/vectors2.h: 3
if _libs["libratlib.so"].has("m3_inverse", "cdecl"):
    m3_inverse = _libs["libratlib.so"].get("m3_inverse", "cdecl")
    m3_inverse.argtypes = []
    m3_inverse.restype = c_double

# /Users/plewis/librat/src/vectors2.h: 4
if _libs["libratlib.so"].has("rotate_about", "cdecl"):
    rotate_about = _libs["libratlib.so"].get("rotate_about", "cdecl")
    rotate_about.argtypes = []
    rotate_about.restype = triplet

# /Users/plewis/librat/src/vectors2.h: 4
if _libs["libratlib.so"].has("rotate_vector", "cdecl"):
    rotate_vector = _libs["libratlib.so"].get("rotate_vector", "cdecl")
    rotate_vector.argtypes = []
    rotate_vector.restype = triplet

# /Users/plewis/librat/src/vectors2.h: 4
if _libs["libratlib.so"].has("multiply_vector", "cdecl"):
    multiply_vector = _libs["libratlib.so"].get("multiply_vector", "cdecl")
    multiply_vector.argtypes = []
    multiply_vector.restype = triplet

# /Users/plewis/librat/src/vectors2.h: 4
if _libs["libratlib.so"].has("spherical_to_cartesian", "cdecl"):
    spherical_to_cartesian = _libs["libratlib.so"].get("spherical_to_cartesian", "cdecl")
    spherical_to_cartesian.argtypes = []
    spherical_to_cartesian.restype = triplet

# /Users/plewis/librat/src/vectors2.h: 4
if _libs["libratlib.so"].has("cartesian_to_spherical", "cdecl"):
    cartesian_to_spherical = _libs["libratlib.so"].get("cartesian_to_spherical", "cdecl")
    cartesian_to_spherical.argtypes = []
    cartesian_to_spherical.restype = triplet

# /Users/plewis/librat/src/vectors2.h: 4
if _libs["libratlib.so"].has("V_factor", "cdecl"):
    V_factor = _libs["libratlib.so"].get("V_factor", "cdecl")
    V_factor.argtypes = []
    V_factor.restype = triplet

# /Users/plewis/librat/src/vectors2.h: 4
if _libs["libratlib.so"].has("vector_plus", "cdecl"):
    vector_plus = _libs["libratlib.so"].get("vector_plus", "cdecl")
    vector_plus.argtypes = []
    vector_plus.restype = triplet

# /Users/plewis/librat/src/vectors2.h: 4
if _libs["libratlib.so"].has("vector_minus", "cdecl"):
    vector_minus = _libs["libratlib.so"].get("vector_minus", "cdecl")
    vector_minus.argtypes = []
    vector_minus.restype = triplet

# /Users/plewis/librat/src/vectors2.h: 4
if _libs["libratlib.so"].has("normalise", "cdecl"):
    normalise = _libs["libratlib.so"].get("normalise", "cdecl")
    normalise.argtypes = []
    normalise.restype = triplet

# /Users/plewis/librat/src/vectors2.h: 4
if _libs["libratlib.so"].has("vector_cross", "cdecl"):
    vector_cross = _libs["libratlib.so"].get("vector_cross", "cdecl")
    vector_cross.argtypes = []
    vector_cross.restype = triplet

# /Users/plewis/librat/src/vectors2.h: 4
if _libs["libratlib.so"].has("vector_copy", "cdecl"):
    vector_copy = _libs["libratlib.so"].get("vector_copy", "cdecl")
    vector_copy.argtypes = []
    vector_copy.restype = triplet

# /Users/plewis/librat/src/vectors2.h: 4
if _libs["libratlib.so"].has("vector_cross", "cdecl"):
    vector_cross = _libs["libratlib.so"].get("vector_cross", "cdecl")
    vector_cross.argtypes = []
    vector_cross.restype = triplet

# /Users/plewis/librat/src/vectors2.h: 4
if _libs["libratlib.so"].has("matrix3_mult", "cdecl"):
    matrix3_mult = _libs["libratlib.so"].get("matrix3_mult", "cdecl")
    matrix3_mult.argtypes = []
    matrix3_mult.restype = triplet

# /Users/plewis/librat/src/vectors2.h: 4
if _libs["libratlib.so"].has("vector_copy2", "cdecl"):
    vector_copy2 = _libs["libratlib.so"].get("vector_copy2", "cdecl")
    vector_copy2.argtypes = []
    vector_copy2.restype = triplet

# /Users/plewis/librat/src/vectors2.h: 6
if _libs["libratlib.so"].has("matrix3_ip", "cdecl"):
    matrix3_ip = _libs["libratlib.so"].get("matrix3_ip", "cdecl")
    matrix3_ip.argtypes = []
    matrix3_ip.restype = matrix3

# /Users/plewis/librat/src/vectors2.h: 6
if _libs["libratlib.so"].has("matrix3_copy", "cdecl"):
    matrix3_copy = _libs["libratlib.so"].get("matrix3_copy", "cdecl")
    matrix3_copy.argtypes = []
    matrix3_copy.restype = matrix3

# /Users/plewis/librat/src/vectors2.h: 6
if _libs["libratlib.so"].has("matrix3_inverse", "cdecl"):
    matrix3_inverse = _libs["libratlib.so"].get("matrix3_inverse", "cdecl")
    matrix3_inverse.argtypes = []
    matrix3_inverse.restype = matrix3

# /Users/plewis/librat/src/vectors2.h: 6
if _libs["libratlib.so"].has("inverse3", "cdecl"):
    inverse3 = _libs["libratlib.so"].get("inverse3", "cdecl")
    inverse3.argtypes = []
    inverse3.restype = matrix3

# /Users/plewis/librat/src/vectors2.h: 6
if _libs["libratlib.so"].has("transpose3", "cdecl"):
    transpose3 = _libs["libratlib.so"].get("transpose3", "cdecl")
    transpose3.argtypes = []
    transpose3.restype = matrix3

# /Users/plewis/librat/src/vectors2.h: 7
if _libs["libratlib.so"].has("scale", "cdecl"):
    scale = _libs["libratlib.so"].get("scale", "cdecl")
    scale.argtypes = []
    scale.restype = c_double

# /Users/plewis/librat/src/vectors2.h: 7
if _libs["libratlib.so"].has("cofactors", "cdecl"):
    cofactors = _libs["libratlib.so"].get("cofactors", "cdecl")
    cofactors.argtypes = []
    cofactors.restype = c_double

# /Users/plewis/librat/src/4D_vectors.h: 12
if _libs["libratlib.so"].has("clock_count", "cdecl"):
    clock_count = _libs["libratlib.so"].get("clock_count", "cdecl")
    clock_count.argtypes = []
    clock_count.restype = c_int

# /Users/plewis/librat/src/4D_vectors.h: 13
if _libs["libratlib.so"].has("modulus_matrix4", "cdecl"):
    modulus_matrix4 = _libs["libratlib.so"].get("modulus_matrix4", "cdecl")
    modulus_matrix4.argtypes = []
    modulus_matrix4.restype = c_double

# /Users/plewis/librat/src/4D_vectors.h: 14
if _libs["libratlib.so"].has("multiply_matrix4_vector3", "cdecl"):
    multiply_matrix4_vector3 = _libs["libratlib.so"].get("multiply_matrix4_vector3", "cdecl")
    multiply_matrix4_vector3.argtypes = []
    multiply_matrix4_vector3.restype = triplet

# /Users/plewis/librat/src/4D_vectors.h: 14
if _libs["libratlib.so"].has("multiply_upper_matrix3_vector3", "cdecl"):
    multiply_upper_matrix3_vector3 = _libs["libratlib.so"].get("multiply_upper_matrix3_vector3", "cdecl")
    multiply_upper_matrix3_vector3.argtypes = []
    multiply_upper_matrix3_vector3.restype = triplet

# /Users/plewis/librat/src/4D_vectors.h: 14
if _libs["libratlib.so"].has("multiply_matrix4_vector3", "cdecl"):
    multiply_matrix4_vector3 = _libs["libratlib.so"].get("multiply_matrix4_vector3", "cdecl")
    multiply_matrix4_vector3.argtypes = []
    multiply_matrix4_vector3.restype = triplet

# /Users/plewis/librat/src/4D_vectors.h: 15
if _libs["libratlib.so"].has("vector_copy4", "cdecl"):
    vector_copy4 = _libs["libratlib.so"].get("vector_copy4", "cdecl")
    vector_copy4.argtypes = []
    vector_copy4.restype = Vector4

# /Users/plewis/librat/src/4D_vectors.h: 15
if _libs["libratlib.so"].has("multiply_matrix4_vector4", "cdecl"):
    multiply_matrix4_vector4 = _libs["libratlib.so"].get("multiply_matrix4_vector4", "cdecl")
    multiply_matrix4_vector4.argtypes = []
    multiply_matrix4_vector4.restype = Vector4

# /Users/plewis/librat/src/4D_vectors.h: 16
if _libs["libratlib.so"].has("load_identity_matrix4", "cdecl"):
    load_identity_matrix4 = _libs["libratlib.so"].get("load_identity_matrix4", "cdecl")
    load_identity_matrix4.argtypes = []
    load_identity_matrix4.restype = Matrix4

# /Users/plewis/librat/src/4D_vectors.h: 16
if _libs["libratlib.so"].has("load_translation_matrix4", "cdecl"):
    load_translation_matrix4 = _libs["libratlib.so"].get("load_translation_matrix4", "cdecl")
    load_translation_matrix4.argtypes = []
    load_translation_matrix4.restype = Matrix4

# /Users/plewis/librat/src/4D_vectors.h: 16
if _libs["libratlib.so"].has("load_scaling_matrix4", "cdecl"):
    load_scaling_matrix4 = _libs["libratlib.so"].get("load_scaling_matrix4", "cdecl")
    load_scaling_matrix4.argtypes = []
    load_scaling_matrix4.restype = Matrix4

# /Users/plewis/librat/src/4D_vectors.h: 16
if _libs["libratlib.so"].has("load_differential_scaling_matrix4", "cdecl"):
    load_differential_scaling_matrix4 = _libs["libratlib.so"].get("load_differential_scaling_matrix4", "cdecl")
    load_differential_scaling_matrix4.argtypes = []
    load_differential_scaling_matrix4.restype = Matrix4

# /Users/plewis/librat/src/4D_vectors.h: 16
if _libs["libratlib.so"].has("load_x_axis_rotation_matrix4", "cdecl"):
    load_x_axis_rotation_matrix4 = _libs["libratlib.so"].get("load_x_axis_rotation_matrix4", "cdecl")
    load_x_axis_rotation_matrix4.argtypes = []
    load_x_axis_rotation_matrix4.restype = Matrix4

# /Users/plewis/librat/src/4D_vectors.h: 16
if _libs["libratlib.so"].has("load_y_axis_rotation_matrix4", "cdecl"):
    load_y_axis_rotation_matrix4 = _libs["libratlib.so"].get("load_y_axis_rotation_matrix4", "cdecl")
    load_y_axis_rotation_matrix4.argtypes = []
    load_y_axis_rotation_matrix4.restype = Matrix4

# /Users/plewis/librat/src/4D_vectors.h: 16
if _libs["libratlib.so"].has("load_z_axis_rotation_matrix4", "cdecl"):
    load_z_axis_rotation_matrix4 = _libs["libratlib.so"].get("load_z_axis_rotation_matrix4", "cdecl")
    load_z_axis_rotation_matrix4.argtypes = []
    load_z_axis_rotation_matrix4.restype = Matrix4

# /Users/plewis/librat/src/4D_vectors.h: 16
if _libs["libratlib.so"].has("load_scaling_fix_point_matrix4", "cdecl"):
    load_scaling_fix_point_matrix4 = _libs["libratlib.so"].get("load_scaling_fix_point_matrix4", "cdecl")
    load_scaling_fix_point_matrix4.argtypes = []
    load_scaling_fix_point_matrix4.restype = Matrix4

# /Users/plewis/librat/src/4D_vectors.h: 16
if _libs["libratlib.so"].has("load_differential_scaling_fix_point_matrix4", "cdecl"):
    load_differential_scaling_fix_point_matrix4 = _libs["libratlib.so"].get("load_differential_scaling_fix_point_matrix4", "cdecl")
    load_differential_scaling_fix_point_matrix4.argtypes = []
    load_differential_scaling_fix_point_matrix4.restype = Matrix4

# /Users/plewis/librat/src/4D_vectors.h: 16
if _libs["libratlib.so"].has("load_x_axis_rotation_fix_point_matrix4", "cdecl"):
    load_x_axis_rotation_fix_point_matrix4 = _libs["libratlib.so"].get("load_x_axis_rotation_fix_point_matrix4", "cdecl")
    load_x_axis_rotation_fix_point_matrix4.argtypes = []
    load_x_axis_rotation_fix_point_matrix4.restype = Matrix4

# /Users/plewis/librat/src/4D_vectors.h: 16
if _libs["libratlib.so"].has("load_y_axis_rotation_fix_point_matrix4", "cdecl"):
    load_y_axis_rotation_fix_point_matrix4 = _libs["libratlib.so"].get("load_y_axis_rotation_fix_point_matrix4", "cdecl")
    load_y_axis_rotation_fix_point_matrix4.argtypes = []
    load_y_axis_rotation_fix_point_matrix4.restype = Matrix4

# /Users/plewis/librat/src/4D_vectors.h: 16
if _libs["libratlib.so"].has("load_z_axis_rotation_fix_point_matrix4", "cdecl"):
    load_z_axis_rotation_fix_point_matrix4 = _libs["libratlib.so"].get("load_z_axis_rotation_fix_point_matrix4", "cdecl")
    load_z_axis_rotation_fix_point_matrix4.argtypes = []
    load_z_axis_rotation_fix_point_matrix4.restype = Matrix4

# /Users/plewis/librat/src/4D_vectors.h: 16
if _libs["libratlib.so"].has("multiply_matrix4_matrix4", "cdecl"):
    multiply_matrix4_matrix4 = _libs["libratlib.so"].get("multiply_matrix4_matrix4", "cdecl")
    multiply_matrix4_matrix4.argtypes = []
    multiply_matrix4_matrix4.restype = Matrix4

# /Users/plewis/librat/src/4D_vectors.h: 16
if _libs["libratlib.so"].has("transpose4", "cdecl"):
    transpose4 = _libs["libratlib.so"].get("transpose4", "cdecl")
    transpose4.argtypes = []
    transpose4.restype = Matrix4

# /Users/plewis/librat/src/4D_vectors.h: 16
if _libs["libratlib.so"].has("inverse_matrix4", "cdecl"):
    inverse_matrix4 = _libs["libratlib.so"].get("inverse_matrix4", "cdecl")
    inverse_matrix4.argtypes = []
    inverse_matrix4.restype = Matrix4

# /Users/plewis/librat/src/4D_vectors.h: 16
if _libs["libratlib.so"].has("read_matrix4", "cdecl"):
    read_matrix4 = _libs["libratlib.so"].get("read_matrix4", "cdecl")
    read_matrix4.argtypes = []
    read_matrix4.restype = Matrix4

# /Users/plewis/librat/src/4D_vectors.h: 16
if _libs["libratlib.so"].has("load_arbitrary_axis_rotation_matrix4", "cdecl"):
    load_arbitrary_axis_rotation_matrix4 = _libs["libratlib.so"].get("load_arbitrary_axis_rotation_matrix4", "cdecl")
    load_arbitrary_axis_rotation_matrix4.argtypes = []
    load_arbitrary_axis_rotation_matrix4.restype = Matrix4

# /Users/plewis/librat/src/2D_vectors.h: 10
if _libs["libratlib.so"].has("same_sign", "cdecl"):
    same_sign = _libs["libratlib.so"].get("same_sign", "cdecl")
    same_sign.argtypes = []
    same_sign.restype = pair

# /Users/plewis/librat/src/2D_vectors.h: 10
if _libs["libratlib.so"].has("fabs_2D", "cdecl"):
    fabs_2D = _libs["libratlib.so"].get("fabs_2D", "cdecl")
    fabs_2D.argtypes = []
    fabs_2D.restype = pair

# /Users/plewis/librat/src/2D_vectors.h: 10
if _libs["libratlib.so"].has("copy_2D", "cdecl"):
    copy_2D = _libs["libratlib.so"].get("copy_2D", "cdecl")
    copy_2D.argtypes = []
    copy_2D.restype = pair

# /Users/plewis/librat/src/2D_vectors.h: 10
if _libs["libratlib.so"].has("bbox", "cdecl"):
    bbox = _libs["libratlib.so"].get("bbox", "cdecl")
    bbox.argtypes = []
    bbox.restype = POINTER(pair)

# /Users/plewis/librat/src/2D_vectors.h: 10
if _libs["libratlib.so"].has("plus_2D", "cdecl"):
    plus_2D = _libs["libratlib.so"].get("plus_2D", "cdecl")
    plus_2D.argtypes = []
    plus_2D.restype = pair

# /Users/plewis/librat/src/2D_vectors.h: 10
if _libs["libratlib.so"].has("factor_2D", "cdecl"):
    factor_2D = _libs["libratlib.so"].get("factor_2D", "cdecl")
    factor_2D.argtypes = []
    factor_2D.restype = pair

# /Users/plewis/librat/src/2D_vectors.h: 10
if _libs["libratlib.so"].has("multiply_2D", "cdecl"):
    multiply_2D = _libs["libratlib.so"].get("multiply_2D", "cdecl")
    multiply_2D.argtypes = []
    multiply_2D.restype = pair

# /Users/plewis/librat/src/2D_vectors.h: 10
if _libs["libratlib.so"].has("divide_2D", "cdecl"):
    divide_2D = _libs["libratlib.so"].get("divide_2D", "cdecl")
    divide_2D.argtypes = []
    divide_2D.restype = pair

# /Users/plewis/librat/src/2D_vectors.h: 10
if _libs["libratlib.so"].has("minus_2D", "cdecl"):
    minus_2D = _libs["libratlib.so"].get("minus_2D", "cdecl")
    minus_2D.argtypes = []
    minus_2D.restype = pair

# /Users/plewis/librat/src/2D_vectors.h: 11
if _libs["libratlib.so"].has("compare_2D", "cdecl"):
    compare_2D = _libs["libratlib.so"].get("compare_2D", "cdecl")
    compare_2D.argtypes = []
    compare_2D.restype = c_int

# /Users/plewis/librat/src/2D_vectors.h: 12
if _libs["libratlib.so"].has("factor_2D", "cdecl"):
    factor_2D = _libs["libratlib.so"].get("factor_2D", "cdecl")
    factor_2D.argtypes = []
    factor_2D.restype = pair

# /Users/plewis/librat/src/2D_vectors.h: 13
if _libs["libratlib.so"].has("factor_matrix_2D", "cdecl"):
    factor_matrix_2D = _libs["libratlib.so"].get("factor_matrix_2D", "cdecl")
    factor_matrix_2D.argtypes = []
    factor_matrix_2D.restype = matrix_2D

# /Users/plewis/librat/src/2D_vectors.h: 13
if _libs["libratlib.so"].has("inverse_2D", "cdecl"):
    inverse_2D = _libs["libratlib.so"].get("inverse_2D", "cdecl")
    inverse_2D.argtypes = []
    inverse_2D.restype = matrix_2D

# /Users/plewis/librat/src/2D_vectors.h: 13
if _libs["libratlib.so"].has("transpose_2D", "cdecl"):
    transpose_2D = _libs["libratlib.so"].get("transpose_2D", "cdecl")
    transpose_2D.argtypes = []
    transpose_2D.restype = matrix_2D

# /Users/plewis/librat/src/2D_vectors.h: 14
if _libs["libratlib.so"].has("normalise_2D", "cdecl"):
    normalise_2D = _libs["libratlib.so"].get("normalise_2D", "cdecl")
    normalise_2D.argtypes = []
    normalise_2D.restype = pair

# /Users/plewis/librat/src/2D_vectors.h: 14
if _libs["libratlib.so"].has("matrix_mult_2D", "cdecl"):
    matrix_mult_2D = _libs["libratlib.so"].get("matrix_mult_2D", "cdecl")
    matrix_mult_2D.argtypes = []
    matrix_mult_2D.restype = pair

# /Users/plewis/librat/src/2D_vectors.h: 15
if _libs["libratlib.so"].has("icopy_2D", "cdecl"):
    icopy_2D = _libs["libratlib.so"].get("icopy_2D", "cdecl")
    icopy_2D.argtypes = []
    icopy_2D.restype = ipair

# /Users/plewis/librat/src/2D_vectors.h: 15
if _libs["libratlib.so"].has("iplus_2D", "cdecl"):
    iplus_2D = _libs["libratlib.so"].get("iplus_2D", "cdecl")
    iplus_2D.argtypes = []
    iplus_2D.restype = ipair

# /Users/plewis/librat/src/2D_vectors.h: 15
if _libs["libratlib.so"].has("f2i_2D", "cdecl"):
    f2i_2D = _libs["libratlib.so"].get("f2i_2D", "cdecl")
    f2i_2D.argtypes = []
    f2i_2D.restype = ipair

# /Users/plewis/librat/src/2D_vectors.h: 16
if _libs["libratlib.so"].has("i2f_2D", "cdecl"):
    i2f_2D = _libs["libratlib.so"].get("i2f_2D", "cdecl")
    i2f_2D.argtypes = []
    i2f_2D.restype = pair

# /Users/plewis/librat/src/2D_vectors.h: 16
if _libs["libratlib.so"].has("lo_allign_to_grid", "cdecl"):
    lo_allign_to_grid = _libs["libratlib.so"].get("lo_allign_to_grid", "cdecl")
    lo_allign_to_grid.argtypes = []
    lo_allign_to_grid.restype = pair

# /Users/plewis/librat/src/2D_vectors.h: 16
if _libs["libratlib.so"].has("hi_allign_to_grid", "cdecl"):
    hi_allign_to_grid = _libs["libratlib.so"].get("hi_allign_to_grid", "cdecl")
    hi_allign_to_grid.argtypes = []
    hi_allign_to_grid.restype = pair

# /Users/plewis/librat/src/2D_vectors.h: 16
if _libs["libratlib.so"].has("line_intersection_2D", "cdecl"):
    line_intersection_2D = _libs["libratlib.so"].get("line_intersection_2D", "cdecl")
    line_intersection_2D.argtypes = []
    line_intersection_2D.restype = pair

# /Users/plewis/librat/src/2D_vectors.h: 17
if _libs["libratlib.so"].has("mod_2D", "cdecl"):
    mod_2D = _libs["libratlib.so"].get("mod_2D", "cdecl")
    mod_2D.argtypes = []
    mod_2D.restype = c_double

# /Users/plewis/librat/src/2D_vectors.h: 17
if _libs["libratlib.so"].has("add_2D", "cdecl"):
    add_2D = _libs["libratlib.so"].get("add_2D", "cdecl")
    add_2D.argtypes = []
    add_2D.restype = c_double

# /Users/plewis/librat/src/2D_vectors.h: 17
if _libs["libratlib.so"].has("line_distance_2D", "cdecl"):
    line_distance_2D = _libs["libratlib.so"].get("line_distance_2D", "cdecl")
    line_distance_2D.argtypes = []
    line_distance_2D.restype = c_double

# /Users/plewis/librat/src/2D_vectors.h: 18
if _libs["libratlib.so"].has("quick_affine", "cdecl"):
    quick_affine = _libs["libratlib.so"].get("quick_affine", "cdecl")
    quick_affine.argtypes = []
    quick_affine.restype = triangle

# /Users/plewis/librat/src/2D_vectors.h: 19
if _libs["libratlib.so"].has("affine_transform", "cdecl"):
    affine_transform = _libs["libratlib.so"].get("affine_transform", "cdecl")
    affine_transform.argtypes = []
    affine_transform.restype = pair

# /Users/plewis/librat/src/2D_vectors.h: 19
if _libs["libratlib.so"].has("backwards_affine_transform", "cdecl"):
    backwards_affine_transform = _libs["libratlib.so"].get("backwards_affine_transform", "cdecl")
    backwards_affine_transform.argtypes = []
    backwards_affine_transform.restype = pair

# /Users/plewis/librat/src/wavefront.h: 18
class struct_anon_18(Structure):
    pass

struct_anon_18.__slots__ = [
    'vertex',
]
struct_anon_18._fields_ = [
    ('vertex', c_int * int(3)),
]

Ivect3 = struct_anon_18# /Users/plewis/librat/src/wavefront.h: 18

# /Users/plewis/librat/src/wavefront.h: 21
class struct_anon_19(Structure):
    pass

struct_anon_19.__slots__ = [
    'vertex',
]
struct_anon_19._fields_ = [
    ('vertex', c_int * int(16)),
]

Bezier_patch = struct_anon_19# /Users/plewis/librat/src/wavefront.h: 21

# /Users/plewis/librat/src/wavefront.h: 27
class struct_anon_20(Structure):
    pass

struct_anon_20.__slots__ = [
    'new',
    'block',
    'no_of_vertex_numbers',
    'triangles',
]
struct_anon_20._fields_ = [
    ('new', c_int),
    ('block', c_int),
    ('no_of_vertex_numbers', c_int),
    ('triangles', POINTER(Ivect3) * int(200)),
]

Plant_triangles = struct_anon_20# /Users/plewis/librat/src/wavefront.h: 27

# /Users/plewis/librat/src/wavefront.h: 33
class struct_anon_21(Structure):
    pass

struct_anon_21.__slots__ = [
    'new',
    'block',
    'no_of_vertex_numbers',
]
struct_anon_21._fields_ = [
    ('new', c_int),
    ('block', c_int),
    ('no_of_vertex_numbers', c_int),
]

Plant_bzp = struct_anon_21# /Users/plewis/librat/src/wavefront.h: 33

# /Users/plewis/librat/src/wavefront.h: 40
class struct_anon_22(Structure):
    pass

struct_anon_22.__slots__ = [
    'leaf_area',
    'projected_leaf_area',
    'zeniths',
    'look_zeniths',
    'azimuths',
]
struct_anon_22._fields_ = [
    ('leaf_area', c_double),
    ('projected_leaf_area', c_double),
    ('zeniths', POINTER(c_int)),
    ('look_zeniths', POINTER(c_int)),
    ('azimuths', POINTER(c_int)),
]

Plant_area = struct_anon_22# /Users/plewis/librat/src/wavefront.h: 40

# /Users/plewis/librat/src/wavefront.h: 51
class struct_anon_23(Structure):
    pass

struct_anon_23.__slots__ = [
    'in',
    'blocksize',
    'bbox_min',
    'bbox_max',
    'plant_triangles',
    'plant_bzp',
    'plant_area',
]
struct_anon_23._fields_ = [
    ('in', c_int),
    ('blocksize', c_int),
    ('bbox_min', triplet),
    ('bbox_max', triplet),
    ('plant_triangles', Plant_triangles),
    ('plant_bzp', Plant_bzp),
    ('plant_area', Plant_area),
]

Plant = struct_anon_23# /Users/plewis/librat/src/wavefront.h: 51

# /Users/plewis/librat/src/wavefront.h: 61
class struct_anon_24(Structure):
    pass

struct_anon_24.__slots__ = [
    'in',
    'new',
    'displacement',
    'plant_number',
    'plant_area',
]
struct_anon_24._fields_ = [
    ('in', c_int),
    ('new', c_int),
    ('displacement', triplet),
    ('plant_number', c_int),
    ('plant_area', Plant_area),
]

Clone = struct_anon_24# /Users/plewis/librat/src/wavefront.h: 61

# /Users/plewis/librat/src/wavefront.h: 70
class struct_anon_25(Structure):
    pass

struct_anon_25.__slots__ = [
    'new',
    'block',
    'no_of_vertices',
    'blocksize',
    'vertices',
]
struct_anon_25._fields_ = [
    ('new', c_int * int(1000)),
    ('block', c_int),
    ('no_of_vertices', c_int),
    ('blocksize', c_int),
    ('vertices', POINTER(triplet) * int(1000)),
]

VeRtIcEs = struct_anon_25# /Users/plewis/librat/src/wavefront.h: 70

IVECT3 = Ivect3# /Users/plewis/librat/src/wavefront.h: 72

# /Users/plewis/librat/src/hips.h: 11
class struct_anon_26(Structure):
    pass

struct_anon_26.__slots__ = [
    'fdata',
    'bdata',
    'sdata',
    'idata',
]
struct_anon_26._fields_ = [
    ('fdata', POINTER(c_float)),
    ('bdata', POINTER(c_ubyte)),
    ('sdata', POINTER(c_short)),
    ('idata', POINTER(c_int)),
]

Data = struct_anon_26# /Users/plewis/librat/src/hips.h: 11

# /Users/plewis/librat/src/hips.h: 18
for _lib in _libs.values():
    try:
        imagename = (String).in_dll(_lib, "imagename")
        break
    except:
        pass

# /Users/plewis/librat/src/allocate.h: 4
if _libs["libratlib.so"].has("v_allocate", "cdecl"):
    v_allocate = _libs["libratlib.so"].get("v_allocate", "cdecl")
    v_allocate.argtypes = []
    v_allocate.restype = POINTER(c_ubyte)
    v_allocate.errcheck = lambda v,*a : cast(v, c_void_p)

# /Users/plewis/librat/src/allocate.h: 5
if _libs["libratlib.so"].has("t_allocate", "cdecl"):
    t_allocate = _libs["libratlib.so"].get("t_allocate", "cdecl")
    t_allocate.argtypes = []
    t_allocate.restype = POINTER(triplet)

# /Users/plewis/librat/src/allocate.h: 6
if _libs["libratlib.so"].has("d_allocate", "cdecl"):
    d_allocate = _libs["libratlib.so"].get("d_allocate", "cdecl")
    d_allocate.argtypes = []
    d_allocate.restype = POINTER(c_double)

# /Users/plewis/librat/src/allocate.h: 7
if _libs["libratlib.so"].has("i_allocate", "cdecl"):
    i_allocate = _libs["libratlib.so"].get("i_allocate", "cdecl")
    i_allocate.argtypes = []
    i_allocate.restype = POINTER(c_int)

# /Users/plewis/librat/src/allocate.h: 8
if _libs["libratlib.so"].has("D_allocate", "cdecl"):
    D_allocate = _libs["libratlib.so"].get("D_allocate", "cdecl")
    D_allocate.argtypes = []
    D_allocate.restype = Data

# /Users/plewis/librat/src/allocate.h: 9
if _libs["libratlib.so"].has("I_allocate", "cdecl"):
    I_allocate = _libs["libratlib.so"].get("I_allocate", "cdecl")
    I_allocate.argtypes = []
    I_allocate.restype = POINTER(Ivect3)

# /Users/plewis/librat/src/allocate.h: 10
if _libs["libratlib.so"].has("f_allocate", "cdecl"):
    f_allocate = _libs["libratlib.so"].get("f_allocate", "cdecl")
    f_allocate.argtypes = []
    f_allocate.restype = POINTER(c_float)

# /Users/plewis/librat/src/allocate.h: 11
if _libs["libratlib.so"].has("b_allocate", "cdecl"):
    b_allocate = _libs["libratlib.so"].get("b_allocate", "cdecl")
    b_allocate.argtypes = []
    if sizeof(c_int) == sizeof(c_void_p):
        b_allocate.restype = ReturnString
    else:
        b_allocate.restype = String
        b_allocate.errcheck = ReturnString

# /Users/plewis/librat/src/allocate.h: 12
if _libs["libratlib.so"].has("c_allocate", "cdecl"):
    c_allocate = _libs["libratlib.so"].get("c_allocate", "cdecl")
    c_allocate.argtypes = []
    if sizeof(c_int) == sizeof(c_void_p):
        c_allocate.restype = ReturnString
    else:
        c_allocate.restype = String
        c_allocate.errcheck = ReturnString

# /Users/plewis/librat/src/allocate.h: 13
if _libs["libratlib.so"].has("B_allocate", "cdecl"):
    B_allocate = _libs["libratlib.so"].get("B_allocate", "cdecl")
    B_allocate.argtypes = []
    B_allocate.restype = POINTER(Bezier_patch)

# /Users/plewis/librat/src/intersect_objects.h: 4
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

Contents_order = struct_Sort_Hit# /Users/plewis/librat/src/intersect_objects.h: 11

# ./rpv.h: 12
class struct_anon_27(Structure):
    pass

struct_anon_27.__slots__ = [
    'rho0',
    'k',
    'bigtheta',
    'rhoc',
]
struct_anon_27._fields_ = [
    ('rho0', c_double),
    ('k', c_double),
    ('bigtheta', c_double),
    ('rhoc', c_double),
]

Rpv = struct_anon_27# ./rpv.h: 12

# ./rpv.h: 14
if _libs["libratlib.so"].has("rpv", "cdecl"):
    rpv = _libs["libratlib.so"].get("rpv", "cdecl")
    rpv.argtypes = [c_double, c_double, c_double, c_double, c_double, c_double, c_double]
    rpv.restype = c_double

# /Users/plewis/librat/src/materials.h: 7
class struct_anon_28(Structure):
    pass

struct_anon_28.__slots__ = [
    'data',
    'hd',
]
struct_anon_28._fields_ = [
    ('data', Data),
    ('hd', struct_header),
]

ImageMap = struct_anon_28# /Users/plewis/librat/src/materials.h: 7

# /Users/plewis/librat/src/materials.h: 9
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

Material_table = struct_Material_table_struct# /Users/plewis/librat/src/materials.h: 27

# /Users/plewis/librat/src/prat_objects.h: 21
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

Plane = struct_Plane_struct# /Users/plewis/librat/src/prat_objects.h: 28

# /Users/plewis/librat/src/prat_objects.h: 34
class struct_anon_29(Structure):
    pass

struct_anon_29.__slots__ = [
    'vertex_normal',
    'normal_du',
    'normal_dv',
]
struct_anon_29._fields_ = [
    ('vertex_normal', triplet),
    ('normal_du', triplet),
    ('normal_dv', triplet),
]

Vertex_normals = struct_anon_29# /Users/plewis/librat/src/prat_objects.h: 34

# /Users/plewis/librat/src/prat_objects.h: 43
class struct_anon_30(Structure):
    pass

struct_anon_30.__slots__ = [
    'vertex_local',
    'local_du',
    'local_dv',
    'basis',
    'du_length',
    'dv_length',
]
struct_anon_30._fields_ = [
    ('vertex_local', pair),
    ('local_du', pair),
    ('local_dv', pair),
    ('basis', triplet * int(2)),
    ('du_length', c_double),
    ('dv_length', c_double),
]

Vertex_locals = struct_anon_30# /Users/plewis/librat/src/prat_objects.h: 43

# /Users/plewis/librat/src/prat_objects.h: 54
class struct_anon_31(Structure):
    pass

struct_anon_31.__slots__ = [
    'orientation',
    'g',
    'dw',
    'normal',
    'material',
]
struct_anon_31._fields_ = [
    ('orientation', c_char),
    ('g', pair * int(3)),
    ('dw', c_double),
    ('normal', triplet),
    ('material', POINTER(Material_table)),
]

Facet_quick = struct_anon_31# /Users/plewis/librat/src/prat_objects.h: 54

# /Users/plewis/librat/src/prat_objects.h: 58
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
    ('base', Pair),
    ('store', c_double * int(4)),
    ('dw', c_double),
    ('normal', triplet),
    ('vertex_normals', POINTER(Vertex_normals)),
    ('local_coords', POINTER(Vertex_locals)),
    ('material', POINTER(Material_table)),
    ('next_facet', POINTER(struct_D_Facet_struct)),
]

D_Facet = struct_D_Facet_struct# /Users/plewis/librat/src/prat_objects.h: 70

# /Users/plewis/librat/src/prat_objects.h: 72
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

Disk = struct_Disk_struct# /Users/plewis/librat/src/prat_objects.h: 81

# /Users/plewis/librat/src/prat_objects.h: 85
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

BiLinearPatch = struct_BiLinearPatch_struct# /Users/plewis/librat/src/prat_objects.h: 97

# /Users/plewis/librat/src/prat_objects.h: 101
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

Cylinder = struct_Cylinder_struct# /Users/plewis/librat/src/prat_objects.h: 114

# /Users/plewis/librat/src/prat_objects.h: 119
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

Ellipse = struct_Ellipse_struct# /Users/plewis/librat/src/prat_objects.h: 128

# /Users/plewis/librat/src/prat_objects.h: 134
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

Spheroid = struct_Spheroid_struct# /Users/plewis/librat/src/prat_objects.h: 167

# /Users/plewis/librat/src/prat_objects.h: 180
class struct_anon_32(Structure):
    pass

struct_anon_32.__slots__ = [
    'offset',
    'extent',
    'inv_extent',
    'bbox_max',
    'bbox_min',
    'direction',
]
struct_anon_32._fields_ = [
    ('offset', triplet),
    ('extent', triplet),
    ('inv_extent', triplet),
    ('bbox_max', triplet),
    ('bbox_min', triplet),
    ('direction', String),
]

Dem_Attributes = struct_anon_32# /Users/plewis/librat/src/prat_objects.h: 180

# /Users/plewis/librat/src/prat_objects.h: 189
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

Sphere = struct_Sphere_struct# /Users/plewis/librat/src/prat_objects.h: 198

# /Users/plewis/librat/src/prat_objects.h: 200
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

D_Sphere = struct_D_Sphere_struct# /Users/plewis/librat/src/prat_objects.h: 208

# /Users/plewis/librat/src/prat_objects.h: 216
class struct_anon_33(Structure):
    pass

struct_anon_33.__slots__ = [
    'spherical_cell_bounds_flag',
    'spherical_cell_bounds',
    'extent',
    'inv_extent',
    'offset',
]
struct_anon_33._fields_ = [
    ('spherical_cell_bounds_flag', c_int),
    ('spherical_cell_bounds', D_Facet * int(4)),
    ('extent', triplet),
    ('inv_extent', triplet),
    ('offset', triplet),
]

Partial_Spherical_Data = struct_anon_33# /Users/plewis/librat/src/prat_objects.h: 216

# /Users/plewis/librat/src/prat_objects.h: 222
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

Spherical_Dem = struct_Spherical_Dem_struct# /Users/plewis/librat/src/prat_objects.h: 233

# /Users/plewis/librat/src/prat_objects.h: 245
class struct_anon_34(Structure):
    pass

struct_anon_34.__slots__ = [
    'origin',
    'direction',
    'ray_length',
    'sourceOfRay',
    'lengthOfRaySoFar',
    'rayLengthThroughObject',
]
struct_anon_34._fields_ = [
    ('origin', triplet),
    ('direction', triplet),
    ('ray_length', c_double),
    ('sourceOfRay', c_int),
    ('lengthOfRaySoFar', c_double),
    ('rayLengthThroughObject', c_double),
]

Ray = struct_anon_34# /Users/plewis/librat/src/prat_objects.h: 245

# /Users/plewis/librat/src/prat_objects.h: 251
class struct_anon_35(Structure):
    pass

struct_anon_35.__slots__ = [
    'origin',
    'direction',
    'ray_length',
]
struct_anon_35._fields_ = [
    ('origin', triplet),
    ('direction', triplet),
    ('ray_length', c_double),
]

D_Ray = struct_anon_35# /Users/plewis/librat/src/prat_objects.h: 251

# /Users/plewis/librat/src/prat_objects.h: 256
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

Bezier4 = struct_Bezier4_struct# /Users/plewis/librat/src/prat_objects.h: 263

# /Users/plewis/librat/src/prat_objects.h: 265
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

fFacet = struct_Facet_struct# /Users/plewis/librat/src/prat_objects.h: 280

# /Users/plewis/librat/src/prat_objects.h: 282
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

Dem = struct_Dem_struct# /Users/plewis/librat/src/prat_objects.h: 294

# /Users/plewis/librat/src/prat_objects.h: 342
class struct_Bbox(Structure):
    pass

# /Users/plewis/librat/src/prat_objects.h: 360
class struct_Clone(Structure):
    pass

# /Users/plewis/librat/src/prat_objects.h: 297
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
    ('hit', POINTER(Contents_order) * int(7)),
    ('transform_flag', c_char),
    ('define_flag', c_char),
    ('m', Matrix4),
    ('m_inverse', Matrix4),
    ('m_inverse_transpose', Matrix4),
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

BBox = struct_Bbox# /Users/plewis/librat/src/prat_objects.h: 409

Contents = struct_Content# /Users/plewis/librat/src/prat_objects.h: 410

Clones = struct_Clone# /Users/plewis/librat/src/prat_objects.h: 411

# /Users/plewis/librat/src/prat_objects.h: 423
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

Group = struct_anon_36# /Users/plewis/librat/src/prat_objects.h: 423

# /Users/plewis/librat/src/prat_objects.h: 459
class struct_anon_37(Structure):
    pass

struct_anon_37.__slots__ = [
    'RayLength',
    'intersection',
    'real_intersection',
    'normal',
    'matrix',
    'm',
    'TriangleObj',
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
    ('m', Matrix4),
    ('TriangleObj', POINTER(fFacet)),
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

ObjectList = struct_anon_37# /Users/plewis/librat/src/prat_objects.h: 459

# /Users/plewis/librat/src/prat_objects.h: 468
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

PixelCondition = struct_anon_38# /Users/plewis/librat/src/prat_objects.h: 468

# /Users/plewis/librat/src/prat_objects.h: 473
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

PixelVarianceLimits = struct_anon_39# /Users/plewis/librat/src/prat_objects.h: 473

# /Users/plewis/librat/src/prat_objects.h: 479
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

Box = struct_BOX# /Users/plewis/librat/src/prat_objects.h: 485

# /Users/plewis/librat/src/prat_objects.h: 514
class struct_anon_40(Structure):
    pass

struct_anon_40.__slots__ = [
    'location',
    'localNormal',
    'fromLocation',
    'fromVector',
    'toVector',
    'fromVectorLength',
    'interactionType',
    'nSuns',
    'hitSun',
    'sunVector',
    'sunInteractionType',
    'lambertian',
    'distanceBackToViewer',
    'hitSky',
    'skyVector',
    'material',
    'materialIndex',
]
struct_anon_40._fields_ = [
    ('location', triplet),
    ('localNormal', triplet),
    ('fromLocation', triplet),
    ('fromVector', triplet),
    ('toVector', triplet),
    ('fromVectorLength', c_double),
    ('interactionType', c_int),
    ('nSuns', c_int),
    ('hitSun', POINTER(c_int)),
    ('sunVector', POINTER(triplet)),
    ('sunInteractionType', POINTER(c_int)),
    ('lambertian', POINTER(c_double)),
    ('distanceBackToViewer', POINTER(c_double)),
    ('hitSky', c_int),
    ('skyVector', triplet),
    ('material', POINTER(Material_table)),
    ('materialIndex', c_int),
]

HitPoint = struct_anon_40# /Users/plewis/librat/src/prat_objects.h: 514

# /Users/plewis/librat/src/prat_objects.h: 530
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

PStats = struct_anon_41# /Users/plewis/librat/src/prat_objects.h: 530

# /Users/plewis/librat/src/prat_objects.h: 540
for _lib in _libs.values():
    try:
        doTestFrame = (c_int).in_dll(_lib, "doTestFrame")
        break
    except:
        pass

# /Users/plewis/librat/src/prat_objects.h: 541
for _lib in _libs.values():
    try:
        whichTestFrame = (c_int).in_dll(_lib, "whichTestFrame")
        break
    except:
        pass

# /Users/plewis/librat/src/prat_objects.h: 543
for _lib in _libs.values():
    try:
        nBins = (c_int).in_dll(_lib, "nBins")
        break
    except:
        pass

# /Users/plewis/librat/src/prat_objects.h: 543
for _lib in _libs.values():
    try:
        nBands = (c_int).in_dll(_lib, "nBands")
        break
    except:
        pass

# /Users/plewis/librat/src/prat_objects.h: 543
for _lib in _libs.values():
    try:
        nRPP = (c_int).in_dll(_lib, "nRPP")
        break
    except:
        pass

# /Users/plewis/librat/src/prat_objects.h: 544
for _lib in _libs.values():
    try:
        argc = (c_int).in_dll(_lib, "argc")
        break
    except:
        pass

# /Users/plewis/librat/src/prat_objects.h: 545
for _lib in _libs.values():
    try:
        argv = (POINTER(POINTER(c_char))).in_dll(_lib, "argv")
        break
    except:
        pass

# /Users/plewis/librat/src/sensor_rsr.h: 9
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

Spectral_Data = struct_anon_42# /Users/plewis/librat/src/sensor_rsr.h: 9

# /Users/plewis/librat/src/sensor_rsr.h: 15
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

Sensor_Wavebands = struct_anon_43# /Users/plewis/librat/src/sensor_rsr.h: 15

# /Users/plewis/librat/src/camera.h: 30
class struct_anon_44(Structure):
    pass

struct_anon_44.__slots__ = [
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
struct_anon_44._fields_ = [
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

Camera_ip = struct_anon_44# /Users/plewis/librat/src/camera.h: 30

# /Users/plewis/librat/src/camera.h: 54
class struct_anon_45(Structure):
    pass

struct_anon_45.__slots__ = [
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
struct_anon_45._fields_ = [
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

Camera_op = struct_anon_45# /Users/plewis/librat/src/camera.h: 54

# /Users/plewis/librat/src/camera.h: 63
class struct_anon_46(Structure):
    pass

struct_anon_46.__slots__ = [
    'restart',
    'from',
    'to',
    'from_sample',
    'to_sample',
    'recover',
]
struct_anon_46._fields_ = [
    ('restart', c_int),
    ('from', c_int),
    ('to', c_int),
    ('from_sample', c_int),
    ('to_sample', c_int),
    ('recover', c_int),
]

Restart = struct_anon_46# /Users/plewis/librat/src/camera.h: 63

# /Users/plewis/librat/src/reflectance.h: 9
class struct_anon_47(Structure):
    pass

struct_anon_47.__slots__ = [
    'diffuse_reflectance',
    'diffuse_transmittance',
    'proportion',
    'diffuse_reflectance_flag',
    'diffuse_transmittance_flag',
]
struct_anon_47._fields_ = [
    ('diffuse_reflectance', POINTER(c_double)),
    ('diffuse_transmittance', POINTER(c_double)),
    ('proportion', c_double),
    ('diffuse_reflectance_flag', c_int),
    ('diffuse_transmittance_flag', c_int),
]

Standard_Reflectance_Model = struct_anon_47# /Users/plewis/librat/src/reflectance.h: 9

# /Users/plewis/librat/src/reflectance.h: 23
if _libs["libratlib.so"].has("apply_local_coordinates", "cdecl"):
    apply_local_coordinates = _libs["libratlib.so"].get("apply_local_coordinates", "cdecl")
    apply_local_coordinates.argtypes = []
    apply_local_coordinates.restype = POINTER(Material_table)

# /Users/plewis/librat/src/volume.h: 23
class struct_anon_48(Structure):
    pass

struct_anon_48.__slots__ = [
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
struct_anon_48._fields_ = [
    ('uL', c_double),
    ('l', c_double),
    ('lad', CFUNCTYPE(UNCHECKED(triplet), )),
    ('material', c_int),
    ('s', triplet),
    ('m', c_double),
    ('G', c_double),
    ('Glut', POINTER(c_double)),
    ('nSamples', c_int),
    ('VOLUME_nZeniths', c_int),
    ('VOLUME_nZenithsFreq', POINTER(c_float)),
]

Volume = struct_anon_48# /Users/plewis/librat/src/volume.h: 23

# /Users/plewis/librat/src/mtllib.h: 16
class struct_anon_49(Structure):
    pass

struct_anon_49.__slots__ = [
    'no_of_samples',
    'wavelength',
    'srm',
    'volume',
]
struct_anon_49._fields_ = [
    ('no_of_samples', c_int),
    ('wavelength', POINTER(c_double)),
    ('srm', Standard_Reflectance_Model),
    ('volume', Volume),
]

Standard_Material_List = struct_anon_49# /Users/plewis/librat/src/mtllib.h: 16

# /Users/plewis/librat/src/reflectance_functions.h: 23
class struct_anon_50(Structure):
    pass

struct_anon_50.__slots__ = [
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
struct_anon_50._fields_ = [
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

Samples = struct_anon_50# /Users/plewis/librat/src/reflectance_functions.h: 23

# /Users/plewis/librat/src/bags.h: 6
for _lib in _libs.values():
    try:
        sun_magnitude = (POINTER(c_double)).in_dll(_lib, "sun_magnitude")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 7
for _lib in _libs.values():
    try:
        sky_black = (c_int).in_dll(_lib, "sky_black")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 8
for _lib in _libs.values():
    try:
        sun_Ptr = (POINTER(triplet)).in_dll(_lib, "sun_Ptr")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 9
for _lib in _libs.values():
    try:
        sky_flag = (c_int).in_dll(_lib, "sky_flag")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 10
for _lib in _libs.values():
    try:
        sun_fov = (c_double).in_dll(_lib, "sun_fov")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 11
for _lib in _libs.values():
    try:
        theta_sun = (c_double).in_dll(_lib, "theta_sun")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 12
for _lib in _libs.values():
    try:
        phi_gap = (c_double).in_dll(_lib, "phi_gap")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 13
for _lib in _libs.values():
    try:
        sun_elevation_axis = (triplet).in_dll(_lib, "sun_elevation_axis")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 14
for _lib in _libs.values():
    try:
        direct_file = (String).in_dll(_lib, "direct_file")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 15
for _lib in _libs.values():
    try:
        position = (POINTER(Camera_op)).in_dll(_lib, "position")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 21
for _lib in _libs.values():
    try:
        name = (String).in_dll(_lib, "name")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 22
for _lib in _libs.values():
    try:
        camera_ip = (Camera_ip).in_dll(_lib, "camera_ip")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 23
for _lib in _libs.values():
    try:
        camera_op = (Camera_op).in_dll(_lib, "camera_op")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 60
for _lib in _libs.values():
    try:
        vertexStore = (c_int).in_dll(_lib, "vertexStore")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 61
for _lib in _libs.values():
    try:
        angleTol = (c_double).in_dll(_lib, "angleTol")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 61
for _lib in _libs.values():
    try:
        distanceTol = (c_double).in_dll(_lib, "distanceTol")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 61
for _lib in _libs.values():
    try:
        sizeTol = (c_double).in_dll(_lib, "sizeTol")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 63
for _lib in _libs.values():
    try:
        distanceStore = (POINTER(c_double)).in_dll(_lib, "distanceStore")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 64
for _lib in _libs.values():
    try:
        reflectanceStore = (POINTER(c_double)).in_dll(_lib, "reflectanceStore")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 65
for _lib in _libs.values():
    try:
        doInfinite = (c_int).in_dll(_lib, "doInfinite")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 66
for _lib in _libs.values():
    try:
        infiniteOffset = (triplet).in_dll(_lib, "infiniteOffset")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 67
for _lib in _libs.values():
    try:
        blackSky = (c_int).in_dll(_lib, "blackSky")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 68
for _lib in _libs.values():
    try:
        rtRatio = (c_double).in_dll(_lib, "rtRatio")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 69
for _lib in _libs.values():
    try:
        reverse = (c_int).in_dll(_lib, "reverse")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 70
for _lib in _libs.values():
    try:
        photonSpread = (c_int).in_dll(_lib, "photonSpread")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 71
for _lib in _libs.values():
    try:
        lidar = (c_int).in_dll(_lib, "lidar")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 72
for _lib in _libs.values():
    try:
        sz = (c_double).in_dll(_lib, "sz")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 73
for _lib in _libs.values():
    try:
        sunlocation = (POINTER(c_double)).in_dll(_lib, "sunlocation")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 74
for _lib in _libs.values():
    try:
        sunrayangle = (POINTER(c_double)).in_dll(_lib, "sunrayangle")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 75
for _lib in _libs.values():
    try:
        nSuns = (c_int).in_dll(_lib, "nSuns")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 76
for _lib in _libs.values():
    try:
        illuminationvector = (POINTER(triplet)).in_dll(_lib, "illuminationvector")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 77
for _lib in _libs.values():
    try:
        lengthToSun = (POINTER(c_double)).in_dll(_lib, "lengthToSun")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 78
for _lib in _libs.values():
    try:
        isSunTest = (c_int).in_dll(_lib, "isSunTest")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 79
for _lib in _libs.values():
    try:
        rowsandcols = (POINTER(c_int)).in_dll(_lib, "rowsandcols")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 93
class struct_anon_51(Structure):
    pass

struct_anon_51.__slots__ = [
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
struct_anon_51._fields_ = [
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

WavebandBag = struct_anon_51# /Users/plewis/librat/src/bags.h: 93

# /Users/plewis/librat/src/bags.h: 98
for _lib in _libs.values():
    try:
        samples = (POINTER(Samples)).in_dll(_lib, "samples")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 99
for _lib in _libs.values():
    try:
        material_lut = (POINTER(Standard_Material_List)).in_dll(_lib, "material_lut")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 100
for _lib in _libs.values():
    try:
        material_names = (POINTER(POINTER(c_char))).in_dll(_lib, "material_names")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 126
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

RATmaterials = struct_anon_52# /Users/plewis/librat/src/bags.h: 126

# /Users/plewis/librat/src/bags.h: 142
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

RATtree = struct_anon_53# /Users/plewis/librat/src/bags.h: 142

# /Users/plewis/librat/src/bags.h: 149
class struct_anon_54(Structure):
    pass

struct_anon_54.__slots__ = [
    'nothing',
]
struct_anon_54._fields_ = [
    ('nothing', c_int),
]

RATvol = struct_anon_54# /Users/plewis/librat/src/bags.h: 149

# /Users/plewis/librat/src/bags.h: 158
for _lib in _libs.values():
    try:
        name = (String).in_dll(_lib, "name")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 180
for _lib in _libs.values():
    try:
        nd = (POINTER(c_float)).in_dll(_lib, "nd")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 181
for _lib in _libs.values():
    try:
        nsamps = (c_int * int(3)).in_dll(_lib, "nsamps")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 183
for _lib in _libs.values():
    try:
        otherStuff = (POINTER(None)).in_dll(_lib, "otherStuff")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 184
for _lib in _libs.values():
    try:
        rawfilename = (String).in_dll(_lib, "rawfilename")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 185
for _lib in _libs.values():
    try:
        datafilename = (String).in_dll(_lib, "datafilename")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 193
for _lib in _libs.values():
    try:
        wavebandbag = (POINTER(WavebandBag)).in_dll(_lib, "wavebandbag")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 195
for _lib in _libs.values():
    try:
        material_name = (POINTER(POINTER(c_char))).in_dll(_lib, "material_name")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 196
for _lib in _libs.values():
    try:
        material_table = (POINTER(Material_table)).in_dll(_lib, "material_table")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 197
for _lib in _libs.values():
    try:
        PRAT_MAX_MATERIALS = (c_int).in_dll(_lib, "PRAT_MAX_MATERIALS")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 198
for _lib in _libs.values():
    try:
        current_mtl = (POINTER(Material_table)).in_dll(_lib, "current_mtl")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 199
for _lib in _libs.values():
    try:
        m_inv_reverse = (Matrix4).in_dll(_lib, "m_inv_reverse")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 199
for _lib in _libs.values():
    try:
        m_inverse_fwd = (Matrix4).in_dll(_lib, "m_inverse_fwd")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 200
for _lib in _libs.values():
    try:
        max_number_of_groups = (c_int).in_dll(_lib, "max_number_of_groups")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 202
for _lib in _libs.values():
    try:
        samples = (Samples).in_dll(_lib, "samples")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 203
for _lib in _libs.values():
    try:
        group = (Group).in_dll(_lib, "group")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 204
for _lib in _libs.values():
    try:
        sensor_wavebands = (Sensor_Wavebands).in_dll(_lib, "sensor_wavebands")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 205
for _lib in _libs.values():
    try:
        timer = (c_int).in_dll(_lib, "timer")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 206
for _lib in _libs.values():
    try:
        level = (c_int).in_dll(_lib, "level")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 207
for _lib in _libs.values():
    try:
        vertices = (VeRtIcEs).in_dll(_lib, "vertices")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 207
for _lib in _libs.values():
    try:
        normals = (VeRtIcEs).in_dll(_lib, "normals")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 207
for _lib in _libs.values():
    try:
        locals = (VeRtIcEs).in_dll(_lib, "locals")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 208
try:
    bbox = (POINTER(BBox)).in_dll(_libs["libratlib.so"], "bbox")
except:
    pass

# /Users/plewis/librat/src/bags.h: 209
for _lib in _libs.values():
    try:
        mmap_flag = (c_int).in_dll(_lib, "mmap_flag")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 211
for _lib in _libs.values():
    try:
        lidar = (c_int).in_dll(_lib, "lidar")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 213
for _lib in _libs.values():
    try:
        globalArgc = (c_int).in_dll(_lib, "globalArgc")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 214
for _lib in _libs.values():
    try:
        globalArgv = (POINTER(POINTER(c_char))).in_dll(_lib, "globalArgv")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 218
for _lib in _libs.values():
    try:
        globalnBands = (c_int).in_dll(_lib, "globalnBands")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 219
for _lib in _libs.values():
    try:
        globalOver = (c_double).in_dll(_lib, "globalOver")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 220
for _lib in _libs.values():
    try:
        globalDoLidar = (c_int).in_dll(_lib, "globalDoLidar")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 221
for _lib in _libs.values():
    try:
        globalnRTD = (c_int).in_dll(_lib, "globalnRTD")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 223
for _lib in _libs.values():
    try:
        facets = (POINTER(fFacet)).in_dll(_lib, "facets")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 224
for _lib in _libs.values():
    try:
        nFacets = (c_int).in_dll(_lib, "nFacets")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 224
for _lib in _libs.values():
    try:
        whichFacet = (c_int).in_dll(_lib, "whichFacet")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 224
for _lib in _libs.values():
    try:
        nFacetBlock = (c_int).in_dll(_lib, "nFacetBlock")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 225
for _lib in _libs.values():
    try:
        ratTree = (POINTER(RATtree)).in_dll(_lib, "ratTree")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 226
for _lib in _libs.values():
    try:
        nSuns = (c_int).in_dll(_lib, "nSuns")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 229
for _lib in _libs.values():
    try:
        ratMat = (POINTER(RATmaterials)).in_dll(_lib, "ratMat")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 230
for _lib in _libs.values():
    try:
        nMaterials = (POINTER(c_int)).in_dll(_lib, "nMaterials")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 231
for _lib in _libs.values():
    try:
        ratMatUsed = (POINTER(RATmaterials)).in_dll(_lib, "ratMatUsed")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 232
for _lib in _libs.values():
    try:
        nMaterialsUsed = (POINTER(c_int)).in_dll(_lib, "nMaterialsUsed")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 233
for _lib in _libs.values():
    try:
        materialTranslatorLut = (POINTER(c_int)).in_dll(_lib, "materialTranslatorLut")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 236
for _lib in _libs.values():
    try:
        nObjectTypes = (c_int).in_dll(_lib, "nObjectTypes")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 237
for _lib in _libs.values():
    try:
        objectTypes = (POINTER(c_int)).in_dll(_lib, "objectTypes")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 238
for _lib in _libs.values():
    try:
        objectTypeCapacity = (c_int).in_dll(_lib, "objectTypeCapacity")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 239
for _lib in _libs.values():
    try:
        objectMaterialNames = (POINTER(POINTER(c_char))).in_dll(_lib, "objectMaterialNames")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 240
for _lib in _libs.values():
    try:
        minObjectSize = (POINTER(c_double)).in_dll(_lib, "minObjectSize")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 241
for _lib in _libs.values():
    try:
        maxObjectSize = (POINTER(c_double)).in_dll(_lib, "maxObjectSize")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 244
for _lib in _libs.values():
    try:
        ratvol = (POINTER(RATvol)).in_dll(_lib, "ratvol")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 248
for _lib in _libs.values():
    try:
        nratObjs = (c_int).in_dll(_lib, "nratObjs")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 249
for _lib in _libs.values():
    try:
        hit_camera = (c_int).in_dll(_lib, "hit_camera")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 251
for _lib in _libs.values():
    try:
        nstore = (c_int).in_dll(_lib, "nstore")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 252
for _lib in _libs.values():
    try:
        store = (POINTER(POINTER(None))).in_dll(_lib, "store")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 254
for _lib in _libs.values():
    try:
        Matrix = (c_double * int(16)).in_dll(_lib, "Matrix")
        break
    except:
        pass

# /Users/plewis/librat/src/bags.h: 264
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

CurrentReflectanceBag = struct_anon_55# /Users/plewis/librat/src/bags.h: 264

# /Users/plewis/librat/src/3D_vectors_double.h: 4
if _libs["libratlib.so"].has("v_mod", "cdecl"):
    v_mod = _libs["libratlib.so"].get("v_mod", "cdecl")
    v_mod.argtypes = []
    v_mod.restype = c_double

# /Users/plewis/librat/src/3D_vectors_double.h: 4
if _libs["libratlib.so"].has("v_dot", "cdecl"):
    v_dot = _libs["libratlib.so"].get("v_dot", "cdecl")
    v_dot.argtypes = []
    v_dot.restype = c_double

# /Users/plewis/librat/src/3D_vectors_double.h: 4
if _libs["libratlib.so"].has("Xy_angle", "cdecl"):
    Xy_angle = _libs["libratlib.so"].get("Xy_angle", "cdecl")
    Xy_angle.argtypes = []
    Xy_angle.restype = c_double

# /Users/plewis/librat/src/3D_vectors_double.h: 4
if _libs["libratlib.so"].has("M3_inverse", "cdecl"):
    M3_inverse = _libs["libratlib.so"].get("M3_inverse", "cdecl")
    M3_inverse.argtypes = []
    M3_inverse.restype = c_double

# /Users/plewis/librat/src/3D_vectors_double.h: 5
if _libs["libratlib.so"].has("Rotate_about", "cdecl"):
    Rotate_about = _libs["libratlib.so"].get("Rotate_about", "cdecl")
    Rotate_about.argtypes = []
    Rotate_about.restype = Triplet

# /Users/plewis/librat/src/3D_vectors_double.h: 5
if _libs["libratlib.so"].has("Rotate_vector", "cdecl"):
    Rotate_vector = _libs["libratlib.so"].get("Rotate_vector", "cdecl")
    Rotate_vector.argtypes = []
    Rotate_vector.restype = Triplet

# /Users/plewis/librat/src/3D_vectors_double.h: 5
if _libs["libratlib.so"].has("Multiply_vector", "cdecl"):
    Multiply_vector = _libs["libratlib.so"].get("Multiply_vector", "cdecl")
    Multiply_vector.argtypes = []
    Multiply_vector.restype = Triplet

# /Users/plewis/librat/src/3D_vectors_double.h: 5
if _libs["libratlib.so"].has("Spherical_to_cartesian", "cdecl"):
    Spherical_to_cartesian = _libs["libratlib.so"].get("Spherical_to_cartesian", "cdecl")
    Spherical_to_cartesian.argtypes = []
    Spherical_to_cartesian.restype = Triplet

# /Users/plewis/librat/src/3D_vectors_double.h: 5
if _libs["libratlib.so"].has("Cartesian_to_spherical", "cdecl"):
    Cartesian_to_spherical = _libs["libratlib.so"].get("Cartesian_to_spherical", "cdecl")
    Cartesian_to_spherical.argtypes = []
    Cartesian_to_spherical.restype = Triplet

# /Users/plewis/librat/src/3D_vectors_double.h: 5
if _libs["libratlib.so"].has("v_factor", "cdecl"):
    v_factor = _libs["libratlib.so"].get("v_factor", "cdecl")
    v_factor.argtypes = []
    v_factor.restype = Triplet

# /Users/plewis/librat/src/3D_vectors_double.h: 5
if _libs["libratlib.so"].has("Vector_plus", "cdecl"):
    Vector_plus = _libs["libratlib.so"].get("Vector_plus", "cdecl")
    Vector_plus.argtypes = []
    Vector_plus.restype = Triplet

# /Users/plewis/librat/src/3D_vectors_double.h: 5
if _libs["libratlib.so"].has("Vector_minus", "cdecl"):
    Vector_minus = _libs["libratlib.so"].get("Vector_minus", "cdecl")
    Vector_minus.argtypes = []
    Vector_minus.restype = Triplet

# /Users/plewis/librat/src/3D_vectors_double.h: 5
if _libs["libratlib.so"].has("Normalise", "cdecl"):
    Normalise = _libs["libratlib.so"].get("Normalise", "cdecl")
    Normalise.argtypes = []
    Normalise.restype = Triplet

# /Users/plewis/librat/src/3D_vectors_double.h: 5
if _libs["libratlib.so"].has("Vector_cross", "cdecl"):
    Vector_cross = _libs["libratlib.so"].get("Vector_cross", "cdecl")
    Vector_cross.argtypes = []
    Vector_cross.restype = Triplet

# /Users/plewis/librat/src/3D_vectors_double.h: 5
if _libs["libratlib.so"].has("Vector_copy", "cdecl"):
    Vector_copy = _libs["libratlib.so"].get("Vector_copy", "cdecl")
    Vector_copy.argtypes = []
    Vector_copy.restype = Triplet

# /Users/plewis/librat/src/3D_vectors_double.h: 5
if _libs["libratlib.so"].has("Matrix3_mult", "cdecl"):
    Matrix3_mult = _libs["libratlib.so"].get("Matrix3_mult", "cdecl")
    Matrix3_mult.argtypes = []
    Matrix3_mult.restype = Triplet

# /Users/plewis/librat/src/3D_vectors_double.h: 5
if _libs["libratlib.so"].has("Vector_copy2", "cdecl"):
    Vector_copy2 = _libs["libratlib.so"].get("Vector_copy2", "cdecl")
    Vector_copy2.argtypes = []
    Vector_copy2.restype = Triplet

# /Users/plewis/librat/src/3D_vectors_double.h: 7
if _libs["libratlib.so"].has("Matrix3_ip", "cdecl"):
    Matrix3_ip = _libs["libratlib.so"].get("Matrix3_ip", "cdecl")
    Matrix3_ip.argtypes = []
    Matrix3_ip.restype = Matrix3

# /Users/plewis/librat/src/3D_vectors_double.h: 7
if _libs["libratlib.so"].has("Matrix3_copy", "cdecl"):
    Matrix3_copy = _libs["libratlib.so"].get("Matrix3_copy", "cdecl")
    Matrix3_copy.argtypes = []
    Matrix3_copy.restype = Matrix3

# /Users/plewis/librat/src/3D_vectors_double.h: 7
if _libs["libratlib.so"].has("Matrix3_inverse", "cdecl"):
    Matrix3_inverse = _libs["libratlib.so"].get("Matrix3_inverse", "cdecl")
    Matrix3_inverse.argtypes = []
    Matrix3_inverse.restype = Matrix3

# /Users/plewis/librat/src/3D_vectors_double.h: 7
if _libs["libratlib.so"].has("Inverse3", "cdecl"):
    Inverse3 = _libs["libratlib.so"].get("Inverse3", "cdecl")
    Inverse3.argtypes = []
    Inverse3.restype = Matrix3

# /Users/plewis/librat/src/3D_vectors_double.h: 7
if _libs["libratlib.so"].has("Transpose3", "cdecl"):
    Transpose3 = _libs["libratlib.so"].get("Transpose3", "cdecl")
    Transpose3.argtypes = []
    Transpose3.restype = Matrix3

# /Users/plewis/librat/src/3D_vectors_double.h: 8
if _libs["libratlib.so"].has("Scale", "cdecl"):
    Scale = _libs["libratlib.so"].get("Scale", "cdecl")
    Scale.argtypes = []
    Scale.restype = c_double

# /Users/plewis/librat/src/3D_vectors_double.h: 8
if _libs["libratlib.so"].has("Cofactors", "cdecl"):
    Cofactors = _libs["libratlib.so"].get("Cofactors", "cdecl")
    Cofactors.argtypes = []
    Cofactors.restype = c_double

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

# ./bags.h: 6
for _lib in _libs.values():
    try:
        sun_magnitude = (POINTER(c_double)).in_dll(_lib, "sun_magnitude")
        break
    except:
        pass

# ./bags.h: 7
for _lib in _libs.values():
    try:
        sky_black = (c_int).in_dll(_lib, "sky_black")
        break
    except:
        pass

# ./bags.h: 8
for _lib in _libs.values():
    try:
        sun_Ptr = (POINTER(triplet)).in_dll(_lib, "sun_Ptr")
        break
    except:
        pass

# ./bags.h: 9
for _lib in _libs.values():
    try:
        sky_flag = (c_int).in_dll(_lib, "sky_flag")
        break
    except:
        pass

# ./bags.h: 10
for _lib in _libs.values():
    try:
        sun_fov = (c_double).in_dll(_lib, "sun_fov")
        break
    except:
        pass

# ./bags.h: 11
for _lib in _libs.values():
    try:
        theta_sun = (c_double).in_dll(_lib, "theta_sun")
        break
    except:
        pass

# ./bags.h: 12
for _lib in _libs.values():
    try:
        phi_gap = (c_double).in_dll(_lib, "phi_gap")
        break
    except:
        pass

# ./bags.h: 13
for _lib in _libs.values():
    try:
        sun_elevation_axis = (triplet).in_dll(_lib, "sun_elevation_axis")
        break
    except:
        pass

# ./bags.h: 14
for _lib in _libs.values():
    try:
        direct_file = (String).in_dll(_lib, "direct_file")
        break
    except:
        pass

# ./bags.h: 15
for _lib in _libs.values():
    try:
        position = (POINTER(Camera_op)).in_dll(_lib, "position")
        break
    except:
        pass

# ./bags.h: 21
for _lib in _libs.values():
    try:
        name = (String).in_dll(_lib, "name")
        break
    except:
        pass

# ./bags.h: 22
for _lib in _libs.values():
    try:
        camera_ip = (Camera_ip).in_dll(_lib, "camera_ip")
        break
    except:
        pass

# ./bags.h: 23
for _lib in _libs.values():
    try:
        camera_op = (Camera_op).in_dll(_lib, "camera_op")
        break
    except:
        pass

# ./bags.h: 60
for _lib in _libs.values():
    try:
        vertexStore = (c_int).in_dll(_lib, "vertexStore")
        break
    except:
        pass

# ./bags.h: 61
for _lib in _libs.values():
    try:
        angleTol = (c_double).in_dll(_lib, "angleTol")
        break
    except:
        pass

# ./bags.h: 61
for _lib in _libs.values():
    try:
        distanceTol = (c_double).in_dll(_lib, "distanceTol")
        break
    except:
        pass

# ./bags.h: 61
for _lib in _libs.values():
    try:
        sizeTol = (c_double).in_dll(_lib, "sizeTol")
        break
    except:
        pass

# ./bags.h: 63
for _lib in _libs.values():
    try:
        distanceStore = (POINTER(c_double)).in_dll(_lib, "distanceStore")
        break
    except:
        pass

# ./bags.h: 64
for _lib in _libs.values():
    try:
        reflectanceStore = (POINTER(c_double)).in_dll(_lib, "reflectanceStore")
        break
    except:
        pass

# ./bags.h: 65
for _lib in _libs.values():
    try:
        doInfinite = (c_int).in_dll(_lib, "doInfinite")
        break
    except:
        pass

# ./bags.h: 66
for _lib in _libs.values():
    try:
        infiniteOffset = (triplet).in_dll(_lib, "infiniteOffset")
        break
    except:
        pass

# ./bags.h: 67
for _lib in _libs.values():
    try:
        blackSky = (c_int).in_dll(_lib, "blackSky")
        break
    except:
        pass

# ./bags.h: 68
for _lib in _libs.values():
    try:
        rtRatio = (c_double).in_dll(_lib, "rtRatio")
        break
    except:
        pass

# ./bags.h: 69
for _lib in _libs.values():
    try:
        reverse = (c_int).in_dll(_lib, "reverse")
        break
    except:
        pass

# ./bags.h: 70
for _lib in _libs.values():
    try:
        photonSpread = (c_int).in_dll(_lib, "photonSpread")
        break
    except:
        pass

# ./bags.h: 71
for _lib in _libs.values():
    try:
        lidar = (c_int).in_dll(_lib, "lidar")
        break
    except:
        pass

# ./bags.h: 72
for _lib in _libs.values():
    try:
        sz = (c_double).in_dll(_lib, "sz")
        break
    except:
        pass

# ./bags.h: 73
for _lib in _libs.values():
    try:
        sunlocation = (POINTER(c_double)).in_dll(_lib, "sunlocation")
        break
    except:
        pass

# ./bags.h: 74
for _lib in _libs.values():
    try:
        sunrayangle = (POINTER(c_double)).in_dll(_lib, "sunrayangle")
        break
    except:
        pass

# ./bags.h: 75
for _lib in _libs.values():
    try:
        nSuns = (c_int).in_dll(_lib, "nSuns")
        break
    except:
        pass

# ./bags.h: 76
for _lib in _libs.values():
    try:
        illuminationvector = (POINTER(triplet)).in_dll(_lib, "illuminationvector")
        break
    except:
        pass

# ./bags.h: 77
for _lib in _libs.values():
    try:
        lengthToSun = (POINTER(c_double)).in_dll(_lib, "lengthToSun")
        break
    except:
        pass

# ./bags.h: 78
for _lib in _libs.values():
    try:
        isSunTest = (c_int).in_dll(_lib, "isSunTest")
        break
    except:
        pass

# ./bags.h: 79
for _lib in _libs.values():
    try:
        rowsandcols = (POINTER(c_int)).in_dll(_lib, "rowsandcols")
        break
    except:
        pass

# ./bags.h: 98
for _lib in _libs.values():
    try:
        samples = (POINTER(Samples)).in_dll(_lib, "samples")
        break
    except:
        pass

# ./bags.h: 99
for _lib in _libs.values():
    try:
        material_lut = (POINTER(Standard_Material_List)).in_dll(_lib, "material_lut")
        break
    except:
        pass

# ./bags.h: 100
for _lib in _libs.values():
    try:
        material_names = (POINTER(POINTER(c_char))).in_dll(_lib, "material_names")
        break
    except:
        pass

# ./bags.h: 158
for _lib in _libs.values():
    try:
        name = (String).in_dll(_lib, "name")
        break
    except:
        pass

# ./bags.h: 180
for _lib in _libs.values():
    try:
        nd = (POINTER(c_float)).in_dll(_lib, "nd")
        break
    except:
        pass

# ./bags.h: 181
for _lib in _libs.values():
    try:
        nsamps = (c_int * int(3)).in_dll(_lib, "nsamps")
        break
    except:
        pass

# ./bags.h: 183
for _lib in _libs.values():
    try:
        otherStuff = (POINTER(None)).in_dll(_lib, "otherStuff")
        break
    except:
        pass

# ./bags.h: 184
for _lib in _libs.values():
    try:
        rawfilename = (String).in_dll(_lib, "rawfilename")
        break
    except:
        pass

# ./bags.h: 185
for _lib in _libs.values():
    try:
        datafilename = (String).in_dll(_lib, "datafilename")
        break
    except:
        pass

# ./bags.h: 193
for _lib in _libs.values():
    try:
        wavebandbag = (POINTER(WavebandBag)).in_dll(_lib, "wavebandbag")
        break
    except:
        pass

# ./bags.h: 195
for _lib in _libs.values():
    try:
        material_name = (POINTER(POINTER(c_char))).in_dll(_lib, "material_name")
        break
    except:
        pass

# ./bags.h: 196
for _lib in _libs.values():
    try:
        material_table = (POINTER(Material_table)).in_dll(_lib, "material_table")
        break
    except:
        pass

# ./bags.h: 197
for _lib in _libs.values():
    try:
        PRAT_MAX_MATERIALS = (c_int).in_dll(_lib, "PRAT_MAX_MATERIALS")
        break
    except:
        pass

# ./bags.h: 198
for _lib in _libs.values():
    try:
        current_mtl = (POINTER(Material_table)).in_dll(_lib, "current_mtl")
        break
    except:
        pass

# ./bags.h: 199
for _lib in _libs.values():
    try:
        m_inv_reverse = (Matrix4).in_dll(_lib, "m_inv_reverse")
        break
    except:
        pass

# ./bags.h: 199
for _lib in _libs.values():
    try:
        m_inverse_fwd = (Matrix4).in_dll(_lib, "m_inverse_fwd")
        break
    except:
        pass

# ./bags.h: 200
for _lib in _libs.values():
    try:
        max_number_of_groups = (c_int).in_dll(_lib, "max_number_of_groups")
        break
    except:
        pass

# ./bags.h: 202
for _lib in _libs.values():
    try:
        samples = (Samples).in_dll(_lib, "samples")
        break
    except:
        pass

# ./bags.h: 203
for _lib in _libs.values():
    try:
        group = (Group).in_dll(_lib, "group")
        break
    except:
        pass

# ./bags.h: 204
for _lib in _libs.values():
    try:
        sensor_wavebands = (Sensor_Wavebands).in_dll(_lib, "sensor_wavebands")
        break
    except:
        pass

# ./bags.h: 205
for _lib in _libs.values():
    try:
        timer = (c_int).in_dll(_lib, "timer")
        break
    except:
        pass

# ./bags.h: 206
for _lib in _libs.values():
    try:
        level = (c_int).in_dll(_lib, "level")
        break
    except:
        pass

# ./bags.h: 207
for _lib in _libs.values():
    try:
        vertices = (VeRtIcEs).in_dll(_lib, "vertices")
        break
    except:
        pass

# ./bags.h: 207
for _lib in _libs.values():
    try:
        normals = (VeRtIcEs).in_dll(_lib, "normals")
        break
    except:
        pass

# ./bags.h: 207
for _lib in _libs.values():
    try:
        locals = (VeRtIcEs).in_dll(_lib, "locals")
        break
    except:
        pass

# ./bags.h: 208
try:
    bbox = (POINTER(BBox)).in_dll(_libs["libratlib.so"], "bbox")
except:
    pass

# ./bags.h: 209
for _lib in _libs.values():
    try:
        mmap_flag = (c_int).in_dll(_lib, "mmap_flag")
        break
    except:
        pass

# ./bags.h: 211
for _lib in _libs.values():
    try:
        lidar = (c_int).in_dll(_lib, "lidar")
        break
    except:
        pass

# ./bags.h: 213
for _lib in _libs.values():
    try:
        globalArgc = (c_int).in_dll(_lib, "globalArgc")
        break
    except:
        pass

# ./bags.h: 214
for _lib in _libs.values():
    try:
        globalArgv = (POINTER(POINTER(c_char))).in_dll(_lib, "globalArgv")
        break
    except:
        pass

# ./bags.h: 218
for _lib in _libs.values():
    try:
        globalnBands = (c_int).in_dll(_lib, "globalnBands")
        break
    except:
        pass

# ./bags.h: 219
for _lib in _libs.values():
    try:
        globalOver = (c_double).in_dll(_lib, "globalOver")
        break
    except:
        pass

# ./bags.h: 220
for _lib in _libs.values():
    try:
        globalDoLidar = (c_int).in_dll(_lib, "globalDoLidar")
        break
    except:
        pass

# ./bags.h: 221
for _lib in _libs.values():
    try:
        globalnRTD = (c_int).in_dll(_lib, "globalnRTD")
        break
    except:
        pass

# ./bags.h: 223
for _lib in _libs.values():
    try:
        facets = (POINTER(fFacet)).in_dll(_lib, "facets")
        break
    except:
        pass

# ./bags.h: 224
for _lib in _libs.values():
    try:
        nFacets = (c_int).in_dll(_lib, "nFacets")
        break
    except:
        pass

# ./bags.h: 224
for _lib in _libs.values():
    try:
        whichFacet = (c_int).in_dll(_lib, "whichFacet")
        break
    except:
        pass

# ./bags.h: 224
for _lib in _libs.values():
    try:
        nFacetBlock = (c_int).in_dll(_lib, "nFacetBlock")
        break
    except:
        pass

# ./bags.h: 225
for _lib in _libs.values():
    try:
        ratTree = (POINTER(RATtree)).in_dll(_lib, "ratTree")
        break
    except:
        pass

# ./bags.h: 226
for _lib in _libs.values():
    try:
        nSuns = (c_int).in_dll(_lib, "nSuns")
        break
    except:
        pass

# ./bags.h: 229
for _lib in _libs.values():
    try:
        ratMat = (POINTER(RATmaterials)).in_dll(_lib, "ratMat")
        break
    except:
        pass

# ./bags.h: 230
for _lib in _libs.values():
    try:
        nMaterials = (POINTER(c_int)).in_dll(_lib, "nMaterials")
        break
    except:
        pass

# ./bags.h: 231
for _lib in _libs.values():
    try:
        ratMatUsed = (POINTER(RATmaterials)).in_dll(_lib, "ratMatUsed")
        break
    except:
        pass

# ./bags.h: 232
for _lib in _libs.values():
    try:
        nMaterialsUsed = (POINTER(c_int)).in_dll(_lib, "nMaterialsUsed")
        break
    except:
        pass

# ./bags.h: 233
for _lib in _libs.values():
    try:
        materialTranslatorLut = (POINTER(c_int)).in_dll(_lib, "materialTranslatorLut")
        break
    except:
        pass

# ./bags.h: 236
for _lib in _libs.values():
    try:
        nObjectTypes = (c_int).in_dll(_lib, "nObjectTypes")
        break
    except:
        pass

# ./bags.h: 237
for _lib in _libs.values():
    try:
        objectTypes = (POINTER(c_int)).in_dll(_lib, "objectTypes")
        break
    except:
        pass

# ./bags.h: 238
for _lib in _libs.values():
    try:
        objectTypeCapacity = (c_int).in_dll(_lib, "objectTypeCapacity")
        break
    except:
        pass

# ./bags.h: 239
for _lib in _libs.values():
    try:
        objectMaterialNames = (POINTER(POINTER(c_char))).in_dll(_lib, "objectMaterialNames")
        break
    except:
        pass

# ./bags.h: 240
for _lib in _libs.values():
    try:
        minObjectSize = (POINTER(c_double)).in_dll(_lib, "minObjectSize")
        break
    except:
        pass

# ./bags.h: 241
for _lib in _libs.values():
    try:
        maxObjectSize = (POINTER(c_double)).in_dll(_lib, "maxObjectSize")
        break
    except:
        pass

# ./bags.h: 244
for _lib in _libs.values():
    try:
        ratvol = (POINTER(RATvol)).in_dll(_lib, "ratvol")
        break
    except:
        pass

# ./bags.h: 248
for _lib in _libs.values():
    try:
        nratObjs = (c_int).in_dll(_lib, "nratObjs")
        break
    except:
        pass

# ./bags.h: 249
for _lib in _libs.values():
    try:
        hit_camera = (c_int).in_dll(_lib, "hit_camera")
        break
    except:
        pass

# ./bags.h: 251
for _lib in _libs.values():
    try:
        nstore = (c_int).in_dll(_lib, "nstore")
        break
    except:
        pass

# ./bags.h: 252
for _lib in _libs.values():
    try:
        store = (POINTER(POINTER(None))).in_dll(_lib, "store")
        break
    except:
        pass

# ./bags.h: 254
for _lib in _libs.values():
    try:
        Matrix = (c_double * int(16)).in_dll(_lib, "Matrix")
        break
    except:
        pass

# /Users/plewis/librat/src/prat.h: 80
if _libs["libratlib.so"].has("atof", "cdecl"):
    atof = _libs["libratlib.so"].get("atof", "cdecl")
    atof.argtypes = []
    atof.restype = c_double

# /Users/plewis/librat/src/prat.h: 81
if _libs["libratlib.so"].has("exit", "cdecl"):
    exit = _libs["libratlib.so"].get("exit", "cdecl")
    exit.argtypes = []
    exit.restype = None

# /Users/plewis/librat/src/prat.h: 82
if _libs["libratlib.so"].has("error1", "cdecl"):
    error1 = _libs["libratlib.so"].get("error1", "cdecl")
    error1.argtypes = []
    error1.restype = None

# /Users/plewis/librat/src/prat.h: 82
if _libs["libratlib.so"].has("error2", "cdecl"):
    error2 = _libs["libratlib.so"].get("error2", "cdecl")
    error2.argtypes = []
    error2.restype = None

# /Users/plewis/librat/src/prat.h: 82
if _libs["libratlib.so"].has("init_header", "cdecl"):
    init_header = _libs["libratlib.so"].get("init_header", "cdecl")
    init_header.argtypes = []
    init_header.restype = None

# /Users/plewis/librat/src/prat.h: 83
if _libs["libratlib.so"].has("apply_material_map", "cdecl"):
    apply_material_map = _libs["libratlib.so"].get("apply_material_map", "cdecl")
    apply_material_map.argtypes = []
    apply_material_map.restype = c_int

# /Users/plewis/librat/src/prat.h: 84
if _libs["libratlib.so"].has("calculate_current_reflectance", "cdecl"):
    calculate_current_reflectance = _libs["libratlib.so"].get("calculate_current_reflectance", "cdecl")
    calculate_current_reflectance.argtypes = []
    calculate_current_reflectance.restype = POINTER(c_ubyte)
    calculate_current_reflectance.errcheck = lambda v,*a : cast(v, c_void_p)

# /Users/plewis/librat/src/prat.h: 85
if _libs["libratlib.so"].has("mmap_write_hips_image_no_free", "cdecl"):
    mmap_write_hips_image_no_free = _libs["libratlib.so"].get("mmap_write_hips_image_no_free", "cdecl")
    mmap_write_hips_image_no_free.argtypes = []
    mmap_write_hips_image_no_free.restype = c_int

# /Users/plewis/librat/src/prat.h: 86
if _libs["libratlib.so"].has("calloc", "cdecl"):
    calloc = _libs["libratlib.so"].get("calloc", "cdecl")
    calloc.argtypes = []
    calloc.restype = POINTER(c_ubyte)
    calloc.errcheck = lambda v,*a : cast(v, c_void_p)

# /Users/plewis/librat/src/prat.h: 94
if _libs["libratlib.so"].has("is_double", "cdecl"):
    is_double = _libs["libratlib.so"].get("is_double", "cdecl")
    is_double.argtypes = [String, POINTER(c_double)]
    is_double.restype = c_int

# /Users/plewis/librat/src/frat.h: 20
if _libs["libratlib.so"].has("Random", "cdecl"):
    Random = _libs["libratlib.so"].get("Random", "cdecl")
    Random.argtypes = []
    Random.restype = c_double

# /Users/plewis/librat/src/frat.h: 36
if _libs["libratlib.so"].has("write_hips_image", "cdecl"):
    write_hips_image = _libs["libratlib.so"].get("write_hips_image", "cdecl")
    write_hips_image.argtypes = []
    write_hips_image.restype = None

# /Users/plewis/librat/src/frat.h: 36
if _libs["libratlib.so"].has("reset_local_z_to_local_max_height", "cdecl"):
    reset_local_z_to_local_max_height = _libs["libratlib.so"].get("reset_local_z_to_local_max_height", "cdecl")
    reset_local_z_to_local_max_height.argtypes = []
    reset_local_z_to_local_max_height.restype = None

# /Users/plewis/librat/src/frat.h: 36
if _libs["libratlib.so"].has("read_default_materials", "cdecl"):
    read_default_materials = _libs["libratlib.so"].get("read_default_materials", "cdecl")
    read_default_materials.argtypes = []
    read_default_materials.restype = None

# /Users/plewis/librat/src/frat.h: 37
for _lib in _libs.values():
    if not _lib.has("get_mmap_flag", "cdecl"):
        continue
    get_mmap_flag = _lib.get("get_mmap_flag", "cdecl")
    get_mmap_flag.argtypes = []
    get_mmap_flag.restype = c_int
    break

# /Users/plewis/librat/src/frat.h: 37
if _libs["libratlib.so"].has("read_camera_description", "cdecl"):
    read_camera_description = _libs["libratlib.so"].get("read_camera_description", "cdecl")
    read_camera_description.argtypes = []
    read_camera_description.restype = c_int

# /Users/plewis/librat/src/frat.h: 38
if _libs["libratlib.so"].has("getenv", "cdecl"):
    getenv = _libs["libratlib.so"].get("getenv", "cdecl")
    getenv.argtypes = []
    if sizeof(c_int) == sizeof(c_void_p):
        getenv.restype = ReturnString
    else:
        getenv.restype = String
        getenv.errcheck = ReturnString

# /Users/plewis/librat/src/frat.h: 39
if _libs["libratlib.so"].has("load_identity_matrix4", "cdecl"):
    load_identity_matrix4 = _libs["libratlib.so"].get("load_identity_matrix4", "cdecl")
    load_identity_matrix4.argtypes = []
    load_identity_matrix4.restype = Matrix4

# /Users/plewis/librat/src/frat.h: 40
for _lib in _libs.values():
    if not _lib.has("useage", "cdecl"):
        continue
    useage = _lib.get("useage", "cdecl")
    useage.argtypes = []
    useage.restype = None
    break

# /Users/plewis/librat/src/frat.h: 41
for _lib in _libs.values():
    if not _lib.has("sort_contents_of_bbox", "cdecl"):
        continue
    sort_contents_of_bbox = _lib.get("sort_contents_of_bbox", "cdecl")
    sort_contents_of_bbox.argtypes = []
    sort_contents_of_bbox.restype = triplet
    break

# /Users/plewis/librat/src/frat.h: 42
if _libs["libratlib.so"].has("calloc", "cdecl"):
    calloc = _libs["libratlib.so"].get("calloc", "cdecl")
    calloc.argtypes = []
    calloc.restype = POINTER(c_ubyte)
    calloc.errcheck = lambda v,*a : cast(v, c_void_p)

# /Users/plewis/librat/src/frat.h: 42
if _libs["libratlib.so"].has("init_header", "cdecl"):
    init_header = _libs["libratlib.so"].get("init_header", "cdecl")
    init_header.argtypes = []
    init_header.restype = None

# /Users/plewis/librat/src/frat.h: 42
if _libs["libratlib.so"].has("update_header", "cdecl"):
    update_header = _libs["libratlib.so"].get("update_header", "cdecl")
    update_header.argtypes = []
    update_header.restype = None

# /Users/plewis/librat/src/frat.h: 43
if _libs["libratlib.so"].has("mmap_read_hips_image", "cdecl"):
    mmap_read_hips_image = _libs["libratlib.so"].get("mmap_read_hips_image", "cdecl")
    mmap_read_hips_image.argtypes = []
    mmap_read_hips_image.restype = c_int

# /Users/plewis/librat/src/frat.h: 44
if _libs["libratlib.so"].has("get_skymap_wavelengths", "cdecl"):
    get_skymap_wavelengths = _libs["libratlib.so"].get("get_skymap_wavelengths", "cdecl")
    get_skymap_wavelengths.argtypes = []
    get_skymap_wavelengths.restype = None

# /Users/plewis/librat/src/frat.h: 44
if _libs["libratlib.so"].has("read_spectral_file", "cdecl"):
    read_spectral_file = _libs["libratlib.so"].get("read_spectral_file", "cdecl")
    read_spectral_file.argtypes = []
    read_spectral_file.restype = None

# /Users/plewis/librat/src/frat.h: 44
if _libs["libratlib.so"].has("update_desc", "cdecl"):
    update_desc = _libs["libratlib.so"].get("update_desc", "cdecl")
    update_desc.argtypes = []
    update_desc.restype = None

# /Users/plewis/librat/src/frat.h: 44
if _libs["libratlib.so"].has("scan_image", "cdecl"):
    scan_image = _libs["libratlib.so"].get("scan_image", "cdecl")
    scan_image.argtypes = []
    scan_image.restype = None

# /Users/plewis/librat/src/frat.h: 45
if _libs["libratlib.so"].has("initialise_wavefront_storage", "cdecl"):
    initialise_wavefront_storage = _libs["libratlib.so"].get("initialise_wavefront_storage", "cdecl")
    initialise_wavefront_storage.argtypes = []
    initialise_wavefront_storage.restype = c_int

# /Users/plewis/librat/src/frat.h: 45
if _libs["libratlib.so"].has("parse_prat_wavefront_data", "cdecl"):
    parse_prat_wavefront_data = _libs["libratlib.so"].get("parse_prat_wavefront_data", "cdecl")
    parse_prat_wavefront_data.argtypes = []
    parse_prat_wavefront_data.restype = c_int

# /Users/plewis/librat/src/frat.h: 45
if _libs["libratlib.so"].has("mmap_write_hips_image_no_free", "cdecl"):
    mmap_write_hips_image_no_free = _libs["libratlib.so"].get("mmap_write_hips_image_no_free", "cdecl")
    mmap_write_hips_image_no_free.argtypes = []
    mmap_write_hips_image_no_free.restype = c_int

# /Users/plewis/librat/src/frat.h: 45
if _libs["libratlib.so"].has("sscan_int", "cdecl"):
    sscan_int = _libs["libratlib.so"].get("sscan_int", "cdecl")
    sscan_int.argtypes = []
    sscan_int.restype = c_int

# /Users/plewis/librat/src/frat.h: 45
if _libs["libratlib.so"].has("sscan_double", "cdecl"):
    sscan_double = _libs["libratlib.so"].get("sscan_double", "cdecl")
    sscan_double.argtypes = []
    sscan_double.restype = c_int

# /Users/plewis/librat/src/frat.h: 45
if _libs["libratlib.so"].has("atoi", "cdecl"):
    atoi = _libs["libratlib.so"].get("atoi", "cdecl")
    atoi.argtypes = []
    atoi.restype = c_int

# /Users/plewis/librat/src/frat.h: 46
if _libs["libratlib.so"].has("pload_identity_matrix4", "cdecl"):
    pload_identity_matrix4 = _libs["libratlib.so"].get("pload_identity_matrix4", "cdecl")
    pload_identity_matrix4.argtypes = []
    pload_identity_matrix4.restype = None

# /Users/plewis/librat/src/rat.h: 5
if _libs["libratlib.so"].has("sphereTesselate", "cdecl"):
    sphereTesselate = _libs["libratlib.so"].get("sphereTesselate", "cdecl")
    sphereTesselate.argtypes = [c_int, POINTER(c_int)]
    sphereTesselate.restype = POINTER(c_double)

# /Users/plewis/librat/src/rat.h: 6
if _libs["libratlib.so"].has("facetNormal", "cdecl"):
    facetNormal = _libs["libratlib.so"].get("facetNormal", "cdecl")
    facetNormal.argtypes = [POINTER(c_double), POINTER(struct_Facet_struct)]
    facetNormal.restype = c_double

# /Users/plewis/librat/src/rat.h: 7
if _libs["libratlib.so"].has("allocate_material_list", "cdecl"):
    allocate_material_list = _libs["libratlib.so"].get("allocate_material_list", "cdecl")
    allocate_material_list.argtypes = []
    allocate_material_list.restype = None

# /Users/plewis/librat/src/rat.h: 8
if _libs["libratlib.so"].has("fp_fwrite_header", "cdecl"):
    fp_fwrite_header = _libs["libratlib.so"].get("fp_fwrite_header", "cdecl")
    fp_fwrite_header.argtypes = []
    fp_fwrite_header.restype = None

# /Users/plewis/librat/src/rat.h: 9
if _libs["libratlib.so"].has("calculate_material_lut", "cdecl"):
    calculate_material_lut = _libs["libratlib.so"].get("calculate_material_lut", "cdecl")
    calculate_material_lut.argtypes = []
    calculate_material_lut.restype = None

# /Users/plewis/librat/src/rat.h: 10
if _libs["libratlib.so"].has("find_number_of_materials_in_material_map", "cdecl"):
    find_number_of_materials_in_material_map = _libs["libratlib.so"].get("find_number_of_materials_in_material_map", "cdecl")
    find_number_of_materials_in_material_map.argtypes = []
    find_number_of_materials_in_material_map.restype = c_int

# /Users/plewis/librat/src/rat.h: 11
if _libs["libratlib.so"].has("calculate_sun_elevation_axis", "cdecl"):
    calculate_sun_elevation_axis = _libs["libratlib.so"].get("calculate_sun_elevation_axis", "cdecl")
    calculate_sun_elevation_axis.argtypes = []
    calculate_sun_elevation_axis.restype = None

# /Users/plewis/librat/src/rat.h: 12
if _libs["libratlib.so"].has("pre_calculate_sun_parameters", "cdecl"):
    pre_calculate_sun_parameters = _libs["libratlib.so"].get("pre_calculate_sun_parameters", "cdecl")
    pre_calculate_sun_parameters.argtypes = []
    pre_calculate_sun_parameters.restype = None

# /Users/plewis/librat/src/rat.h: 13
if _libs["libratlib.so"].has("clearHitPoint", "cdecl"):
    clearHitPoint = _libs["libratlib.so"].get("clearHitPoint", "cdecl")
    clearHitPoint.argtypes = []
    clearHitPoint.restype = None

# /Users/plewis/librat/src/rat.h: 14
if _libs["libratlib.so"].has("gatherPixelStatsForRay", "cdecl"):
    gatherPixelStatsForRay = _libs["libratlib.so"].get("gatherPixelStatsForRay", "cdecl")
    gatherPixelStatsForRay.argtypes = []
    gatherPixelStatsForRay.restype = None

# /Users/plewis/librat/src/rat.h: 15
if _libs["libratlib.so"].has("getNdefaultMaterials", "cdecl"):
    getNdefaultMaterials = _libs["libratlib.so"].get("getNdefaultMaterials", "cdecl")
    getNdefaultMaterials.argtypes = []
    getNdefaultMaterials.restype = c_int

# /Users/plewis/librat/src/rat.h: 16
if _libs["libratlib.so"].has("seed_randomise", "cdecl"):
    seed_randomise = _libs["libratlib.so"].get("seed_randomise", "cdecl")
    seed_randomise.argtypes = []
    seed_randomise.restype = c_int

# /Users/plewis/librat/src/rat.h: 17
if _libs["libratlib.so"].has("quantise_sky_waveband", "cdecl"):
    quantise_sky_waveband = _libs["libratlib.so"].get("quantise_sky_waveband", "cdecl")
    quantise_sky_waveband.argtypes = []
    quantise_sky_waveband.restype = c_int

# /Users/plewis/librat/src/rat.h: 18
if _libs["libratlib.so"].has("Vector_Matrix_multiplication_to_Vector", "cdecl"):
    Vector_Matrix_multiplication_to_Vector = _libs["libratlib.so"].get("Vector_Matrix_multiplication_to_Vector", "cdecl")
    Vector_Matrix_multiplication_to_Vector.argtypes = []
    Vector_Matrix_multiplication_to_Vector.restype = None

# /Users/plewis/librat/src/rat.h: 19
for _lib in _libs.values():
    if not _lib.has("expand_filename", "cdecl"):
        continue
    expand_filename = _lib.get("expand_filename", "cdecl")
    expand_filename.argtypes = []
    expand_filename.restype = c_int
    break

# /Users/plewis/librat/src/rat.h: 20
if _libs["libratlib.so"].has("calculate_reflectance_data", "cdecl"):
    calculate_reflectance_data = _libs["libratlib.so"].get("calculate_reflectance_data", "cdecl")
    calculate_reflectance_data.argtypes = []
    calculate_reflectance_data.restype = c_int

# /Users/plewis/librat/src/rat.h: 21
if _libs["libratlib.so"].has("loadObjectInfo", "cdecl"):
    loadObjectInfo = _libs["libratlib.so"].get("loadObjectInfo", "cdecl")
    loadObjectInfo.argtypes = []
    loadObjectInfo.restype = None

# /Users/plewis/librat/src/rat.h: 22
if _libs["libratlib.so"].has("Matrix_Matrix_multiplication_to_Matrix", "cdecl"):
    Matrix_Matrix_multiplication_to_Matrix = _libs["libratlib.so"].get("Matrix_Matrix_multiplication_to_Matrix", "cdecl")
    Matrix_Matrix_multiplication_to_Matrix.argtypes = []
    Matrix_Matrix_multiplication_to_Matrix.restype = None

# /Users/plewis/librat/src/rat.h: 23
if _libs["libratlib.so"].has("loadObject", "cdecl"):
    loadObject = _libs["libratlib.so"].get("loadObject", "cdecl")
    loadObject.argtypes = []
    loadObject.restype = c_int

# /Users/plewis/librat/src/rat.h: 36
for _lib in _libs.values():
    if not _lib.has("RATsignalInterrupt", "cdecl"):
        continue
    RATsignalInterrupt = _lib.get("RATsignalInterrupt", "cdecl")
    RATsignalInterrupt.argtypes = [c_int]
    RATsignalInterrupt.restype = None
    break

# /Users/plewis/librat/src/ratFront.h: 39
try:
    ___i = (c_int).in_dll(_libs["libratlib.so"], "___i")
except:
    pass

# /Users/plewis/librat/src/ratFront.h: 45
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

# /Users/plewis/librat/src/ratFront.h: 60
for _lib in _libs.values():
    try:
        m = (c_int).in_dll(_lib, "m")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 60
for _lib in _libs.values():
    try:
        n = (c_int).in_dll(_lib, "n")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 60
for _lib in _libs.values():
    try:
        nbands = (c_int).in_dll(_lib, "nbands")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 61
for _lib in _libs.values():
    try:
        ratMat = (POINTER(RATmaterials)).in_dll(_lib, "ratMat")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 62
try:
    counter = (POINTER(c_int)).in_dll(_libs["libratlib.so"], "counter")
except:
    pass

# /Users/plewis/librat/src/ratFront.h: 63
for _lib in _libs.values():
    try:
        r = (POINTER(POINTER(struct_RATorderStore))).in_dll(_lib, "r")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 64
for _lib in _libs.values():
    try:
        nRatorders = (c_int).in_dll(_lib, "nRatorders")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 65
try:
    mode = (c_int).in_dll(_libs["libratlib.so"], "mode")
except:
    pass

# /Users/plewis/librat/src/ratFront.h: 72
for _lib in _libs.values():
    try:
        wavebandFiles = (POINTER(POINTER(c_char))).in_dll(_lib, "wavebandFiles")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 73
for _lib in _libs.values():
    try:
        nWavebandFiles = (c_int).in_dll(_lib, "nWavebandFiles")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 75
for _lib in _libs.values():
    try:
        wavebandbag = (POINTER(WavebandBag)).in_dll(_lib, "wavebandbag")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 77
for _lib in _libs.values():
    try:
        inputImage = (String).in_dll(_lib, "inputImage")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 78
for _lib in _libs.values():
    try:
        sampleCountImage = (String).in_dll(_lib, "sampleCountImage")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 79
for _lib in _libs.values():
    try:
        outputImage = (String).in_dll(_lib, "outputImage")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 80
for _lib in _libs.values():
    try:
        gridMap = (String).in_dll(_lib, "gridMap")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 81
for _lib in _libs.values():
    try:
        PSFRead = (c_int).in_dll(_lib, "PSFRead")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 82
for _lib in _libs.values():
    try:
        modulationMap = (String).in_dll(_lib, "modulationMap")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 85
for _lib in _libs.values():
    try:
        nScatteredImages = (c_int).in_dll(_lib, "nScatteredImages")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 86
for _lib in _libs.values():
    try:
        scatteredImages = (POINTER(POINTER(c_char))).in_dll(_lib, "scatteredImages")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 87
for _lib in _libs.values():
    try:
        scatterStart = (POINTER(c_int)).in_dll(_lib, "scatterStart")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 88
for _lib in _libs.values():
    try:
        scatterEnd = (POINTER(c_int)).in_dll(_lib, "scatterEnd")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 89
for _lib in _libs.values():
    try:
        scatterResultImage = (POINTER(POINTER(c_double))).in_dll(_lib, "scatterResultImage")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 91
for _lib in _libs.values():
    try:
        resultImage = (String).in_dll(_lib, "resultImage")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 92
for _lib in _libs.values():
    try:
        resultPsfImage = (String).in_dll(_lib, "resultPsfImage")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 94
for _lib in _libs.values():
    try:
        name = (String).in_dll(_lib, "name")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 95
for _lib in _libs.values():
    try:
        filename = (String).in_dll(_lib, "filename")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 97
for _lib in _libs.values():
    try:
        nBands = (c_int).in_dll(_lib, "nBands")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 98
for _lib in _libs.values():
    try:
        nBins = (c_int).in_dll(_lib, "nBins")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 99
for _lib in _libs.values():
    try:
        nOrders = (c_int).in_dll(_lib, "nOrders")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 100
for _lib in _libs.values():
    try:
        binStart = (c_double).in_dll(_lib, "binStart")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 101
for _lib in _libs.values():
    try:
        binStep = (c_double).in_dll(_lib, "binStep")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 102
for _lib in _libs.values():
    try:
        binMax = (c_double).in_dll(_lib, "binMax")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 103
for _lib in _libs.values():
    try:
        imagedata = (POINTER(c_double)).in_dll(_lib, "imagedata")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 104
for _lib in _libs.values():
    try:
        psfdata = (POINTER(c_double)).in_dll(_lib, "psfdata")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 105
for _lib in _libs.values():
    try:
        radiance = (POINTER(c_double)).in_dll(_lib, "radiance")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 106
for _lib in _libs.values():
    try:
        directRadiance = (POINTER(POINTER(POINTER(c_double)))).in_dll(_lib, "directRadiance")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 107
for _lib in _libs.values():
    try:
        diffuseRadiance = (POINTER(POINTER(POINTER(c_double)))).in_dll(_lib, "diffuseRadiance")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 108
for _lib in _libs.values():
    try:
        radianceNorm = (c_double * int(2)).in_dll(_lib, "radianceNorm")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 111
for _lib in _libs.values():
    try:
        resultIntegralFormat = (c_int).in_dll(_lib, "resultIntegralFormat")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 112
for _lib in _libs.values():
    try:
        resultIntegralMode = (c_int).in_dll(_lib, "resultIntegralMode")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 113
for _lib in _libs.values():
    try:
        resultIntegralFilename = (String).in_dll(_lib, "resultIntegralFilename")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 115
for _lib in _libs.values():
    try:
        resultIntegralFilenameDiffuse = (String).in_dll(_lib, "resultIntegralFilenameDiffuse")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 118
for _lib in _libs.values():
    try:
        resultIntegralCompress = (c_int).in_dll(_lib, "resultIntegralCompress")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 121
for _lib in _libs.values():
    try:
        cameraType = (c_int).in_dll(_lib, "cameraType")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 123
for _lib in _libs.values():
    try:
        nPixels = (c_int).in_dll(_lib, "nPixels")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 124
for _lib in _libs.values():
    try:
        rpp = (c_int).in_dll(_lib, "rpp")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 127
for _lib in _libs.values():
    try:
        focalPoint = (triplet).in_dll(_lib, "focalPoint")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 129
for _lib in _libs.values():
    try:
        principalPoint = (triplet).in_dll(_lib, "principalPoint")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 130
for _lib in _libs.values():
    try:
        cameraX = (triplet).in_dll(_lib, "cameraX")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 130
for _lib in _libs.values():
    try:
        cameraY = (triplet).in_dll(_lib, "cameraY")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 132
for _lib in _libs.values():
    try:
        imagingPlaneDimensions = (c_double * int(2)).in_dll(_lib, "imagingPlaneDimensions")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 135
for _lib in _libs.values():
    try:
        psf = (POINTER(c_double)).in_dll(_lib, "psf")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 136
for _lib in _libs.values():
    try:
        nPsf = (c_int * int(2)).in_dll(_lib, "nPsf")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 137
for _lib in _libs.values():
    try:
        psfmap = (POINTER(c_int) * int(2)).in_dll(_lib, "psfmap")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 138
for _lib in _libs.values():
    try:
        psfSampleCount = (POINTER(c_int)).in_dll(_lib, "psfSampleCount")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 139
for _lib in _libs.values():
    try:
        psfModulation = (POINTER(c_double)).in_dll(_lib, "psfModulation")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 142
for _lib in _libs.values():
    try:
        pulse = (POINTER(c_double)).in_dll(_lib, "pulse")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 143
for _lib in _libs.values():
    try:
        nPulse = (c_int).in_dll(_lib, "nPulse")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 144
for _lib in _libs.values():
    try:
        pulseResolution = (c_double).in_dll(_lib, "pulseResolution")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 145
for _lib in _libs.values():
    try:
        pulseStart = (c_double).in_dll(_lib, "pulseStart")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 146
for _lib in _libs.values():
    try:
        pulseType = (String).in_dll(_lib, "pulseType")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 147
for _lib in _libs.values():
    try:
        pulseSD = (c_double).in_dll(_lib, "pulseSD")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 148
for _lib in _libs.values():
    try:
        pulseLength = (c_double).in_dll(_lib, "pulseLength")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 149
for _lib in _libs.values():
    try:
        pulseSamples = (c_int).in_dll(_lib, "pulseSamples")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 150
for _lib in _libs.values():
    try:
        pulseIPFile = (String).in_dll(_lib, "pulseIPFile")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 151
for _lib in _libs.values():
    try:
        pulseOPFile = (String).in_dll(_lib, "pulseOPFile")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 152
for _lib in _libs.values():
    try:
        pulseSample = (POINTER(c_double)).in_dll(_lib, "pulseSample")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 153
for _lib in _libs.values():
    try:
        pulseStep = (c_double).in_dll(_lib, "pulseStep")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 154
for _lib in _libs.values():
    try:
        cumulativePulse = (POINTER(c_double)).in_dll(_lib, "cumulativePulse")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 157
for _lib in _libs.values():
    try:
        nS = (c_int * int(2)).in_dll(_lib, "nS")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 159
for _lib in _libs.values():
    try:
        focalAxis = (triplet).in_dll(_lib, "focalAxis")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 160
for _lib in _libs.values():
    try:
        focalLength = (c_double).in_dll(_lib, "focalLength")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 162
for _lib in _libs.values():
    try:
        twist = (c_double).in_dll(_lib, "twist")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 162
for _lib in _libs.values():
    try:
        zenith = (c_double).in_dll(_lib, "zenith")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 162
for _lib in _libs.values():
    try:
        azimuth = (c_double).in_dll(_lib, "azimuth")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 164
for _lib in _libs.values():
    try:
        fov = (c_double * int(2)).in_dll(_lib, "fov")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 165
for _lib in _libs.values():
    try:
        fovMax = (c_double).in_dll(_lib, "fovMax")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 166
for _lib in _libs.values():
    try:
        aspectRatio = (c_double).in_dll(_lib, "aspectRatio")
        break
    except:
        pass

# /Users/plewis/librat/src/ratFront.h: 179
class struct_anon_68(Structure):
    pass

struct_anon_68.__slots__ = [
    'nBands',
    'doSumImage',
    'hit',
    'sumImage',
    'sumRayLength',
    'nScatteredImages',
    'scatteredImages',
]
struct_anon_68._fields_ = [
    ('nBands', c_int),
    ('doSumImage', c_int),
    ('hit', POINTER(c_int)),
    ('sumImage', POINTER(c_double)),
    ('sumRayLength', POINTER(c_double)),
    ('nScatteredImages', c_int),
    ('scatteredImages', POINTER(POINTER(c_double))),
]

RATsample = struct_anon_68# /Users/plewis/librat/src/ratFront.h: 179

# /Users/plewis/librat/src/ratFront.h: 182
if _libs["libratlib.so"].has("RATreadSpectra", "cdecl"):
    RATreadSpectra = _libs["libratlib.so"].get("RATreadSpectra", "cdecl")
    RATreadSpectra.argtypes = [String]
    RATreadSpectra.restype = POINTER(POINTER(c_double))

# /Users/plewis/librat/src/ratFront.h: 186
if _libs["libratlib.so"].has("RATputImage", "cdecl"):
    RATputImage = _libs["libratlib.so"].get("RATputImage", "cdecl")
    RATputImage.argtypes = [String, POINTER(None), c_int, c_int, c_int, c_int]
    RATputImage.restype = c_int

# /Users/plewis/librat/src/ratFront.h: 193
if _libs["libratlib.so"].has("RATgetImage", "cdecl"):
    RATgetImage = _libs["libratlib.so"].get("RATgetImage", "cdecl")
    RATgetImage.argtypes = [String, POINTER(c_int), POINTER(c_int), POINTER(c_int), POINTER(c_int), POINTER(c_int)]
    RATgetImage.restype = POINTER(c_ubyte)
    RATgetImage.errcheck = lambda v,*a : cast(v, c_void_p)

# /Users/plewis/librat/src/ratFront.h: 194
if _libs["libratlib.so"].has("RATgetImageDouble", "cdecl"):
    RATgetImageDouble = _libs["libratlib.so"].get("RATgetImageDouble", "cdecl")
    RATgetImageDouble.argtypes = [String, POINTER(c_int), POINTER(c_int), POINTER(c_int), POINTER(c_int)]
    RATgetImageDouble.restype = POINTER(c_ubyte)
    RATgetImageDouble.errcheck = lambda v,*a : cast(v, c_void_p)

# /Users/plewis/librat/src/ratFront.h: 195
if _libs["libratlib.so"].has("RATgenerateGaussian", "cdecl"):
    RATgenerateGaussian = _libs["libratlib.so"].get("RATgenerateGaussian", "cdecl")
    RATgenerateGaussian.argtypes = [String, c_int, c_int, c_double, c_double, c_double, c_double, c_double, c_double]
    RATgenerateGaussian.restype = POINTER(c_double)

# /Users/plewis/librat/src/imagelib.h: 38
class struct_anon_69(Structure):
    pass

struct_anon_69.__slots__ = [
    'statsSet',
    'min',
    'max',
]
struct_anon_69._fields_ = [
    ('statsSet', c_int),
    ('min', c_double * int(1280)),
    ('max', c_double * int(1280)),
]

Stats = struct_anon_69# /Users/plewis/librat/src/imagelib.h: 38

# /Users/plewis/librat/src/imagelib.h: 119
for _lib in _libs.values():
    try:
        verbose = (c_int).in_dll(_lib, "verbose")
        break
    except:
        pass

# /Users/plewis/librat/src/imagelib.h: 120
for _lib in _libs.values():
    try:
        allocatedFlag = (c_int).in_dll(_lib, "allocatedFlag")
        break
    except:
        pass

# /Users/plewis/librat/src/imagelib.h: 122
for _lib in _libs.values():
    try:
        openFlag = (c_int).in_dll(_lib, "openFlag")
        break
    except:
        pass

# /Users/plewis/librat/src/imagelib.h: 124
try:
    mmap = (c_int).in_dll(_libs["libratlib.so"], "mmap")
except:
    pass

# /Users/plewis/librat/src/imagelib.h: 126
for _lib in _libs.values():
    try:
        image_format = (c_int).in_dll(_lib, "image_format")
        break
    except:
        pass

# /Users/plewis/librat/src/imagelib.h: 127
for _lib in _libs.values():
    try:
        fd = (c_int).in_dll(_lib, "fd")
        break
    except:
        pass

# /Users/plewis/librat/src/imagelib.h: 131
if _libs["libratlib.so"].has("setImageStyle", "cdecl"):
    setImageStyle = _libs["libratlib.so"].get("setImageStyle", "cdecl")
    setImageStyle.argtypes = []
    setImageStyle.restype = None

# /Users/plewis/librat/src/imagelib.h: 132
if _libs["libratlib.so"].has("getImageStyle", "cdecl"):
    getImageStyle = _libs["libratlib.so"].get("getImageStyle", "cdecl")
    getImageStyle.argtypes = []
    getImageStyle.restype = c_int

# /Users/plewis/librat/src/imagelib.h: 133
if _libs["libratlib.so"].has("setImageDefaults", "cdecl"):
    setImageDefaults = _libs["libratlib.so"].get("setImageDefaults", "cdecl")
    setImageDefaults.argtypes = []
    setImageDefaults.restype = c_int

# /Users/plewis/librat/src/imagelib.h: 134
if _libs["libratlib.so"].has("getImageBlockSize", "cdecl"):
    getImageBlockSize = _libs["libratlib.so"].get("getImageBlockSize", "cdecl")
    getImageBlockSize.argtypes = []
    getImageBlockSize.restype = c_int

# /Users/plewis/librat/src/imagelib.h: 135
if _libs["libratlib.so"].has("setImageBlockSize", "cdecl"):
    setImageBlockSize = _libs["libratlib.so"].get("setImageBlockSize", "cdecl")
    setImageBlockSize.argtypes = []
    setImageBlockSize.restype = None

# /Users/plewis/librat/src/imagelib.h: 136
if _libs["libratlib.so"].has("getDataSize", "cdecl"):
    getDataSize = _libs["libratlib.so"].get("getDataSize", "cdecl")
    getDataSize.argtypes = []
    getDataSize.restype = c_int

# /Users/plewis/librat/src/imagelib.h: 137
if _libs["libratlib.so"].has("allocateImage", "cdecl"):
    allocateImage = _libs["libratlib.so"].get("allocateImage", "cdecl")
    allocateImage.argtypes = []
    allocateImage.restype = c_int

# /Users/plewis/librat/src/imagelib.h: 138
if _libs["libratlib.so"].has("writeImage", "cdecl"):
    writeImage = _libs["libratlib.so"].get("writeImage", "cdecl")
    writeImage.argtypes = []
    writeImage.restype = c_int

# /Users/plewis/librat/src/imagelib.h: 139
if _libs["libratlib.so"].has("readImage", "cdecl"):
    readImage = _libs["libratlib.so"].get("readImage", "cdecl")
    readImage.argtypes = []
    readImage.restype = c_int

# /Users/plewis/librat/src/imagelib.h: 140
if _libs["libratlib.so"].has("imageArrayIndex", "cdecl"):
    imageArrayIndex = _libs["libratlib.so"].get("imageArrayIndex", "cdecl")
    imageArrayIndex.argtypes = []
    imageArrayIndex.restype = c_int

# /Users/plewis/librat/src/imagelib.h: 142
if _libs["libratlib.so"].has("getPixel", "cdecl"):
    getPixel = _libs["libratlib.so"].get("getPixel", "cdecl")
    getPixel.argtypes = []
    getPixel.restype = None

# /Users/plewis/librat/src/imagelib.h: 143
if _libs["libratlib.so"].has("putPixel", "cdecl"):
    putPixel = _libs["libratlib.so"].get("putPixel", "cdecl")
    putPixel.argtypes = []
    putPixel.restype = c_int

# /Users/plewis/librat/src/imagelib.h: 144
if _libs["libratlib.so"].has("openImage", "cdecl"):
    openImage = _libs["libratlib.so"].get("openImage", "cdecl")
    openImage.argtypes = []
    openImage.restype = c_int

# /Users/plewis/librat/src/imagelib.h: 145
if _libs["libratlib.so"].has("getImageFormat", "cdecl"):
    getImageFormat = _libs["libratlib.so"].get("getImageFormat", "cdecl")
    getImageFormat.argtypes = []
    getImageFormat.restype = c_int

# /Users/plewis/librat/src/imagelib.h: 147
if _libs["libratlib.so"].has("copyImageHeader", "cdecl"):
    copyImageHeader = _libs["libratlib.so"].get("copyImageHeader", "cdecl")
    copyImageHeader.argtypes = []
    copyImageHeader.restype = c_int

# /Users/plewis/librat/src/imagelib.h: 148
if _libs["libratlib.so"].has("setImageFormat", "cdecl"):
    setImageFormat = _libs["libratlib.so"].get("setImageFormat", "cdecl")
    setImageFormat.argtypes = []
    setImageFormat.restype = None

# /Users/plewis/librat/src/imagelib.h: 149
if _libs["libratlib.so"].has("setImageRows", "cdecl"):
    setImageRows = _libs["libratlib.so"].get("setImageRows", "cdecl")
    setImageRows.argtypes = []
    setImageRows.restype = None

# /Users/plewis/librat/src/imagelib.h: 150
if _libs["libratlib.so"].has("setImageCols", "cdecl"):
    setImageCols = _libs["libratlib.so"].get("setImageCols", "cdecl")
    setImageCols.argtypes = []
    setImageCols.restype = None

# /Users/plewis/librat/src/imagelib.h: 151
if _libs["libratlib.so"].has("setImageFrames", "cdecl"):
    setImageFrames = _libs["libratlib.so"].get("setImageFrames", "cdecl")
    setImageFrames.argtypes = []
    setImageFrames.restype = None

# /Users/plewis/librat/src/imagelib.h: 152
if _libs["libratlib.so"].has("getImageFrames", "cdecl"):
    getImageFrames = _libs["libratlib.so"].get("getImageFrames", "cdecl")
    getImageFrames.argtypes = []
    getImageFrames.restype = c_int

# /Users/plewis/librat/src/imagelib.h: 153
if _libs["libratlib.so"].has("getImageRows", "cdecl"):
    getImageRows = _libs["libratlib.so"].get("getImageRows", "cdecl")
    getImageRows.argtypes = []
    getImageRows.restype = c_int

# /Users/plewis/librat/src/imagelib.h: 154
if _libs["libratlib.so"].has("getImageCols", "cdecl"):
    getImageCols = _libs["libratlib.so"].get("getImageCols", "cdecl")
    getImageCols.argtypes = []
    getImageCols.restype = c_int

# /Users/plewis/librat/src/imagelib.h: 156
if _libs["libratlib.so"].has("pixelFloat", "cdecl"):
    pixelFloat = _libs["libratlib.so"].get("pixelFloat", "cdecl")
    pixelFloat.argtypes = []
    pixelFloat.restype = c_float

# /Users/plewis/librat/src/imagelib.h: 157
if _libs["libratlib.so"].has("pixelShort", "cdecl"):
    pixelShort = _libs["libratlib.so"].get("pixelShort", "cdecl")
    pixelShort.argtypes = []
    pixelShort.restype = c_short

# /Users/plewis/librat/src/imagelib.h: 158
if _libs["libratlib.so"].has("pixelInt", "cdecl"):
    pixelInt = _libs["libratlib.so"].get("pixelInt", "cdecl")
    pixelInt.argtypes = []
    pixelInt.restype = c_int

# /Users/plewis/librat/src/imagelib.h: 159
if _libs["libratlib.so"].has("pixelChar", "cdecl"):
    pixelChar = _libs["libratlib.so"].get("pixelChar", "cdecl")
    pixelChar.argtypes = []
    pixelChar.restype = c_ubyte

# /Users/plewis/librat/src/imagelib.h: 160
if _libs["libratlib.so"].has("pixelByte", "cdecl"):
    pixelByte = _libs["libratlib.so"].get("pixelByte", "cdecl")
    pixelByte.argtypes = []
    pixelByte.restype = c_ubyte

# /Users/plewis/librat/src/imagelib.h: 161
if _libs["libratlib.so"].has("setArrayCheckFlag", "cdecl"):
    setArrayCheckFlag = _libs["libratlib.so"].get("setArrayCheckFlag", "cdecl")
    setArrayCheckFlag.argtypes = []
    setArrayCheckFlag.restype = None

# /Users/plewis/librat/src/imagelib.h: 162
if _libs["libratlib.so"].has("getArrayCheckFlag", "cdecl"):
    getArrayCheckFlag = _libs["libratlib.so"].get("getArrayCheckFlag", "cdecl")
    getArrayCheckFlag.argtypes = []
    getArrayCheckFlag.restype = c_int

# /Users/plewis/librat/src/imagelib.h: 163
if _libs["libratlib.so"].has("getArrayAccess", "cdecl"):
    getArrayAccess = _libs["libratlib.so"].get("getArrayAccess", "cdecl")
    getArrayAccess.argtypes = []
    getArrayAccess.restype = c_int

# /Users/plewis/librat/src/imagelib.h: 164
if _libs["libratlib.so"].has("setArrayAccess", "cdecl"):
    setArrayAccess = _libs["libratlib.so"].get("setArrayAccess", "cdecl")
    setArrayAccess.argtypes = []
    setArrayAccess.restype = None

# /Users/plewis/librat/src/imagelib.h: 165
if _libs["libratlib.so"].has("queryImageStyle", "cdecl"):
    queryImageStyle = _libs["libratlib.so"].get("queryImageStyle", "cdecl")
    queryImageStyle.argtypes = []
    queryImageStyle.restype = c_int

# /Users/plewis/librat/src/imagelib.h: 166
if _libs["libratlib.so"].has("getImageName", "cdecl"):
    getImageName = _libs["libratlib.so"].get("getImageName", "cdecl")
    getImageName.argtypes = []
    if sizeof(c_int) == sizeof(c_void_p):
        getImageName.restype = ReturnString
    else:
        getImageName.restype = String
        getImageName.errcheck = ReturnString

# /Users/plewis/librat/src/imagelib.h: 167
if _libs["libratlib.so"].has("setImageName", "cdecl"):
    setImageName = _libs["libratlib.so"].get("setImageName", "cdecl")
    setImageName.argtypes = []
    setImageName.restype = c_int

# /Users/plewis/librat/src/imagelib.h: 170
if _libs["libratlib.so"].has("getImageVerbose", "cdecl"):
    getImageVerbose = _libs["libratlib.so"].get("getImageVerbose", "cdecl")
    getImageVerbose.argtypes = []
    getImageVerbose.restype = c_int

# /Users/plewis/librat/src/imagelib.h: 171
if _libs["libratlib.so"].has("setImageVerbose", "cdecl"):
    setImageVerbose = _libs["libratlib.so"].get("setImageVerbose", "cdecl")
    setImageVerbose.argtypes = []
    setImageVerbose.restype = None

# /Users/plewis/librat/src/imagelib.h: 171
if _libs["libratlib.so"].has("unsetImageVerbose", "cdecl"):
    unsetImageVerbose = _libs["libratlib.so"].get("unsetImageVerbose", "cdecl")
    unsetImageVerbose.argtypes = []
    unsetImageVerbose.restype = None

# /Users/plewis/librat/src/imagelib.h: 172
if _libs["libratlib.so"].has("copyImage", "cdecl"):
    copyImage = _libs["libratlib.so"].get("copyImage", "cdecl")
    copyImage.argtypes = []
    copyImage.restype = c_int

# /Users/plewis/librat/src/imagelib.h: 174
if _libs["libratlib.so"].has("getPixelFloat", "cdecl"):
    getPixelFloat = _libs["libratlib.so"].get("getPixelFloat", "cdecl")
    getPixelFloat.argtypes = []
    getPixelFloat.restype = c_float

# /Users/plewis/librat/src/imagelib.h: 175
if _libs["libratlib.so"].has("getPixelInt", "cdecl"):
    getPixelInt = _libs["libratlib.so"].get("getPixelInt", "cdecl")
    getPixelInt.argtypes = []
    getPixelInt.restype = c_int

# /Users/plewis/librat/src/imagelib.h: 176
if _libs["libratlib.so"].has("getPixelShort", "cdecl"):
    getPixelShort = _libs["libratlib.so"].get("getPixelShort", "cdecl")
    getPixelShort.argtypes = []
    getPixelShort.restype = c_short

# /Users/plewis/librat/src/imagelib.h: 177
if _libs["libratlib.so"].has("getPixelChar", "cdecl"):
    getPixelChar = _libs["libratlib.so"].get("getPixelChar", "cdecl")
    getPixelChar.argtypes = []
    getPixelChar.restype = c_ubyte

# /Users/plewis/librat/src/imagelib.h: 178
if _libs["libratlib.so"].has("getPixelByte", "cdecl"):
    getPixelByte = _libs["libratlib.so"].get("getPixelByte", "cdecl")
    getPixelByte.argtypes = []
    getPixelByte.restype = c_ubyte

# /Users/plewis/librat/src/imagelib.h: 179
if _libs["libratlib.so"].has("getValidPixelFloat", "cdecl"):
    getValidPixelFloat = _libs["libratlib.so"].get("getValidPixelFloat", "cdecl")
    getValidPixelFloat.argtypes = []
    getValidPixelFloat.restype = c_float

# /Users/plewis/librat/src/imagelib.h: 180
if _libs["libratlib.so"].has("getValidPixelInt", "cdecl"):
    getValidPixelInt = _libs["libratlib.so"].get("getValidPixelInt", "cdecl")
    getValidPixelInt.argtypes = []
    getValidPixelInt.restype = c_int

# /Users/plewis/librat/src/imagelib.h: 181
if _libs["libratlib.so"].has("getValidPixelChar", "cdecl"):
    getValidPixelChar = _libs["libratlib.so"].get("getValidPixelChar", "cdecl")
    getValidPixelChar.argtypes = []
    getValidPixelChar.restype = c_ubyte

# /Users/plewis/librat/src/imagelib.h: 182
if _libs["libratlib.so"].has("getValidPixelByte", "cdecl"):
    getValidPixelByte = _libs["libratlib.so"].get("getValidPixelByte", "cdecl")
    getValidPixelByte.argtypes = []
    getValidPixelByte.restype = c_ubyte

# /Users/plewis/librat/src/imagelib.h: 183
if _libs["libratlib.so"].has("getValidPixelShort", "cdecl"):
    getValidPixelShort = _libs["libratlib.so"].get("getValidPixelShort", "cdecl")
    getValidPixelShort.argtypes = []
    getValidPixelShort.restype = c_short

# /Users/plewis/librat/src/imagelib.h: 184
if _libs["libratlib.so"].has("setValueFromInt", "cdecl"):
    setValueFromInt = _libs["libratlib.so"].get("setValueFromInt", "cdecl")
    setValueFromInt.argtypes = []
    setValueFromInt.restype = None

# /Users/plewis/librat/src/imagelib.h: 185
if _libs["libratlib.so"].has("setValueFromDouble", "cdecl"):
    setValueFromDouble = _libs["libratlib.so"].get("setValueFromDouble", "cdecl")
    setValueFromDouble.argtypes = []
    setValueFromDouble.restype = None

# /Users/plewis/librat/src/imagelib.h: 186
if _libs["libratlib.so"].has("setValueFromFloat", "cdecl"):
    setValueFromFloat = _libs["libratlib.so"].get("setValueFromFloat", "cdecl")
    setValueFromFloat.argtypes = []
    setValueFromFloat.restype = None

# /Users/plewis/librat/src/imagelib.h: 187
if _libs["libratlib.so"].has("setValueFromChar", "cdecl"):
    setValueFromChar = _libs["libratlib.so"].get("setValueFromChar", "cdecl")
    setValueFromChar.argtypes = []
    setValueFromChar.restype = None

# /Users/plewis/librat/src/imagelib.h: 188
if _libs["libratlib.so"].has("setValueFromByte", "cdecl"):
    setValueFromByte = _libs["libratlib.so"].get("setValueFromByte", "cdecl")
    setValueFromByte.argtypes = []
    setValueFromByte.restype = None

# /Users/plewis/librat/src/imagelib.h: 189
if _libs["libratlib.so"].has("setValueFromShort", "cdecl"):
    setValueFromShort = _libs["libratlib.so"].get("setValueFromShort", "cdecl")
    setValueFromShort.argtypes = []
    setValueFromShort.restype = None

# /Users/plewis/librat/src/image_formats.h: 7
if _libs["libratlib.so"].has("isHips", "cdecl"):
    isHips = _libs["libratlib.so"].get("isHips", "cdecl")
    isHips.argtypes = []
    isHips.restype = c_int

# /Users/plewis/librat/src/image_formats.h: 8
if _libs["libratlib.so"].has("isEnvi", "cdecl"):
    isEnvi = _libs["libratlib.so"].get("isEnvi", "cdecl")
    isEnvi.argtypes = []
    isEnvi.restype = c_int

# /Users/plewis/librat/src/image_formats.h: 19
try:
    set_format = (POINTER(POINTER(CFUNCTYPE(UNCHECKED(c_int), )))).in_dll(_libs["libratlib.so"], "set_format")
except:
    pass

# /Users/plewis/librat/src/image_formats.h: 20
try:
    format_array = (POINTER(c_int)).in_dll(_libs["libratlib.so"], "format_array")
except:
    pass

# /Users/plewis/librat/src/hipl_format.h: 49
try:
    PFBYTE = 0
except:
    pass

# /Users/plewis/librat/src/hipl_format.h: 50
try:
    PFSHORT = 1
except:
    pass

# /Users/plewis/librat/src/hipl_format.h: 51
try:
    PFINT = 2
except:
    pass

# /Users/plewis/librat/src/hipl_format.h: 52
try:
    PFFLOAT = 3
except:
    pass

# /Users/plewis/librat/src/hipl_format.h: 53
try:
    PFCOMPLEX = 4
except:
    pass

# /Users/plewis/librat/src/hipl_format.h: 54
try:
    PFASCII = 5
except:
    pass

# /Users/plewis/librat/src/hipl_format.h: 56
try:
    MSBFIRST = 1
except:
    pass

# /Users/plewis/librat/src/hipl_format.h: 57
try:
    LSBFIRST = 2
except:
    pass

# /Users/plewis/librat/src/hipl_format.h: 59
try:
    PFMSBF = 30
except:
    pass

# /Users/plewis/librat/src/hipl_format.h: 60
try:
    PFLSBF = 31
except:
    pass

# /Users/plewis/librat/src/hipl_format.h: 63
try:
    TRUE = 1
except:
    pass

# /Users/plewis/librat/src/hipl_format.h: 67
try:
    FALSE = 0
except:
    pass

# /Users/plewis/librat/src/hipl_format.h: 75
def halloc(A, B):
    return (calloc (A, B))

INT = c_int# /Users/plewis/librat/src/define_float.h: 5

DOUBLE = c_double# /Users/plewis/librat/src/define_float.h: 6

FLOAT = c_double# /Users/plewis/librat/src/define_float.h: 7

# /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/sys/param.h: 215
def MIN(a, b):
    return (a < b) and a or b

# /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/sys/param.h: 218
def MAX(a, b):
    return (a > b) and a or b

# /Users/plewis/librat/src/useful3.h: 22
def MMAX(X, Y):
    return (MAX (X, Y))

# /Users/plewis/librat/src/useful3.h: 23
def MMIN(X, Y):
    return (MIN (X, Y))

# /Users/plewis/librat/src/useful3.h: 26
def ABS(X):
    return (X < 0) and (-X) or X

# /Users/plewis/librat/src/useful3.h: 27
def AVERAGE(X, Y):
    return ((X + Y) / 2.0)

# /Users/plewis/librat/src/useful3.h: 37
def DTOR(alpha):
    return ((alpha * M_PI) / 180.0)

# /Users/plewis/librat/src/useful3.h: 38
def RTOD(alpha):
    return ((alpha * 180.0) / M_PI)

# /Users/plewis/librat/src/useful3.h: 39
try:
    FATAL = 1
except:
    pass

# /Users/plewis/librat/src/useful3.h: 40
try:
    NON_FATAL = 0
except:
    pass

# /Users/plewis/librat/src/wavefront.h: 13
try:
    BIG = 1000000000.0
except:
    pass

# /Users/plewis/librat/src/wavefront.h: 14
try:
    RAT_N_BLOCKS = 1000
except:
    pass

# /Users/plewis/librat/src/intersect_objects.h: 13
try:
    BBOX = 0
except:
    pass

# /Users/plewis/librat/src/intersect_objects.h: 14
try:
    FO = 1
except:
    pass

# /Users/plewis/librat/src/intersect_objects.h: 15
try:
    SPHERE = 2
except:
    pass

# /Users/plewis/librat/src/intersect_objects.h: 16
try:
    CYLINDER = 3
except:
    pass

# /Users/plewis/librat/src/intersect_objects.h: 17
try:
    BEZIER = 4
except:
    pass

# /Users/plewis/librat/src/intersect_objects.h: 18
try:
    PLANE = 5
except:
    pass

# /Users/plewis/librat/src/intersect_objects.h: 19
try:
    CLONE = 6
except:
    pass

# /Users/plewis/librat/src/intersect_objects.h: 20
try:
    DEM = 7
except:
    pass

# /Users/plewis/librat/src/intersect_objects.h: 21
try:
    SPHERICAL_DEM = 8
except:
    pass

# /Users/plewis/librat/src/intersect_objects.h: 22
try:
    ELLIPSE = 9
except:
    pass

# /Users/plewis/librat/src/intersect_objects.h: 23
try:
    VOLUMETRIC_SPHERE = 10
except:
    pass

# /Users/plewis/librat/src/intersect_objects.h: 24
try:
    VOLUMETRIC_CYLINDER = 11
except:
    pass

# /Users/plewis/librat/src/intersect_objects.h: 25
try:
    VOLUMETRIC_ELLIPSE = 12
except:
    pass

# /Users/plewis/librat/src/intersect_objects.h: 26
try:
    VOLUMETRIC_SPHEROID = 25
except:
    pass

# /Users/plewis/librat/src/intersect_objects.h: 27
try:
    CLOSED_CYLINDER = 13
except:
    pass

# /Users/plewis/librat/src/intersect_objects.h: 28
try:
    DISK = 14
except:
    pass

# /Users/plewis/librat/src/intersect_objects.h: 29
try:
    BILINEARPATCH = 15
except:
    pass

# /Users/plewis/librat/src/intersect_objects.h: 30
try:
    SPHEROID = 26
except:
    pass

# /Users/plewis/librat/src/intersect_objects.h: 32
try:
    MAX_OBJECT_TYPES = 16
except:
    pass

# /Users/plewis/librat/src/intersect_objects.h: 34
try:
    MAX_TREE_DEPTH = 20
except:
    pass

# /Users/plewis/librat/src/materials.h: 42
try:
    SRM = 0
except:
    pass

# /Users/plewis/librat/src/materials.h: 43
try:
    BUMPMAP = 1
except:
    pass

# /Users/plewis/librat/src/materials.h: 44
try:
    BRDF_LUT = 2
except:
    pass

# /Users/plewis/librat/src/materials.h: 45
try:
    PVD_BRDF_MODEL = 3
except:
    pass

# /Users/plewis/librat/src/materials.h: 46
try:
    HIPL_TEXTURE_MAP = 4
except:
    pass

# /Users/plewis/librat/src/materials.h: 47
try:
    AHMADD_BRDF_MODEL = 5
except:
    pass

# /Users/plewis/librat/src/materials.h: 48
try:
    BEGUE_BRDF_MODEL = 6
except:
    pass

# /Users/plewis/librat/src/materials.h: 49
try:
    PROSPECT_REFLECTANCE_TRANSMITTANCE_MODEL = 7
except:
    pass

# /Users/plewis/librat/src/materials.h: 50
try:
    PROSPECT_REFLECTANCE_MODEL = 8
except:
    pass

# /Users/plewis/librat/src/materials.h: 51
try:
    VOLUMETRIC = 9
except:
    pass

# /Users/plewis/librat/src/materials.h: 52
try:
    M_TRANSPARENT = 10
except:
    pass

# /Users/plewis/librat/src/materials.h: 53
try:
    RPV = 11
except:
    pass

# /Users/plewis/librat/src/materials.h: 55
try:
    REFLECTANCE = 0
except:
    pass

# /Users/plewis/librat/src/materials.h: 56
try:
    TRANSMITTANCE = 1
except:
    pass

# /Users/plewis/librat/src/prat_objects.h: 182
try:
    TRIANGULAR_REPEAT = 1
except:
    pass

# /Users/plewis/librat/src/prat_objects.h: 183
try:
    SQUARE_REPEAT = 0
except:
    pass

# /Users/plewis/librat/src/prat_objects.h: 235
try:
    VIEWER = 0
except:
    pass

# /Users/plewis/librat/src/prat_objects.h: 236
try:
    ILLUMINATION = 1
except:
    pass

# /Users/plewis/librat/src/sensor_rsr.h: 4
try:
    MAX_NO_OF_RSR_SAMPLES = 2
except:
    pass

# /Users/plewis/librat/src/sensor_rsr.h: 5
try:
    MAX_NO_RSR_SAMPS = 2
except:
    pass

# /Users/plewis/librat/src/camera.h: 66
try:
    ALBEDO = 0
except:
    pass

# /Users/plewis/librat/src/camera.h: 67
try:
    SPHERICAL = 1
except:
    pass

# /Users/plewis/librat/src/camera.h: 68
try:
    PLANAR = 2
except:
    pass

# /Users/plewis/librat/src/camera.h: 69
try:
    PLANAR2 = 3
except:
    pass

# /Users/plewis/librat/src/globals.h: 1
try:
    RAY_LENGTH_TOL = 1e-05
except:
    pass

# /Users/plewis/librat/src/mtllib.h: 4
try:
    DEFAULT_PRAT_MAX_MATERIALS = 1024
except:
    pass

# /Users/plewis/librat/src/element_types.h: 1
try:
    FILE_END = 9
except:
    pass

# /Users/plewis/librat/src/element_types.h: 3
try:
    UNKNOWN_TYPE = 0
except:
    pass

# /Users/plewis/librat/src/element_types.h: 4
try:
    OPEN_BBOX_RET = 100
except:
    pass

# /Users/plewis/librat/src/element_types.h: 5
try:
    OPEN_BBOX = '!{'
except:
    pass

# /Users/plewis/librat/src/element_types.h: 6
try:
    CLOSE_BBOX_RET = 1
except:
    pass

# /Users/plewis/librat/src/element_types.h: 7
try:
    CLOSE_BBOX = '!}'
except:
    pass

# /Users/plewis/librat/src/element_types.h: 8
try:
    GROUP_RET = 2
except:
    pass

# /Users/plewis/librat/src/element_types.h: 9
try:
    GROUP = 'g'
except:
    pass

# /Users/plewis/librat/src/element_types.h: 10
try:
    VERTEX_RET = 3
except:
    pass

# /Users/plewis/librat/src/element_types.h: 11
try:
    VERTEX = 'v'
except:
    pass

# /Users/plewis/librat/src/element_types.h: 12
try:
    LOCAL_RET = 4
except:
    pass

# /Users/plewis/librat/src/element_types.h: 13
try:
    LOCAL = '!local'
except:
    pass

# /Users/plewis/librat/src/element_types.h: 14
try:
    NORMAL_RET = 5
except:
    pass

# /Users/plewis/librat/src/element_types.h: 15
try:
    NORMAL1 = '!n'
except:
    pass

# /Users/plewis/librat/src/element_types.h: 16
try:
    NORMAL2 = '!normal'
except:
    pass

# /Users/plewis/librat/src/element_types.h: 17
try:
    DEFINE_RET = 6
except:
    pass

# /Users/plewis/librat/src/element_types.h: 18
try:
    DEFINE1 = '!define'
except:
    pass

# /Users/plewis/librat/src/element_types.h: 19
try:
    DEFINE2 = '#define'
except:
    pass

# /Users/plewis/librat/src/element_types.h: 20
try:
    TRIANGLE_RET = 7
except:
    pass

# /Users/plewis/librat/src/element_types.h: 21
try:
    TRIANGLE1 = 'f'
except:
    pass

# /Users/plewis/librat/src/element_types.h: 22
try:
    TRIANGLE2 = 'fo'
except:
    pass

# /Users/plewis/librat/src/element_types.h: 23
try:
    SPHERE_RET = 8
except:
    pass

# /Users/plewis/librat/src/element_types.h: 24
try:
    SPH = 'sph'
except:
    pass

# /Users/plewis/librat/src/element_types.h: 25
try:
    ELLIPSE_RET = 17
except:
    pass

# /Users/plewis/librat/src/element_types.h: 26
try:
    ELL = 'ell'
except:
    pass

# /Users/plewis/librat/src/element_types.h: 27
try:
    SPHER_RET = 26
except:
    pass

# /Users/plewis/librat/src/element_types.h: 28
try:
    SPHER = 'spheroid'
except:
    pass

# /Users/plewis/librat/src/element_types.h: 29
try:
    CYLINDER_RET = 9
except:
    pass

# /Users/plewis/librat/src/element_types.h: 30
try:
    CYL = 'cyl'
except:
    pass

# /Users/plewis/librat/src/element_types.h: 31
try:
    CLONE_RET = 10
except:
    pass

# /Users/plewis/librat/src/element_types.h: 32
try:
    CLONE1 = 'clone'
except:
    pass

# /Users/plewis/librat/src/element_types.h: 33
try:
    CLONE2 = '!clone'
except:
    pass

# /Users/plewis/librat/src/element_types.h: 34
try:
    CLONE3 = '#clone'
except:
    pass

# /Users/plewis/librat/src/element_types.h: 35
try:
    DEM_RET = 11
except:
    pass

# /Users/plewis/librat/src/element_types.h: 36
try:
    DEM1 = 'DEM'
except:
    pass

# /Users/plewis/librat/src/element_types.h: 37
try:
    DEM2 = '!DEM'
except:
    pass

# /Users/plewis/librat/src/element_types.h: 38
try:
    DEM3 = '#DEM'
except:
    pass

# /Users/plewis/librat/src/element_types.h: 39
try:
    DEM4 = 'dem'
except:
    pass

# /Users/plewis/librat/src/element_types.h: 40
try:
    DEM5 = '!dem'
except:
    pass

# /Users/plewis/librat/src/element_types.h: 41
try:
    DEM6 = '#dem'
except:
    pass

# /Users/plewis/librat/src/element_types.h: 42
try:
    PLANE_RET = 12
except:
    pass

# /Users/plewis/librat/src/element_types.h: 43
try:
    PLANE1 = 'PLANE'
except:
    pass

# /Users/plewis/librat/src/element_types.h: 44
try:
    PLANE2 = '!PLANE'
except:
    pass

# /Users/plewis/librat/src/element_types.h: 45
try:
    PLANE3 = '#PLANE'
except:
    pass

# /Users/plewis/librat/src/element_types.h: 46
try:
    PLANE4 = 'plane'
except:
    pass

# /Users/plewis/librat/src/element_types.h: 47
try:
    PLANE5 = '!plane'
except:
    pass

# /Users/plewis/librat/src/element_types.h: 48
try:
    PLANE6 = '#plane'
except:
    pass

# /Users/plewis/librat/src/element_types.h: 49
try:
    USEMTL_RET = 13
except:
    pass

# /Users/plewis/librat/src/element_types.h: 50
try:
    USEMTL = 'usemtl'
except:
    pass

# /Users/plewis/librat/src/element_types.h: 51
try:
    MTLLIB_RET = 14
except:
    pass

# /Users/plewis/librat/src/element_types.h: 52
try:
    MTLLIB = 'mtllib'
except:
    pass

# /Users/plewis/librat/src/element_types.h: 53
try:
    TRANS_RET = 15
except:
    pass

# /Users/plewis/librat/src/element_types.h: 56
try:
    TRANS = 'transformation_matrix'
except:
    pass

# /Users/plewis/librat/src/element_types.h: 57
try:
    SPHERICAL_DEM_RET = 16
except:
    pass

# /Users/plewis/librat/src/element_types.h: 58
try:
    SPHERICAL_DEM1 = 'SPHERICAL_DEM'
except:
    pass

# /Users/plewis/librat/src/element_types.h: 59
try:
    SPHERICAL_DEM2 = '!SPHERICAL_DEM'
except:
    pass

# /Users/plewis/librat/src/element_types.h: 60
try:
    SPHERICAL_DEM3 = '#SPHERICAL_DEM'
except:
    pass

# /Users/plewis/librat/src/element_types.h: 61
try:
    SPHERICAL_DEM4 = 'spherical_dem'
except:
    pass

# /Users/plewis/librat/src/element_types.h: 62
try:
    SPHERICAL_DEM5 = '!spherical_dem'
except:
    pass

# /Users/plewis/librat/src/element_types.h: 63
try:
    SPHERICAL_DEM6 = '#spherical_dem'
except:
    pass

# /Users/plewis/librat/src/element_types.h: 64
try:
    VOLUME_SPHERE_RET = 18
except:
    pass

# /Users/plewis/librat/src/element_types.h: 65
try:
    VOLUME_SP = 'vsph'
except:
    pass

# /Users/plewis/librat/src/element_types.h: 66
try:
    CLOSED_CYL = 'ccyl'
except:
    pass

# /Users/plewis/librat/src/element_types.h: 67
try:
    CLOSED_CYL_RET = 19
except:
    pass

# /Users/plewis/librat/src/element_types.h: 68
try:
    VOLUME_CYL_RET = 20
except:
    pass

# /Users/plewis/librat/src/element_types.h: 69
try:
    VOLUME_CYL = 'vcyl'
except:
    pass

# /Users/plewis/librat/src/element_types.h: 70
try:
    DISK_RET = 21
except:
    pass

# /Users/plewis/librat/src/element_types.h: 71
try:
    DISK1 = 'disc'
except:
    pass

# /Users/plewis/librat/src/element_types.h: 72
try:
    DISK2 = 'disk'
except:
    pass

# /Users/plewis/librat/src/element_types.h: 73
try:
    BILINEARPATCH_RET = 23
except:
    pass

# /Users/plewis/librat/src/element_types.h: 74
try:
    BILINEARPATCH0 = 'blp'
except:
    pass

# /Users/plewis/librat/src/element_types.h: 75
try:
    BILINEARPATCH1 = '#blp'
except:
    pass

# /Users/plewis/librat/src/element_types.h: 76
try:
    BILINEARPATCH2 = '!blp'
except:
    pass

# /Users/plewis/librat/src/element_types.h: 77
try:
    BILINEARPATCH3 = 'BiLinearPatch'
except:
    pass

# /Users/plewis/librat/src/element_types.h: 78
try:
    BILINEARPATCH4 = 'biLinearPatch'
except:
    pass

# /Users/plewis/librat/src/element_types.h: 79
try:
    BILINEARPATCH5 = '!BiLinearPatch'
except:
    pass

# /Users/plewis/librat/src/element_types.h: 80
try:
    BILINEARPATCH6 = '!biLinearPatch'
except:
    pass

# /Users/plewis/librat/src/element_types.h: 81
try:
    BILINEARPATCH7 = '#BiLinearPatch'
except:
    pass

# /Users/plewis/librat/src/element_types.h: 82
try:
    BILINEARPATCH8 = '#biLinearPatch'
except:
    pass

# /Users/plewis/librat/src/element_types.h: 83
try:
    VOLUME_ELLIPSE_RET = 22
except:
    pass

# /Users/plewis/librat/src/element_types.h: 84
try:
    VOLUME_ELLIPSE = 'vell'
except:
    pass

# /Users/plewis/librat/src/element_types.h: 85
try:
    VOLUME_SPHEROID_RET = 25
except:
    pass

# /Users/plewis/librat/src/element_types.h: 86
try:
    VOLUME_SPHEROID = 'vspheroid'
except:
    pass

# /Users/plewis/librat/src/bags.h: 1
try:
    MAX_BBOX_LIST = 128
except:
    pass

# /Users/plewis/librat/src/bags.h: 103
try:
    MAX_STRING_LENGTH = 2048
except:
    pass

# /Users/plewis/librat/src/bags.h: 104
try:
    SHORT_STRING_LENGTH = 1024
except:
    pass

# /Users/plewis/librat/src/bags.h: 105
try:
    MAX_WAVEBANDS = 1024
except:
    pass

# /Users/plewis/librat/src/bags.h: 106
try:
    MAX_SENSOR_FILES = 50
except:
    pass

# /Users/plewis/librat/src/vector_macros.h: 1
def V_DOT(a, b):
    return (((((a.x).value) * ((b.x).value)) + (((a.y).value) * ((b.y).value))) + (((a.z).value) * ((b.z).value)))

# /Users/plewis/librat/src/vector_macros.h: 2
def V_FACTOR(a, b):
    return (((a.x).value) * b)

# /Users/plewis/librat/src/vector_macros.h: 3
def V_MOD(a):
    return (sqrt ((((((a.x).value) * ((a.x).value)) + (((a.y).value) * ((a.y).value))) + (((a.z).value) * ((a.z).value)))))

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

# ./bags.h: 1
try:
    MAX_BBOX_LIST = 128
except:
    pass

# ./bags.h: 103
try:
    MAX_STRING_LENGTH = 2048
except:
    pass

# ./bags.h: 104
try:
    SHORT_STRING_LENGTH = 1024
except:
    pass

# ./bags.h: 105
try:
    MAX_WAVEBANDS = 1024
except:
    pass

# ./bags.h: 106
try:
    MAX_SENSOR_FILES = 50
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

# /Users/plewis/librat/src/prat.h: 58
def EXPONENTIAL(X):
    return (X < (-300)) and 0 or (exp (X))

# /Users/plewis/librat/src/prat.h: 59
def FSIGN(a):
    return (a < 0.0) and (-1) or 1

# /Users/plewis/librat/src/prat.h: 64
try:
    pi = M_PI
except:
    pass

# /Users/plewis/librat/src/prat.h: 65
try:
    PI_OVER_2 = M_PI_2
except:
    pass

# /Users/plewis/librat/src/prat.h: 66
try:
    PI_2 = (2.0 * M_PI)
except:
    pass

# /Users/plewis/librat/src/prat.h: 68
try:
    DIFFUSE = 2
except:
    pass

# /Users/plewis/librat/src/prat.h: 71
try:
    TTOL = 0.001
except:
    pass

# /Users/plewis/librat/src/prat.h: 72
try:
    BOXTOL = 1e-05
except:
    pass

# /Users/plewis/librat/src/prat.h: 73
try:
    RAY_TOL = 0.01
except:
    pass

# /Users/plewis/librat/src/prat.h: 74
try:
    UNITY_TOL = 0.001
except:
    pass

# /Users/plewis/librat/src/prat.h: 88
try:
    IP = 1
except:
    pass

# /Users/plewis/librat/src/prat.h: 89
try:
    OP = 2
except:
    pass

# /Users/plewis/librat/src/prat.h: 91
try:
    OLD = 0
except:
    pass

# /Users/plewis/librat/src/prat.h: 92
try:
    NEW = 1
except:
    pass

# /Users/plewis/librat/src/frat.h: 16
try:
    TRANSPARENT = 10
except:
    pass

# /Users/plewis/librat/src/rat.h: 2
try:
    MAX_SUNS = 180
except:
    pass

# /Users/plewis/librat/src/ratFront.h: 10
try:
    ORTHOGRAPHIC = 1
except:
    pass

# /Users/plewis/librat/src/ratFront.h: 11
try:
    PERSPECTIVE = 0
except:
    pass

# /Users/plewis/librat/src/ratFront.h: 12
try:
    DIFFUSE = 2
except:
    pass

# /Users/plewis/librat/src/ratFront.h: 13
try:
    MAXFIELDS = 5
except:
    pass

# /Users/plewis/librat/src/ratFront.h: 15
try:
    RESULT_INTEGRAL_MODE_WAVEBAND = 0
except:
    pass

# /Users/plewis/librat/src/ratFront.h: 16
try:
    RESULT_INTEGRAL_MODE_SCATTERINGORDER = 1
except:
    pass

# /Users/plewis/librat/src/ratFront.h: 17
try:
    RESULT_INTEGRAL_MODE_DISTANCE = 2
except:
    pass

# /Users/plewis/librat/src/ratFront.h: 18
try:
    RESULT_INTEGRAL_FORMAT_ASCII = 0
except:
    pass

# /Users/plewis/librat/src/ratFront.h: 19
try:
    RESULT_INTEGRAL_FORMAT_BINARY = 1
except:
    pass

# /Users/plewis/librat/src/ratFront.h: 41
try:
    is_bigendian = (((String (ord_if_char(pointer(___i)))).value[0]) == 0)
except:
    pass

# /Users/plewis/librat/src/imagelib.h: 6
try:
    TRUE = 1
except:
    pass

# /Users/plewis/librat/src/imagelib.h: 7
try:
    FALSE = 0
except:
    pass

# /Users/plewis/librat/src/imagelib.h: 8
try:
    BLANK_EDGES = 1
except:
    pass

# /Users/plewis/librat/src/imagelib.h: 9
try:
    WRAPAROUND = 2
except:
    pass

# /Users/plewis/librat/src/imagelib.h: 10
try:
    FATAL_ACCESS = 3
except:
    pass

# /Users/plewis/librat/src/imagelib.h: 16
try:
    CALC_STATS_STDIN = 3
except:
    pass

# /Users/plewis/librat/src/imagelib.h: 17
try:
    CALC_STATS = 5
except:
    pass

# /Users/plewis/librat/src/imagelib.h: 19
try:
    BIL = 0
except:
    pass

# /Users/plewis/librat/src/imagelib.h: 20
try:
    BSQ = 1
except:
    pass

# /Users/plewis/librat/src/imagelib.h: 23
try:
    NO_FORMAT = (-1)
except:
    pass

# /Users/plewis/librat/src/imagelib.h: 24
try:
    IMAGE_BYTE = 0
except:
    pass

# /Users/plewis/librat/src/imagelib.h: 25
try:
    IMAGE_SHORT = 1
except:
    pass

# /Users/plewis/librat/src/imagelib.h: 26
try:
    IMAGE_INT = 2
except:
    pass

# /Users/plewis/librat/src/imagelib.h: 27
try:
    IMAGE_FLOAT = 3
except:
    pass

# /Users/plewis/librat/src/imagelib.h: 28
try:
    NFORMATS = 4
except:
    pass

# /Users/plewis/librat/src/imagelib.h: 31
try:
    MAXFRAMES = 1280
except:
    pass

# /Users/plewis/librat/src/image_formats.h: 10
try:
    N_FORMATS = 2
except:
    pass

# /Users/plewis/librat/src/image_formats.h: 13
try:
    HIPSFORMAT = 1
except:
    pass

# /Users/plewis/librat/src/image_formats.h: 14
try:
    ENVIFORMAT = 2
except:
    pass

header = struct_header# /Users/plewis/librat/src/hipl_format.h: 26

Sort_Hit = struct_Sort_Hit# /Users/plewis/librat/src/intersect_objects.h: 4

Material_table_struct = struct_Material_table_struct# /Users/plewis/librat/src/materials.h: 9

Plane_struct = struct_Plane_struct# /Users/plewis/librat/src/prat_objects.h: 21

D_Facet_struct = struct_D_Facet_struct# /Users/plewis/librat/src/prat_objects.h: 58

Disk_struct = struct_Disk_struct# /Users/plewis/librat/src/prat_objects.h: 72

BiLinearPatch_struct = struct_BiLinearPatch_struct# /Users/plewis/librat/src/prat_objects.h: 85

Cylinder_struct = struct_Cylinder_struct# /Users/plewis/librat/src/prat_objects.h: 101

Ellipse_struct = struct_Ellipse_struct# /Users/plewis/librat/src/prat_objects.h: 119

Spheroid_struct = struct_Spheroid_struct# /Users/plewis/librat/src/prat_objects.h: 134

Sphere_struct = struct_Sphere_struct# /Users/plewis/librat/src/prat_objects.h: 189

D_Sphere_struct = struct_D_Sphere_struct# /Users/plewis/librat/src/prat_objects.h: 200

Spherical_Dem_struct = struct_Spherical_Dem_struct# /Users/plewis/librat/src/prat_objects.h: 222

Bezier4_struct = struct_Bezier4_struct# /Users/plewis/librat/src/prat_objects.h: 256

Facet_struct = struct_Facet_struct# /Users/plewis/librat/src/prat_objects.h: 265

Dem_struct = struct_Dem_struct# /Users/plewis/librat/src/prat_objects.h: 282

Bbox = struct_Bbox# /Users/plewis/librat/src/prat_objects.h: 342

Clone = struct_Clone# /Users/plewis/librat/src/prat_objects.h: 360

Content = struct_Content# /Users/plewis/librat/src/prat_objects.h: 297

BOX = struct_BOX# /Users/plewis/librat/src/prat_objects.h: 479

RATorderStore = struct_RATorderStore# /Users/plewis/librat/src/ratFront.h: 45

# No inserted files

# No prefix-stripping

