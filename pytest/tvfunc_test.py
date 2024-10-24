#! /usr/bin/env python3

""" TvFunc のテストプログラム

:file: tvfunc_test.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2024 Yusuke Matsunaga, All rights reserved.
"""

import pytest
from ymlogic import TvFunc, Literal, NpnMap


def count(f):
    ni = f.input_num
    nexp = 1 << ni
    n0 = 0
    n1 = 0
    for p in range(nexp):
        if f.value(p):
            n1 += 1
        else:
            n0 += 1
    return n0, n1

def walsh_0(f):
    ni = f.input_num
    nexp = 1 << ni
    n = 0
    for p in range(nexp):
        if f.value(p):
            n -= 1
        else:
            n += 1
    return n

def walsh_1(f, var):
    f0 = f.cofactor(var, inv=True)
    f1 = f.cofactor(var, inv=False)
    return (walsh_0(f0) - walsh_0(f1)) // 2

def walsh_2(f, var1, var2):
    f0 = f.cofactor(var1, inv=True)
    f1 = f.cofactor(var1, inv=False)
    f00 = f0.cofactor(var2, inv=True)
    f01 = f0.cofactor(var2, inv=False)
    f10 = f1.cofactor(var2, inv=True)
    f11 = f1.cofactor(var2, inv=False)
    
    return (walsh_0(f00) - walsh_0(f01) - walsh_0(f10) + walsh_0(f11)) // 4

def check_sup(f, var):
    f0 = f.cofactor(var, inv=True)
    f1 = f.cofactor(var, inv=False)
    if f0 != f1:
        return True
    else:
        return False

def check_uname(f, var):
    f0 = f.cofactor(var, inv=True)
    f1 = f.cofactor(var, inv=False)
    return False

def test_empty_construct():

    f = TvFunc()

    assert not f.is_valid()
    assert f.is_invalid()

def test_construct1():

    ni = 5
    f = TvFunc(ni)

    assert f.is_valid()
    assert not f.is_invalid()
    assert f.input_num == ni

    nexp = 1 << ni
    for p in range(nexp):
        assert f.value(p) == 0

def test_construct2():
    ni = 2
    vect = [ 0, 1, 1, 0 ]
    f = TvFunc(ni, val_list=vect)

    assert f.is_valid()
    assert f.input_num == ni
    for i, val in enumerate(vect):
        assert f.value(i) == val
    assert str(f) == "0110"
    n0, n1 = count(f)
    assert f.count_zero() == n0
    assert f.count_one() == n1
    for i in range(f.input_num):
        n = walsh_1(f, i)
        assert f.walsh_1(i) == n
    for i1 in range(f.input_num):
        for i2 in range(f.input_num):
            if i1 == i2:
                continue
            n = walsh_2(f, i1, i2)
            assert f.walsh_2(i1, i2) == n
    for i in range(f.input_num):
        exp_val = check_sup(f, i)
        assert f.check_sup(i) == exp_val

def test_construct2_bad():
    ni = 2
    vect = [ 0, 1, 1 ]
    with pytest.raises(ValueError) as e:
        f = TvFunc(ni, val_list=vect)
    
def test_from_string():
    f_str = "0110"
    f = TvFunc.from_string(f_str)

    assert f.is_valid()
    assert f.input_num == 2

    assert str(f) == f_str
    n0, n1 = count(f)
    assert f.count_zero() == n0
    assert f.count_one() == n1
    for i in range(f.input_num):
        n = walsh_1(f, i)
        assert f.walsh_1(i) == n
    for i1 in range(f.input_num):
        for i2 in range(f.input_num):
            if i1 == i2:
                continue
            n = walsh_2(f, i1, i2)
            assert f.walsh_2(i1, i2) == n

def test_from_string_bad():
    f_str = "01101"
    with pytest.raises(ValueError) as e:
        f = TvFunc.from_string(f_str)
        
def test_invalid():
    f = TvFunc.invalid()

    assert f.is_invalid()

def test_zero():
    ni = 5
    f = TvFunc.zero(ni)

    assert f.is_valid()
    assert f.input_num == ni

    nexp = 1 << ni
    for p in range(nexp):
        assert f.value(p) == 0

    assert f.is_zero()
    n0, n1 = count(f)
    assert f.count_zero() == n0
    assert f.count_one() == n1
    assert f.count_zero() == nexp
    assert f.count_one() == 0
    for i in range(ni):
        n = walsh_1(f, i)
        assert f.walsh_1(i) == n
    for i1 in range(f.input_num):
        for i2 in range(f.input_num):
            if i1 == i2:
                continue
            n = walsh_2(f, i1, i2)
            assert f.walsh_2(i1, i2) == n
    
