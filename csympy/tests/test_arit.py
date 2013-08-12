from nose.tools import raises

from csympy import Symbol, Rational, Add, Pow

def test_arit1():
    x = Symbol("x")
    y = Symbol("y")
    e = x + y
    e = x * y
    e = Rational(2)*x
    e = 2*x
    e = x + 1
    e = 1 + x

def test_arit2():
    x = Symbol("x")
    y = Symbol("y")
    assert x+x == Rational(2) * x
    assert x+x != Rational(3) * x
    assert x+y == y+x
    assert x+x == 2*x
    assert x+x == x*2
    assert x+x+x == 3*x
    assert x+y+x+x == 3*x+y

    assert not x+x == 3*x
    assert not x+x != 2*x

@raises(TypeError)
def test_arit3():
    x = Symbol("x")
    y = Symbol("y")
    e = "x"*x

def test_arit4():
    x = Symbol("x")
    y = Symbol("y")
    assert x*x == x**2
    assert x*y == y*x
    assert x*x*x == x**3
    assert x*y*x*x == x**3*y

def test_arit5():
    x = Symbol("x")
    y = Symbol("y")
    e = (x+y)**2
    f = e.expand()
    assert e == (x+y)**2
    assert e != x**2 + 2*x*y + y**2
    assert isinstance(e, Pow)
    assert f == x**2 + 2*x*y + y**2
    assert isinstance(f, Add)

def test_arit6():
    x = Symbol("x")
    y = Symbol("y")
    e = x + y
    assert str(e) == "x + y" or "y + x"
    e = x * y
    assert str(e) == "x*y" or "y*x"
    e = Rational(2)*x
    assert str(e) == "2x"
    e = 2*x
    assert str(e) == "2x"

def test_arit7():
    x = Symbol("x")
    y = Symbol("y")
    assert x - x == 0
    assert x - y != y - x
    assert 2*x - x == x
    assert 3*x - x == 2*x

    assert 2*x*y - x*y == x*y

def test_arit8():
    x = Symbol("x")
    y = Symbol("y")
    z = Symbol("z")
    assert x**y * x**x == x**(x+y)
    assert x**y * x**x * x**z == x**(x+y+z)
    assert x**y - x**y == 0
