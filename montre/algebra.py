import os
import sys

from ctypes import *

from distutils.sysconfig import get_python_lib

# if os.name == 'nt':
#     libmontre = windll.LoadLibrary(os.path.join(get_python_lib(), "libmontre"))
# else:
#     libmontre = cdll.LoadLibrary(os.path.join(get_python_lib(), "libmontre.so"))

if os.name == 'nt':
    libmontre = windll.LoadLibrary(os.path.join("..", "libmontre"))
else:
    libmontre = cdll.LoadLibrary(os.path.join("..", "libmontre.so"))

zone_type = POINTER(c_int64)

libmontre.zs_create.restype = c_void_p
libmontre.zs_create.argtypes = []
libmontre.zs_destroy.restype = None
libmontre.zs_destroy.argtypes = []
libmontre.zs_size.restype = c_int64
libmontre.zs_includes.restype = c_int64


libmontre.zs_append.argtypes = [c_void_p, c_int64, c_int64, c_int64, c_int64,c_int64,c_int64]
libmontre.zs_append.restype = None

libmontre.zs_append_not_anchored.argtypes = [c_void_p, c_int64, c_int64]
libmontre.zs_append_not_anchored.restype = None

libmontre.zs_get_zone.restype = zone_type
libmontre.zs_get_zone.argtypes = [c_void_p, c_int64]

class TimedRelation:

    def __init__(self, obj=None):
        if obj == None:
            self.obj = libmontre.zs_create()
        else:
            self.obj = obj

    def __del__(self):
        libmontre.zs_destroy(self.obj)

    def __len__(self):
        return libmontre.zs_size(self.obj)

    def __iter__(self):
        def get_zone(self, i):
            z = libmontre.zs_get_zone(self.obj, i)
            return (z[1], z[3], z[2], z[6], z[5], z[7])
        return (get_zone(self, i) for i in range(len(self)))

    @property
    def zones(self):
        return [((-(z[0]//2), z[1]//2, -(z[2]//2), z[3]//2, -(z[4]//2), z[5]//2), (z[0]%2, z[1]%2, z[2]%2, z[3]%2, z[4]%2, z[5]%2)) for z in self]

    def __str__(self):
        return "\n".join([
            "({bminval}{bminbnd}x{bmaxbnd}{bmaxval}, {eminval}{eminbnd}y{emaxbnd}{emaxval}, {dminval}{dminbnd}y-x{dmaxbnd}{dmaxval})".format(
            bminval=v[0], bminbnd="<" if b[0] == 0 else "<=",
            bmaxval=v[1], bmaxbnd="<" if b[1] == 0 else "<=",
            eminval=v[2], eminbnd="<" if b[2] == 0 else "<=",
            emaxval=v[3], emaxbnd="<" if b[3] == 0 else "<=",
            dminval=v[4], dminbnd="<" if b[4] == 0 else "<=",
            dmaxval=v[5], dmaxbnd="<" if b[5] == 0 else "<=",
            ) for v, b in self.zones]
        )
        # return str([values for values, bounds in self.zones])

    # def __getitem__(self, i):  # access elements in vector at index
    #     if 0 <= i < len(self):
    #         return libmontre.zs_get_zone(self.obj, i)
    #     raise IndexError('Vector index out of range')

    def append(self, bmin=0, bmax=sys.maxsize, emin=0, emax=sys.maxsize, dmin=0, dmax=sys.maxsize):
        libmontre.zs_append(self.obj, bmin, bmax, emin, emax, dmin, dmax)
        return self

    def append_not_anchored(self, begin, end):
        libmontre.zs_append_not_anchored(self.obj, begin, end)
        return self

    def absorb(self):
        retobj = libmontre.zs_create()
        libmontre.zs_filter(retobj, self.obj)
        self.obj = retobj

    @staticmethod
    def absorbed(self):
        retobj = libmontre.zs_create()
        libmontre.zs_filter(retobj, self.obj)
        return TimedRelation(retobj)

    def includes(self, other):
        return libmontre.zs_includes(self.obj, other.obj) != 0


    def restrict(self, other, a, b):
        retobj = libmontre.zs_create()
        libmontre.zs_restrict(retobj, self.obj, c_int64(a), c_int64(b))
        return TimedRelation(retobj)

    def intersect(self, other):
        retobj = libmontre.zs_create()
        libmontre.zs_intersect(retobj, self.obj, other.obj)
        return TimedRelation(retobj)

    def concatenate(self, other):
        retobj = libmontre.zs_create()
        libmontre.zs_concatenate(retobj, self.obj, other.obj)
        return TimedRelation(retobj)

    def union(self, other):
        retobj = libmontre.zs_create()
        libmontre.zs_union(retobj, self.obj, other.obj)
        return TimedRelation(retobj)

    def plus(self, other):
        retobj = libmontre.zs_create()
        libmontre.zs_plus(retobj, self.obj)
        return TimedRelation(retobj)

class Bound:
    @staticmethod
    def lt(c):
        return 2*c
    @staticmethod
    def leq(c):
        return 2*c + 1
    @staticmethod
    def gt(c):
        return -2*c
    @staticmethod
    def geq(c):
        return -2*c + 1