def test_one():
    ni = 5
    f = TvFunc.one(ni)

    assert f.is_valid()
    assert f.input_num == ni

    nexp = 1 << ni
    for p in range(nexp):
        assert f.value(p) == 1

    assert f.is_one()
    n0, n1 = count(f)
    assert f.count_zero() == n0
    assert f.count_one() == n1
    assert f.count_zero() == 0
    assert f.count_one() == nexp
    for i in range(ni):
        n = walsh_1(f, i)
        assert f.walsh_1(i) == n
    for i1 in range(f.input_num):
        for i2 in range(f.input_num):
            if i1 == i2:
                continue
            n = walsh_2(f, i1, i2)
            assert f.walsh_2(i1, i2) == n
    
def test_literal1():
    ni = 5
    pos = 3
    f = TvFunc.literal(ni, pos)

    assert f.is_valid()
    assert f.input_num == ni

    nexp = 1 << ni
    for p in range(nexp):
        if p & (1 << pos):
            exp_val = 1
        else:
            exp_val = 0
        assert f.value(p) == exp_val

    assert not f.is_zero()
    assert not f.is_one()
    n0, n1 = count(f)
    assert f.count_zero() == n0
    assert f.count_one() == n1
    for i in range(ni):
        n = walsh_1(f, i)
        assert f.walsh_1(i) == n
    for i1 in range(f.input_num):
        for i2 in range(f.input_num):
            if i1 == i2:
                continue
            n = walsh_2(f, i1, i2)
            assert f.walsh_2(i1, i2) == n

def test_literal1_bad():
    ni = 5
    pos = 5
    with pytest.raises(ValueError) as e:
        f = TvFunc.literal(ni, pos)
        
def test_literal2():
    ni = 5
    pos = 3
    lit = Literal(pos)
    f = TvFunc.literal(ni, lit)

    assert f.is_valid()
    assert f.input_num == ni

    nexp = 1 << ni
    for p in range(nexp):
        if p & (1 << pos):
            exp_val = 1
        else:
            exp_val = 0
        assert f.value(p) == exp_val
    n0, n1 = count(f)
    assert f.count_zero() == n0
    assert f.count_one() == n1
    for i in range(ni):
        n = walsh_1(f, i)
        assert f.walsh_1(i) == n
    for i1 in range(f.input_num):
        for i2 in range(f.input_num):
            if i1 == i2:
                continue
            n = walsh_2(f, i1, i2)
            assert f.walsh_2(i1, i2) == n

def test_literal2_bad():
    ni = 5
    pos = 5
    lit = Literal(pos)
    with pytest.raises(ValueError) as e:
        f = TvFunc.literal(ni, lit)
        
def test_posi_literal():
    ni = 5
    pos = 3
    f = TvFunc.posi_literal(ni, pos)

    assert f.is_valid()
    assert f.input_num == ni

    nexp = 1 << ni
    for p in range(nexp):
        if p & (1 << pos):
            exp_val = 1
        else:
            exp_val = 0
        assert f.value(p) == exp_val
    n0, n1 = count(f)
    assert f.count_zero() == n0
    assert f.count_one() == n1
    for i in range(ni):
        n = walsh_1(f, i)
        assert f.walsh_1(i) == n
    for i1 in range(f.input_num):
        for i2 in range(f.input_num):
            if i1 == i2:
                continue
            n = walsh_2(f, i1, i2)
            assert f.walsh_2(i1, i2) == n

def test_posi_literal_bad():
    ni = 5
    pos = 5
    with pytest.raises(ValueError) as e:
        f = TvFunc.posi_literal(ni, pos)
        
def test_nega_literal():
    ni = 5
    pos = 3
    f = TvFunc.nega_literal(ni, pos)

    assert f.is_valid()
    assert f.input_num == ni

    nexp = 1 << ni
    for p in range(nexp):
        if p & (1 << pos):
            exp_val = 0
        else:
            exp_val = 1
        assert f.value(p) == exp_val
    n0, n1 = count(f)
    assert f.count_zero() == n0
    assert f.count_one() == n1
    for i in range(ni):
        n = walsh_1(f, i)
        assert f.walsh_1(i) == n
    for i1 in range(f.input_num):
        for i2 in range(f.input_num):
            if i1 == i2:
                continue
            n = walsh_2(f, i1, i2)
            assert f.walsh_2(i1, i2) == n

def test_nega_literal_bad():
    ni = 5
    pos = 5
    with pytest.raises(ValueError) as e:
        f = TvFunc.nega_literal(ni, pos)

def test_invert():
    f = TvFunc.from_string("01101001")

    f1 = ~f

    assert str(f1) == "10010110"
    n0, n1 = count(f)
    assert f.count_zero() == n0
    assert f.count_one() == n1
    for i in range(f.input_num):
        n = walsh_1(f, i)
        assert f.walsh_1(i) == n
    for i1 in range(f.input_num):
        for i2 in range(f.input_num):
            if i1 == i2:
                continue
            n = walsh_2(f, i1, i2)
            assert f.walsh_2(i1, i2) == n

