#! /usr/bin/env python3

"""Literal のテストプログラム

:file: literal_test.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2023 Yusuke Matsunaga, All rights reserved.
"""

import pytest
from ymlogic import Literal


def test_constructor1():
    lit = Literal()

    assert not lit.is_valid()

    assert str(lit) == "U<>"

def test_constructor2():
    var0 = 0
    lit = Literal(var0)

    assert lit.is_valid()
    assert lit.is_positive()
    assert not lit.is_negative()
    assert lit.varid == var0

    assert str(lit) == "P<0>"
    
    lit_p = lit.make_positive()
    assert lit_p == lit

    lit_n = lit.make_negative()
    assert lit_n == ~lit


def test_constructor3():
    var0 = 1
    lit = Literal(var0, inv=False)

    assert lit.is_valid()
    assert lit.is_positive()
    assert not lit.is_negative()
    assert lit.varid == var0

    assert str(lit) == "P<1>"

    lit_p = lit.make_positive()
    assert lit_p == lit

    lit_n = lit.make_negative()
    assert lit_n == ~lit


def test_constructor4():
    var0 = 2
    lit = Literal(var0, inv=True)

    assert lit.is_valid()
    assert not lit.is_positive()
    assert lit.is_negative()
    assert lit.varid == var0

    assert str(lit) == "N<2>"

    lit_p = lit.make_positive()
    assert lit_p == ~lit

    lit_n = lit.make_negative()
    assert lit_n == lit


def test_bad_constructor1():
    with pytest.raises(Exception) as e:
        _ = Literal(0, True)
    assert e.type == TypeError
    assert str(e.value) == "function takes at most 1 positional argument (2 given)"


def test_compare1():
    lit1 = Literal(0, inv=False)
    lit2 = Literal(0, inv=True)

    assert not (lit1 == lit2)
    assert (lit1 != lit2)
    assert (lit1 < lit2)
    assert (lit1 <= lit2)
    assert not (lit1 > lit2)
    assert not (lit1 >= lit2)


def test_compare2():
    lit1 = Literal(0, inv=True)
    lit2 = Literal(1, inv=False)

    assert not (lit1 == lit2)
    assert (lit1 != lit2)
    assert (lit1 < lit2)
    assert (lit1 <= lit2)
    assert not (lit1 > lit2)
    assert not (lit1 >= lit2)


def test_compare3():
    lit1 = Literal(1, inv=True)
    lit2 = Literal(0, inv=False)

    assert not (lit1 == lit2)
    assert (lit1 != lit2)
    assert not (lit1 < lit2)
    assert not (lit1 <= lit2)
    assert (lit1 > lit2)
    assert (lit1 >= lit2)


def test_compare4():
    lit1 = Literal(1, inv=True)
    lit2 = Literal(1, inv=True)

    assert (lit1 == lit2)
    assert not (lit1 != lit2)
    assert not (lit1 < lit2)
    assert (lit1 <= lit2)
    assert not (lit1 > lit2)
    assert (lit1 >= lit2)
