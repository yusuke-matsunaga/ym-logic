#! /usr/bin/env python3

""" SopCube のテストプログラム

:file: sopcube_test.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2024 Yusuke Matsunaga, All rights reserved.
"""

import pytest
from ymlogic import SopCube, Literal


def test_constructor1():
    ni = 10
    cube = SopCube(ni)

    assert cube.variable_num == ni
    assert cube.literal_num == 0

    lit_list = cube.literal_list()
    assert len(lit_list) == 0

def test_constructor2():
    ni = 10
    lit = Literal(5, inv=True)
    lit_list = [lit]
    cube = SopCube(ni, literal_list=lit_list)

    assert cube.variable_num == ni
    assert cube.literal_num == 1

    lit_list = cube.literal_list()
    assert len(lit_list) == 1
    assert lit_list[0] == lit

    for i in range(ni):
        for inv in (True, False):
            exp_val = True if i == 5 and inv == True else False
            val = cube.check_literal(Literal(i, inv=inv))
            assert val == exp_val

def test_constructor3():
    ni = 10
    lit1 = Literal(5, inv=True)
    lit2 = Literal(0, inv=False)
    src_lit_list = [lit1, lit2]
    cube = SopCube(ni, literal_list=src_lit_list)

    assert cube.variable_num == ni
    assert cube.literal_num == len(src_lit_list)

    lit_list = cube.literal_list()
    assert lit_list[0] == lit2
    assert lit_list[1] == lit1

    for i in range(ni):
        for inv in (True, False):
            lit = Literal(i, inv=inv)
            exp_val = True if lit == lit1 or lit == lit2 else False
            val = cube.check_literal(Literal(i, inv=inv))
            assert val == exp_val
    
def test_copy():
    ni = 10
    lit1 = Literal(5, inv=True)
    lit2 = Literal(0, inv=False)
    src_lit_list = [lit1, lit2]
    cube = SopCube(ni, literal_list=src_lit_list)

    cube2 = cube.copy()

    assert cube2.variable_num == ni
    assert cube2.literal_num == len(src_lit_list)

    lit_list = cube2.literal_list()
    assert lit_list[0] == lit2
    assert lit_list[1] == lit1

    for i in range(ni):
        for inv in (True, False):
            lit = Literal(i, inv=inv)
            exp_val = True if lit == lit1 or lit == lit2 else False
            val = cube2.check_literal(Literal(i, inv=inv))
            assert val == exp_val

def test_check_containment():
    ni = 10
    lit1 = Literal(5, inv=True)
    lit2 = Literal(0, inv=False)
    src_lit_list = [lit1, lit2]

    cube1 = SopCube(ni, literal_list=src_lit_list)

    cube2 = SopCube(ni, literal_list=[lit1])

    assert cube1.check_containment(cube2)
    assert not cube2.check_containment(cube1)

    cube3 = SopCube(ni, literal_list=[lit2])

    assert not cube2.check_containment(cube3)
    assert not cube3.check_containment(cube2)

def test_check_containment_bad():
    ni = 10
    lit1 = Literal(5, inv=True)
    lit2 = Literal(0, inv=False)
    src_lit_list = [lit1, lit2]

    cube1 = SopCube(ni, literal_list=src_lit_list)

    cube2 = SopCube(ni + 1, literal_list=[lit1])

    with pytest.raises(ValueError) as e:
        cube1.check_containment(cube2)
    with pytest.raises(ValueError) as e:
        cube2.check_containment(cube1)
    
def test_cube_product1():
    ni = 10
    lit1 = Literal(0, inv=False)
    lit2 = Literal(5, inv=True)
    lit3 = Literal(7, inv=True)

    cube1 = SopCube(ni, literal_list=[lit2, lit3])
    cube2 = SopCube(ni, literal_list=[lit1, lit2])

    cube3 = cube1 & cube2

    assert cube3.is_valid()
    lit_list = cube3.literal_list()
    assert len(lit_list) == 3
    assert lit_list[0] == lit1
    assert lit_list[1] == lit2
    assert lit_list[2] == lit3
    
def test_cube_product2():
    ni = 10
    lit1 = Literal(0, inv=False)
    lit2 = Literal(5, inv=True)
    lit3 = Literal(7, inv=True)
    lit4 = Literal(5, inv=False)

    cube1 = SopCube(ni, literal_list=[lit2, lit3])
    cube2 = SopCube(ni, literal_list=[lit1, lit4])

    cube3 = cube1 & cube2

    assert cube3.is_invalid()
    
