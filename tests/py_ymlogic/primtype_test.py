#! /usr/bin/env python3

"""PrimType に関するテスト

:file: primtype_test.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2023 Yusuke Matsunaga, All rights reserved.
"""

import pytest
from ymlogic import PrimType


def test_c0():
    ptype = PrimType("c0")
    assert ptype == PrimType.C0

    ptyoe = PrimType("C0")
    assert ptype == PrimType.C0

    assert ptype.__repr__() == "C0"

def test_c1():
    ptype = PrimType("c1")
    assert ptype == PrimType.C1

    ptype = PrimType("C1")
    assert ptype == PrimType.C1

    assert ptype.__repr__() == "C1"

def test_buff():
    ptype = PrimType("buff")
    assert ptype == PrimType.Buff

    type = PrimType("Buff")
    #assert ptype == PrimType.Buff

    #ptype = PrimType("BUFF")
    #assert ptype == PrimType.Buff

    assert ptype.__repr__() == "Buff"

def test_not():
    ptype = PrimType("not")
    assert ptype == PrimType.Not

    ptype = PrimType("Not")
    assert ptype == PrimType.Not

    ptype = PrimType("NOT")
    assert ptype == PrimType.Not

    assert ptype.__repr__() == "Not"

def test_and():
    ptype = PrimType("and")
    assert ptype == PrimType.And

    ptype = PrimType("And")
    assert ptype == PrimType.And

    ptype = PrimType("AND")
    assert ptype == PrimType.And

    assert ptype.__repr__() == "And"

def test_nand():
    ptype = PrimType("nand")
    assert ptype == PrimType.Nand

    ptype = PrimType("Nand")
    assert ptype == PrimType.Nand

    ptype = PrimType("NAND")
    assert ptype == PrimType.Nand

    assert ptype.__repr__() == "Nand"

def test_or():
    ptype = PrimType("or")
    assert ptype == PrimType.Or

    ptype = PrimType("Or")
    assert ptype == PrimType.Or

    ptype = PrimType("OR")
    assert ptype == PrimType.Or

    assert ptype.__repr__() == "Or"

def test_nor():
    ptype = PrimType("nor")
    assert ptype == PrimType.Nor

    ptype = PrimType("Nor")
    assert ptype == PrimType.Nor

    ptype = PrimType("NOR")
    assert ptype == PrimType.Nor

    assert ptype.__repr__() == "Nor"

def test_xor():
    ptype = PrimType("xor")
    assert ptype == PrimType.Xor

    ptype = PrimType("Xor")
    assert ptype == PrimType.Xor

    ptype = PrimType("XOR")
    assert ptype == PrimType.Xor

    assert ptype.__repr__() == "Xor"

def test_xnor():
    ptype = PrimType("xnor")
    assert ptype == PrimType.Xnor

    ptype = PrimType("Xnor")
    assert ptype == PrimType.Xnor

    ptype = PrimType("XNOR")
    assert ptype == PrimType.Xnor

    assert ptype.__repr__() == "Xnor"

def test_invalid():
    with pytest.raises(Exception) as e:
        ptype = PrimType("mux")
    assert e.type == ValueError
    assert str(e.value) == "mux: unknown type for PrimType"