def test_and_op():
    f1 = TvFunc.from_string("0011")
    f2 = TvFunc.from_string("0101")

    f = f1 & f2

    assert str(f) == "0001"
    n0, n1 = count(f)
    assert f.count_zero() == n0
    assert f.count_one() == n1
    for i in range(f.input_num):
        n = walsh_1(f, i)
        assert f.walsh_1(i) == n
    for i1 in range(f.input_num):
        for i2 in range(f.input_num):
            if i1 == i2:
                continue
            n = walsh_2(f, i1, i2)
            assert f.walsh_2(i1, i2) == n

def test_and_op_bad():
    f1 = TvFunc.from_string("0011")
    f2 = TvFunc.from_string("01010011")

    with pytest.raises(ValueError) as e:
        f = f1 & f2

def test_or_op():
    f1 = TvFunc.from_string("0011")
    f2 = TvFunc.from_string("0101")

    f = f1 | f2

    assert str(f) == "0111"
    n0, n1 = count(f)
    assert f.count_zero() == n0
    assert f.count_one() == n1
    for i in range(f.input_num):
        n = walsh_1(f, i)
        assert f.walsh_1(i) == n
    for i1 in range(f.input_num):
        for i2 in range(f.input_num):
            if i1 == i2:
                continue
            n = walsh_2(f, i1, i2)
            assert f.walsh_2(i1, i2) == n

def test_or_op_bad():
    f1 = TvFunc.from_string("0011")
    f2 = TvFunc.from_string("01010011")

    with pytest.raises(ValueError) as e:
        f = f1 | f2

def test_xor_op():
    f1 = TvFunc.from_string("0011")
    f2 = TvFunc.from_string("0101")

    f = f1 ^ f2

    assert str(f) == "0110"
    n0, n1 = count(f)
    assert f.count_zero() == n0
    assert f.count_one() == n1
    for i in range(f.input_num):
        n = walsh_1(f, i)
        assert f.walsh_1(i) == n
    for i1 in range(f.input_num):
        for i2 in range(f.input_num):
            if i1 == i2:
                continue
            n = walsh_2(f, i1, i2)
            assert f.walsh_2(i1, i2) == n

def test_xor_op_bad():
    f1 = TvFunc.from_string("0011")
    f2 = TvFunc.from_string("01010011")

    with pytest.raises(ValueError) as e:
        f = f1 ^ f2

def test_and_int():
    f = TvFunc.from_string("0011")
    f2 = TvFunc.from_string("0101")

    f &= f2

    assert str(f) == "0001"
    n0, n1 = count(f)
    assert f.count_zero() == n0
    assert f.count_one() == n1
    for i in range(f.input_num):
        n = walsh_1(f, i)
        assert f.walsh_1(i) == n
    for i1 in range(f.input_num):
        for i2 in range(f.input_num):
            if i1 == i2:
                continue
            n = walsh_2(f, i1, i2)
            assert f.walsh_2(i1, i2) == n

def test_and_int_bad():
    f = TvFunc.from_string("0011")
    f2 = TvFunc.from_string("01010011")

    with pytest.raises(ValueError) as e:
        f &= f2

def test_or_int():
    f = TvFunc.from_string("0011")
    f2 = TvFunc.from_string("0101")

    f |= f2

    assert str(f) == "0111"
    n0, n1 = count(f)
    assert f.count_zero() == n0
    assert f.count_one() == n1
    for i in range(f.input_num):
        n = walsh_1(f, i)
        assert f.walsh_1(i) == n
    for i1 in range(f.input_num):
        for i2 in range(f.input_num):
            if i1 == i2:
                continue
            n = walsh_2(f, i1, i2)
            assert f.walsh_2(i1, i2) == n

def test_or_int_bad():
    f = TvFunc.from_string("0011")
    f2 = TvFunc.from_string("01010011")

    with pytest.raises(ValueError) as e:
        f |= f2

def test_xor_int():
    f = TvFunc.from_string("0011")
    f2 = TvFunc.from_string("0101")

    f ^= f2

    assert str(f) == "0110"
    n0, n1 = count(f)
    assert f.count_zero() == n0
    assert f.count_one() == n1
    for i in range(f.input_num):
        n = walsh_1(f, i)
        assert f.walsh_1(i) == n
    for i1 in range(f.input_num):
        for i2 in range(f.input_num):
            if i1 == i2:
                continue
            n = walsh_2(f, i1, i2)
            assert f.walsh_2(i1, i2) == n