def test_cube_product_int1():
    ni = 10
    lit1 = Literal(0, inv=False)
    lit2 = Literal(5, inv=True)
    lit3 = Literal(7, inv=True)

    cube1 = SopCube(ni, literal_list=[lit2, lit3])
    cube2 = SopCube(ni, literal_list=[lit1, lit2])

    cube1 &= cube2

    assert cube1.is_valid()
    lit_list = cube1.literal_list()
    assert len(lit_list) == 3
    assert lit_list[0] == lit1
    assert lit_list[1] == lit2
    assert lit_list[2] == lit3
    
def test_cube_product_int2():
    ni = 10
    lit1 = Literal(0, inv=False)
    lit2 = Literal(5, inv=True)
    lit3 = Literal(7, inv=True)
    lit4 = Literal(5, inv=False)

    cube1 = SopCube(ni, literal_list=[lit2, lit3])
    cube2 = SopCube(ni, literal_list=[lit1, lit4])

    cube1 &= cube2

    assert cube1.is_invalid()

def test_lit_cube_product1():
    ni = 10
    lit1 = Literal(0, inv=False)
    lit2 = Literal(5, inv=True)
    lit3 = Literal(7, inv=True)

    cube1 = SopCube(ni, literal_list=[lit2, lit3])

    cube3 = lit1 & cube1

    assert cube3.is_valid()
    lit_list = cube3.literal_list()
    assert len(lit_list) == 3
    assert lit_list[0] == lit1
    assert lit_list[1] == lit2
    assert lit_list[2] == lit3

def test_lit_cube_product2():
    ni = 10
    lit1 = Literal(5, inv=False)
    lit2 = Literal(5, inv=True)
    lit3 = Literal(7, inv=True)

    cube1 = SopCube(ni, literal_list=[lit2, lit3])

    cube3 = lit1 & cube1

    assert cube3.is_invalid()

def test_cube_lit_product1():
    ni = 10
    lit1 = Literal(0, inv=False)
    lit2 = Literal(5, inv=True)
    lit3 = Literal(7, inv=True)

    cube1 = SopCube(ni, literal_list=[lit2, lit3])

    cube3 = cube1 & lit1

    assert cube3.is_valid()
    lit_list = cube3.literal_list()
    assert len(lit_list) == 3
    assert lit_list[0] == lit1
    assert lit_list[1] == lit2
    assert lit_list[2] == lit3

def test_cube_lit_product2():
    ni = 10
    lit1 = Literal(5, inv=False)
    lit2 = Literal(5, inv=True)
    lit3 = Literal(7, inv=True)

    cube1 = SopCube(ni, literal_list=[lit2, lit3])

    cube3 = cube1 & lit1

    assert cube3.is_invalid()

def test_lit_product_int1():
    ni = 10
    lit1 = Literal(0, inv=False)
    lit2 = Literal(5, inv=True)
    lit3 = Literal(7, inv=True)

    cube1 = SopCube(ni, literal_list=[lit2, lit3])

    cube1 &= lit1

    assert cube1.is_valid()
    lit_list = cube1.literal_list()
    assert len(lit_list) == 3
    assert lit_list[0] == lit1
    assert lit_list[1] == lit2
    assert lit_list[2] == lit3

def test_lit_product_int2():
    ni = 10
    lit1 = Literal(5, inv=False)
    lit2 = Literal(5, inv=True)
    lit3 = Literal(7, inv=True)

    cube1 = SopCube(ni, literal_list=[lit2, lit3])

    cube1 &= lit1

    assert cube1.is_invalid()

def test_cube_cube_quotient1():
    ni = 10
    lit1 = Literal(0, inv=False)
    lit2 = Literal(5, inv=True)
    lit3 = Literal(7, inv=True)

    cube1 = SopCube(ni, literal_list=[lit1, lit2, lit3])
    cube2 = SopCube(ni, literal_list=[lit1, lit3])

    cube3 = cube1 / cube2

    assert cube3.is_valid()
    lit_list = cube3.literal_list()
    assert len(lit_list) == 1
    assert lit_list[0] == lit2

def test_cube_cube_quotient2():
    ni = 10
    lit1 = Literal(0, inv=False)
    lit2 = Literal(5, inv=True)
    lit3 = Literal(7, inv=True)

    cube1 = SopCube(ni, literal_list=[lit1, lit2])
    cube2 = SopCube(ni, literal_list=[lit3])

    cube3 = cube1 / cube2

    assert cube3.is_invalid()

def test_cube_lit_quotient1():
    ni = 10
    lit1 = Literal(0, inv=False)
    lit2 = Literal(5, inv=True)
    lit3 = Literal(7, inv=True)

    cube1 = SopCube(ni, literal_list=[lit1, lit2, lit3])

    cube3 = cube1 / lit1

    assert cube3.is_valid()
    lit_list = cube3.literal_list()
    assert len(lit_list) == 2
    assert lit_list[0] == lit2
    assert lit_list[1] == lit3

