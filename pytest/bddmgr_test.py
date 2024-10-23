#! /usr/bin/env python3

""" BddMgr のテストプログラム

:file: bddmgr_test.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2024 Yusuke Matsunaga, All rights reserved.
"""

import pytest
from ymlogic import Bdd, BddMgr


def check(bdd, exp_str):
    n = len(exp_str)
    ni = 0
    while ( (1 << ni) < n ):
        ni += 1
    assert n == (1 << ni)
    inputs = [ False for _ in range(ni) ]
    for p in range(n):
        for i in range(ni):
            if p & (1 << (ni - i - 1)) != 0:
                inputs[i] = True
            else:
                inputs[i] = False
        val = bdd.eval(inputs)
        exp_val = True if exp_str[n - p - 1] == '1' else False
        if exp_val != val:
            print(f"val = {val}")
            print(f"exp_val = {exp_val}")
            return False
    truth_str = bdd.to_truth(ni)
    if truth_str != exp_str:
        print(f'truth_str = {truth_str}')
        return False
    return True
    
def test_zero():
    mgr = BddMgr()

    bdd = mgr.zero()
    assert not bdd.is_invalid()
    assert bdd.is_valid()
    assert bdd.is_zero()
    assert not bdd.is_one()

    assert check(bdd, "0")

def test_one():
    mgr = BddMgr()

    bdd = mgr.one()
    assert not bdd.is_invalid()
    assert bdd.is_valid()
    assert not bdd.is_zero()
    assert bdd.is_one()

    assert check(bdd, "1")

def test_literal1():
    mgr = BddMgr()

    var = 0
    bdd = mgr.literal(var)
    assert not bdd.is_invalid()
    assert bdd.is_valid()
    assert not bdd.is_zero()
    assert not bdd.is_one()

    var1, f0, f1 = bdd.root_decomp()

    assert var1 == var
    assert f0.is_zero()
    assert f1.is_one()
    
    assert check(bdd, "10")
    
def test_literal2():
    mgr = BddMgr()

    var = 1
    bdd = mgr.literal(var, inv=False)
    assert not bdd.is_invalid()
    assert bdd.is_valid()
    assert not bdd.is_zero()
    assert not bdd.is_one()

    var1, f0, f1 = bdd.root_decomp()

    assert var1 == var
    assert f0.is_zero()
    assert f1.is_one()

    assert check(bdd, "1010")
    
def test_literal3():
    mgr = BddMgr()

    var = 1
    bdd = mgr.literal(var, inv=True)
    assert not bdd.is_invalid()
    assert bdd.is_valid()
    assert not bdd.is_zero()
    assert not bdd.is_one()

    var1, f0, f1 = bdd.root_decomp()

    assert var1 == var
    assert f0.is_one()
    assert f1.is_zero()

    assert check(bdd, "0101")

def test_posi_literal():
    mgr = BddMgr()

    var = 2
    bdd = mgr.posi_literal(var)
    assert not bdd.is_invalid()
    assert bdd.is_valid()
    assert not bdd.is_zero()
    assert not bdd.is_one()

    var1, f0, f1 = bdd.root_decomp()

    assert var1 == var
    assert f0.is_zero()
    assert f1.is_one()

    assert check(bdd, "10101010")

def test_nega_literal():
    mgr = BddMgr()

    var = 3
    bdd = mgr.nega_literal(var)
    assert not bdd.is_invalid()
    assert bdd.is_valid()
    assert not bdd.is_zero()
    assert not bdd.is_one()

    var1, f0, f1 = bdd.root_decomp()

    assert var1 == var
    assert f0.is_one()
    assert f1.is_zero()

    assert check(bdd, "0101010101010101")

def test_from_truth1():
    mgr = BddMgr()

    exp_str = "10010110"
    bdd = mgr.from_truth(exp_str)

    assert not bdd.is_invalid()
    assert bdd.is_valid()

    assert check(bdd, exp_str)
