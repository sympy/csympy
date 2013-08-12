from cython.operator cimport dereference as deref
cimport csympy
from csympy cimport rcp, RCP

class SympifyError(Exception):
    pass

cdef c2py(RCP[csympy.Basic] o):
    cdef Basic r
    if (csympy.is_a_Add(deref(o))):
        r = Add.__new__(Add)
    elif (csympy.is_a_Mul(deref(o))):
        r = Mul.__new__(Mul)
    elif (csympy.is_a_Pow(deref(o))):
        r = Pow.__new__(Pow)
    elif (csympy.is_a_Rational(deref(o))):
        # TODO: figure out how to bypass the __init__() completely:
        r = Rational.__new__(Rational, -99999)
    elif (csympy.is_a_Symbol(deref(o))):
        # TODO: figure out how to bypass the __init__() completely:
        r = Symbol.__new__(Symbol, "null")
    else:
        raise Exception("Unsupported CSymPy class.")
    r.thisptr = o
    return r

def sympify(a, raise_error=True):
    if isinstance(a, Basic):
        return a
    elif isinstance(a, (int, long)):
        return Rational(a)
    else:
        if raise_error:
            raise SympifyError("Cannot convert '%r' to a csympy type." % a)

cdef class Basic(object):
    cdef RCP[csympy.Basic] thisptr

    def __str__(self):
        return deref(self.thisptr).__str__()

    def __repr__(self):
        return self.__str__()

    def __add__(a, b):
        cdef Basic A = sympify(a, False)
        cdef Basic B = sympify(b, False)
        if A is None or B is None: return NotImplemented
        return c2py(csympy.add(A.thisptr, B.thisptr))

    def __sub__(a, b):
        cdef Basic A = sympify(a, False)
        cdef Basic B = sympify(b, False)
        if A is None or B is None: return NotImplemented
        return c2py(csympy.sub(A.thisptr, B.thisptr))

    def __mul__(a, b):
        cdef Basic A = sympify(a, False)
        cdef Basic B = sympify(b, False)
        if A is None or B is None: return NotImplemented
        return c2py(csympy.mul(A.thisptr, B.thisptr))

    # What is the purpose of "c" here?
    def __pow__(a, b, c):
        cdef Basic A = sympify(a, False)
        cdef Basic B = sympify(b, False)
        if A is None or B is None: return NotImplemented
        return c2py(csympy.pow(A.thisptr, B.thisptr))

    def __richcmp__(a, b, int op):
        cdef Basic A = sympify(a, False)
        cdef Basic B = sympify(b, False)
        if A is None or B is None: return NotImplemented
        if (op == 2):
            return csympy.eq(A.thisptr, B.thisptr)
        elif (op == 3):
            return csympy.neq(A.thisptr, B.thisptr)
        else:
            return NotImplemented

    def expand(Basic self not None):
        return c2py(csympy.expand(self.thisptr))


cdef class Symbol(Basic):

    def __cinit__(self, name):
        self.thisptr = rcp(new csympy.Symbol(name))

    def __dealloc__(self):
        self.thisptr.reset()

cdef class Rational(Basic):

    def __cinit__(self, i):
        self.thisptr = rcp(new csympy.Rational(i))

    def __dealloc__(self):
        self.thisptr.reset()

cdef class Add(Basic):

    def __dealloc__(self):
        self.thisptr.reset()

cdef class Mul(Basic):

    def __dealloc__(self):
        self.thisptr.reset()

cdef class Pow(Basic):

    def __dealloc__(self):
        self.thisptr.reset()