def test_cube_lit_quotient2():
    ni = 10
    lit1 = Literal(0, inv=False)
    lit2 = Literal(5, inv=True)
    lit3 = Literal(7, inv=True)

    cube1 = SopCube(ni, literal_list=[lit2, lit3])

    cube3 = cube1 / lit1

    assert cube3.is_invalid()

def test_cube_quotient_int1():
    ni = 10
    lit1 = Literal(0, inv=False)
    lit2 = Literal(5, inv=True)
    lit3 = Literal(7, inv=True)

    cube1 = SopCube(ni, literal_list=[lit1, lit2, lit3])
    cube2 = SopCube(ni, literal_list=[lit1, lit3])

    cube1 /= cube2

    assert cube1.is_valid()
    lit_list = cube1.literal_list()
    assert len(lit_list) == 1
    assert lit_list[0] == lit2

def test_cube_quotient_int2():
    ni = 10
    lit1 = Literal(0, inv=False)
    lit2 = Literal(5, inv=True)
    lit3 = Literal(7, inv=True)

    cube1 = SopCube(ni, literal_list=[lit1, lit2])
    cube2 = SopCube(ni, literal_list=[lit3])

    cube1 /= cube2

    assert cube1.is_invalid()

def test_lit_quotient_int1():
    ni = 10
    lit1 = Literal(0, inv=False)
    lit2 = Literal(5, inv=True)
    lit3 = Literal(7, inv=True)

    cube1 = SopCube(ni, literal_list=[lit1, lit2, lit3])

    cube1 /= lit1

    assert cube1.is_valid()
    lit_list = cube1.literal_list()
    assert len(lit_list) == 2
    assert lit_list[0] == lit2
    assert lit_list[1] == lit3

def test_lit_quotient_int2():
    ni = 10
    lit1 = Literal(0, inv=False)
    lit2 = Literal(5, inv=True)
    lit3 = Literal(7, inv=True)

    cube1 = SopCube(ni, literal_list=[lit2, lit3])

    cube1 /= lit1

    assert cube1.is_invalid()

def test_compare1():
    ni = 100

    lit0 = Literal(0)
    lit1 = Literal(80)

    cube1 = SopCube(ni, literal_list=[ lit0,  lit1])
    cube2 = SopCube(ni, literal_list=[ lit0, ~lit1])
    cube3 = SopCube(ni, literal_list=[~lit0,  lit1])
    cube4 = SopCube(ni, literal_list=[ lit0,  lit1])

    assert     cube1 == cube4
    assert not cube1 != cube4
    
    assert not cube1 == cube2
    assert     cube1 != cube2
    assert not cube1 < cube2
    assert     cube1 > cube2
    assert not cube1 <= cube2
    assert     cube1 >= cube2

    assert not cube1 == cube3
    assert     cube1 != cube3
    assert not cube1 < cube3
    assert     cube1 > cube3
    assert not cube1 <= cube3
    assert     cube1 >= cube3

    assert not cube2 == cube1
    assert     cube2 != cube1
    assert     cube2 < cube1
    assert not cube2 > cube1
    assert     cube2 <= cube1
    assert not cube2 >= cube1

    assert not cube2 == cube3
    assert     cube2 != cube3
    assert not cube2 < cube3
    assert     cube2 > cube3
    assert not cube2 <= cube3
    assert     cube2 >= cube3
    
    assert not cube3 == cube1
    assert     cube3 != cube1
    assert     cube3 < cube1
    assert not cube3 > cube1
    assert     cube3 <= cube1
    assert not cube3 >= cube1

    assert not cube3 == cube2
    assert     cube3 != cube2
    assert     cube3 < cube2
    assert not cube3 > cube2
    assert     cube3 <= cube2
    assert not cube3 >= cube2

def test_compare_bad():
    ni = 100

    lit0 = Literal(0)
    lit1 = Literal(80)

    cube1 = SopCube(ni, literal_list=[ lit0,  lit1])
    cube2 = SopCube(ni + 1, literal_list=[ lit0, ~lit1])

    with pytest.raises(ValueError) as e:
        cube1 == cube2
    with pytest.raises(ValueError) as e:
        cube1 != cube2
    with pytest.raises(ValueError) as e:
        cube1 < cube2
    with pytest.raises(ValueError) as e:
        cube1 > cube2
    with pytest.raises(ValueError) as e:
        cube1 <= cube2
    with pytest.raises(ValueError) as e:
        cube1 >= cube2
        
def test_to_expr():
    ni = 3

    lit1 = Literal(0)
    lit2 = Literal(1)
    lit3 = Literal(2, inv=True)

    cube1 = SopCube(ni, literal_list=[lit1, lit2, lit3])

    expr = cube1.expr()
    expr_str = str(expr)

    assert expr_str == "( 0 & 1 & ~2 )"
