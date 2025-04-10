#! /usr/bin/env python3

""" BddMgr のテストプログラム

:file: bddmgr_test.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2024 Yusuke Matsunaga, All rights reserved.
"""

import pytest
from ymlogic import Bdd, BddMgr


def check(bdd, mgr, exp_str):
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
    var_list = [ mgr.variable(i) for i in range(ni) ]
    truth_str = bdd.to_truth(var_list)
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

    assert check(bdd, mgr, "0")

def test_one():
    mgr = BddMgr()

    bdd = mgr.one()
    assert not bdd.is_invalid()
    assert bdd.is_valid()
    assert not bdd.is_zero()
    assert bdd.is_one()

    assert check(bdd, mgr, "1")

def test_variable1():
    mgr = BddMgr()

    var = mgr.variable(0)
    bdd = var
    assert not bdd.is_invalid()
    assert bdd.is_valid()
    assert not bdd.is_zero()
    assert not bdd.is_one()

    var1, f0, f1 = bdd.root_decomp()

    assert var1 == var
    assert f0.is_zero()
    assert f1.is_one()
    
    assert check(bdd, mgr, "10")
    
def test_variable2():
    mgr = BddMgr()

    var = mgr.variable(1)
    bdd = ~var
    assert not bdd.is_invalid()
    assert bdd.is_valid()
    assert not bdd.is_zero()
    assert not bdd.is_one()

    var1, f0, f1 = bdd.root_decomp()

    assert var1 == var
    assert f0.is_one()
    assert f1.is_zero()

    assert check(bdd, mgr, "0101")

def test_variable3():
    mgr = BddMgr()

    var = mgr.variable(2)
    bdd = var
    assert not bdd.is_invalid()
    assert bdd.is_valid()
    assert not bdd.is_zero()
    assert not bdd.is_one()

    var1, f0, f1 = bdd.root_decomp()

    assert var1 == var
    assert f0.is_zero()
    assert f1.is_one()

    assert check(bdd, mgr, "10101010")

def test_variable4():
    mgr = BddMgr()

    var = mgr.variable(3)
    bdd = ~var
    assert not bdd.is_invalid()
    assert bdd.is_valid()
    assert not bdd.is_zero()
    assert not bdd.is_one()

    var1, f0, f1 = bdd.root_decomp()

    assert var1 == var
    assert f0.is_one()
    assert f1.is_zero()

    assert check(bdd, mgr, "0101010101010101")

def test_from_truth1():
    mgr = BddMgr()

    var_list = [ mgr.variable(i) for i in range(3) ]
    exp_str = "10010110"
    bdd = mgr.from_truth(exp_str,
                         var_list=var_list)

    assert not bdd.is_invalid()
    assert bdd.is_valid()

    assert check(bdd, mgr, exp_str)