def test_xor_int_bad():
    f = TvFunc.from_string("0011")
    f2 = TvFunc.from_string("01010011")

    with pytest.raises(ValueError) as e:
        f ^= f2

def test_cofactor1():
    f1 = TvFunc.from_string("00011111")

    f = f1.cofactor(0)

    assert str(f) == "00001111"
    n0, n1 = count(f)
    assert f.count_zero() == n0
    assert f.count_one() == n1
    for i in range(f.input_num):
        n = walsh_1(f, i)
        assert f.walsh_1(i) == n
    for i1 in range(f.input_num):
        for i2 in range(f.input_num):
            if i1 == i2:
                continue
            n = walsh_2(f, i1, i2)
            assert f.walsh_2(i1, i2) == n

def test_cofactor1_bad():
    f1 = TvFunc.from_string("00011111")

    with pytest.raises(ValueError) as e:
        f = f1.cofactor(3)

def test_cofactor2():
    f1 = TvFunc.from_string("00011111")
    lit = Literal(0, inv=True)
    f = f1.cofactor(lit)

    assert str(f) == "00111111"
    n0, n1 = count(f)
    assert f.count_zero() == n0
    assert f.count_one() == n1
    for i in range(f.input_num):
        n = walsh_1(f, i)
        assert f.walsh_1(i) == n
    for i1 in range(f.input_num):
        for i2 in range(f.input_num):
            if i1 == i2:
                continue
            n = walsh_2(f, i1, i2)
            assert f.walsh_2(i1, i2) == n

def test_cofactor2_bad():
    f1 = TvFunc.from_string("00011111")
    lit = Literal(3, inv=True)

    with pytest.raises(ValueError) as e:
        f = f1.cofactor(lit)

def test_cofactor_int1():
    f = TvFunc.from_string("00011111")

    f.cofactor_int(0)

    assert str(f) == "00001111"
    n0, n1 = count(f)
    assert f.count_zero() == n0
    assert f.count_one() == n1
    for i in range(f.input_num):
        n = walsh_1(f, i)
        assert f.walsh_1(i) == n
    for i1 in range(f.input_num):
        for i2 in range(f.input_num):
            if i1 == i2:
                continue
            n = walsh_2(f, i1, i2)
            assert f.walsh_2(i1, i2) == n

def test_cofactor_int1_bad():
    f = TvFunc.from_string("00011111")

    with pytest.raises(ValueError) as e:
        f.cofactor_int(3)

def test_cofactor_int2():
    f = TvFunc.from_string("00011111")
    lit = Literal(0, inv=True)
    f.cofactor_int(lit)

    assert str(f) == "00111111"
    n0, n1 = count(f)
    assert f.count_zero() == n0
    assert f.count_one() == n1
    for i in range(f.input_num):
        n = walsh_1(f, i)
        assert f.walsh_1(i) == n
    for i1 in range(f.input_num):
        for i2 in range(f.input_num):
            if i1 == i2:
                continue
            n = walsh_2(f, i1, i2)
            assert f.walsh_2(i1, i2) == n

def test_cofactor_int2_bad():
    f = TvFunc.from_string("00011111")
    lit = Literal(3, inv=True)

    with pytest.raises(ValueError) as e:
        f.cofactor_int(lit)
    
def test_bcf():
    f = TvFunc.from_string("0111")

    cov = f.bcf()

    assert str(cov.expr()) == "( ~0 | ~1 )"

def test_mwc():
    f = TvFunc.from_string("10010110")

    cov = f.mwc()

    assert str(cov.expr()) == "( ( ~0 & ~1 & 2 ) | ( ~0 & 1 & ~2 ) | ( 0 & ~1 & ~2 ) | ( 0 & 1 & 2 ) )"

def test_xform():
    f = TvFunc.from_string("10000000")

    xmap = NpnMap.identity(3)
    xmap.set(0, 0, True)

    f1 = f.xform(xmap)

    assert str(f1) == "01000000"

def test_shrink():
    f = TvFunc.from_string("10001000")

    xmap = f.shrink_map()
    f1 = f.xform(xmap)

    assert str(f1) == "1000"

def test_npn_cannonical_map():
    f = TvFunc.from_string("00100000")

    xmap = f.npn_cannonical_map()
    f1 = f.xform(xmap)

    assert str(f1) == "10000000"

def test_npn_cannonical_map2():
    f = TvFunc.from_string("11101111")

    xmap = f.npn_cannonical_map()
    f1 = f.xform(xmap)

    assert str(f1) == "10000000"

def test_npn_cannonical_all_map():
    f = TvFunc.from_string("11001010")

    xmap_list = f.npn_cannonical_all_map()

    assert len(xmap_list) == 4

    f0 = f.xform(xmap_list[0])

    for xmap in xmap_list:
        f1 = f.xform(xmap)
        assert f1 == f0
