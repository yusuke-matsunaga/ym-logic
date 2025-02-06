#! /usr/bin/env python3

""" SopCover のテストプログラム

:file: sopcover_test.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2024 Yusuke Matsunaga, All rights reserved.
"""

import pytest
from ymlogic import SopCover, SopCube, Literal


def test_constructor1():
    ni = 10

    cover1 = SopCover(ni)

    assert cover1.variable_num == ni
    assert cover1.literal_num() == 0
    for i in range(ni):
        lit = Literal(i)
        assert cover1.literal_num(lit) == 0
        assert cover1.literal_num(~lit) == 0

def test_construct2():
    ni = 10

    lit0 = Literal(0)
    lit1 = Literal(1)
    lit2 = Literal(2)
    lit3 = Literal(3)
    
    cube1 = SopCube(ni, literal_list=[lit0, ~lit1])
    cube2 = SopCube(ni, literal_list=[lit2, ~lit3])

    cover1 = SopCover(ni, cube_list=[cube1, cube2])

    assert cover1.variable_num == ni
    assert cover1.cube_num == 2
    assert cover1.literal_num() == 4

    for i in range(ni):
        for inv in (True, False):
            lit = Literal(i, inv=inv)
            exp_num = 0
            if lit == lit0:
                exp_num = 1
            elif lit == ~lit1:
                exp_num = 1
            elif lit == lit2:
                exp_num = 1
            elif lit == ~lit3:
                exp_num = 1
            assert cover1.literal_num(lit) == exp_num

    cube_list = cover1.literal_list()
    assert len(cube_list) == 2
    assert len(cube_list[0]) == 2
    assert len(cube_list[1]) == 2
    assert lit0 == cube_list[0][0]
    assert ~lit1 == cube_list[0][1]
    assert lit2 == cube_list[1][0]
    assert ~lit3 == cube_list[1][1]

def test_construct2_bad():
    ni = 10

    lit0 = Literal(0)
    lit1 = Literal(1)
    lit2 = Literal(2)
    lit3 = Literal(3)
    
    cube1 = SopCube(ni, literal_list=[lit0, ~lit1])
    cube2 = SopCube(ni + 1, literal_list=[lit2, ~lit3])

    with pytest.raises(ValueError) as e:
        cover1 = SopCover(ni, cube_list=[cube1, cube2])

def test_construct3():
    ni = 10

    lit0 = Literal(0)
    lit1 = Literal(1)
    lit2 = Literal(2)
    lit3 = Literal(3)
    
    cover1 = SopCover(ni, cube_list=[[lit0, ~lit1], [lit2, ~lit3]])

    assert cover1.variable_num == ni
    assert cover1.cube_num == 2
    assert cover1.literal_num() == 4

    for i in range(ni):
        for inv in (True, False):
            lit = Literal(i, inv=inv)
            exp_num = 0
            if lit == lit0:
                exp_num = 1
            elif lit == ~lit1:
                exp_num = 1
            elif lit == lit2:
                exp_num = 1
            elif lit == ~lit3:
                exp_num = 1
            assert cover1.literal_num(lit) == exp_num

    cube_list = cover1.literal_list()
    assert len(cube_list) == 2
    assert len(cube_list[0]) == 2
    assert len(cube_list[1]) == 2
    assert lit0 == cube_list[0][0]
    assert ~lit1 == cube_list[0][1]
    assert lit2 == cube_list[1][0]
    assert ~lit3 == cube_list[1][1]

def test_copy():
    ni = 10

    lit0 = Literal(0)
    lit1 = Literal(1)
    lit2 = Literal(2)
    lit3 = Literal(3)
    
    cube1 = SopCube(ni, literal_list=[lit0, ~lit1])
    cube2 = SopCube(ni, literal_list=[lit2, ~lit3])

    cover1 = SopCover(ni, cube_list=[cube1, cube2])

    cover2 = cover1.copy()
    
    assert cover2.variable_num == ni
    assert cover2.cube_num == 2
    assert cover2.literal_num() == 4

    for i in range(ni):
        for inv in (True, False):
            lit = Literal(i, inv=inv)
            exp_num = 0
            if lit == lit0:
                exp_num = 1
            elif lit == ~lit1:
                exp_num = 1
            elif lit == lit2:
                exp_num = 1
            elif lit == ~lit3:
                exp_num = 1
            assert cover2.literal_num(lit) == exp_num

    cube_list = cover2.literal_list()
    assert len(cube_list) == 2
    assert len(cube_list[0]) == 2
    assert len(cube_list[1]) == 2
    assert lit0 == cube_list[0][0]
    assert ~lit1 == cube_list[0][1]
    assert lit2 == cube_list[1][0]
    assert ~lit3 == cube_list[1][1]

def test_cover_cover_sum():
    ni = 10

    lit0 = Literal(0)
    lit1 = Literal(1)
    lit2 = Literal(2)
    lit3 = Literal(3)
    
    cube1 = SopCube(ni, literal_list=[lit0, ~lit1])
    cube2 = SopCube(ni, literal_list=[lit2, ~lit3])

    cover1 = SopCover(ni, cube_list=[cube1])
    cover2 = SopCover(ni, cube_list=[cube1, cube2])

    cover3 = cover1 + cover2
    
    assert cover3.variable_num == ni
    assert cover3.cube_num == 2
    assert cover3.literal_num() == 4

    for i in range(ni):
        for inv in (True, False):
            lit = Literal(i, inv=inv)
            exp_num = 0
            if lit == lit0:
                exp_num = 1
            elif lit == ~lit1:
                exp_num = 1
            elif lit == lit2:
                exp_num = 1
            elif lit == ~lit3:
                exp_num = 1
            assert cover3.literal_num(lit) == exp_num

    cube_list = cover3.literal_list()
    assert len(cube_list) == 2
    assert len(cube_list[0]) == 2
    assert len(cube_list[1]) == 2
    assert lit0 == cube_list[0][0]
    assert ~lit1 == cube_list[0][1]
    assert lit2 == cube_list[1][0]
    assert ~lit3 == cube_list[1][1]

def test_cover_cover_sum_bad():
    ni = 10

    lit0 = Literal(0)
    lit1 = Literal(1)
    lit2 = Literal(2)
    lit3 = Literal(3)

    cover1 = SopCover(ni, cube_list=[[lit0, ~lit1]])
    cover2 = SopCover(ni + 1, cube_list=[[lit2, ~lit3]])

    with pytest.raises(ValueError) as e:
        cover3 = cover1 + cover2

def test_cover_cube_sum():
    ni = 10

    lit0 = Literal(0)
    lit1 = Literal(1)
    lit2 = Literal(2)
    lit3 = Literal(3)
    
    cube1 = SopCube(ni, literal_list=[lit0, ~lit1])
    cube2 = SopCube(ni, literal_list=[lit2, ~lit3])

    cover1 = SopCover(ni, cube_list=[cube1])

    cover3 = cover1 + cube2
    
    assert cover3.variable_num == ni
    assert cover3.cube_num == 2
    assert cover3.literal_num() == 4

    for i in range(ni):
        for inv in (True, False):
            lit = Literal(i, inv=inv)
            exp_num = 0
            if lit == lit0:
                exp_num = 1
            elif lit == ~lit1:
                exp_num = 1
            elif lit == lit2:
                exp_num = 1
            elif lit == ~lit3:
                exp_num = 1
            assert cover3.literal_num(lit) == exp_num

    cube_list = cover3.literal_list()
    assert len(cube_list) == 2
    assert len(cube_list[0]) == 2
    assert len(cube_list[1]) == 2
    assert lit0 == cube_list[0][0]
    assert ~lit1 == cube_list[0][1]
    assert lit2 == cube_list[1][0]
    assert ~lit3 == cube_list[1][1]

def test_cover_cube_sum_bad():
    ni = 10

    lit0 = Literal(0)
    lit1 = Literal(1)
    lit2 = Literal(2)
    lit3 = Literal(3)
    
    cube1 = SopCube(ni, literal_list=[lit0, ~lit1])
    cube2 = SopCube(ni + 1, literal_list=[lit2, ~lit3])

    cover1 = SopCover(ni, cube_list=[cube1])

    with pytest.raises(ValueError) as e:
        cover3 = cover1 + cube2

def test_cube_cover_sum():
    ni = 10

    lit0 = Literal(0)
    lit1 = Literal(1)
    lit2 = Literal(2)
    lit3 = Literal(3)
    
    cube1 = SopCube(ni, literal_list=[lit0, ~lit1])
    cube2 = SopCube(ni, literal_list=[lit2, ~lit3])

    cover2 = SopCover(ni, cube_list=[cube2])

    cover3 = cube1 + cover2
    
    assert cover3.variable_num == ni
    assert cover3.cube_num == 2
    assert cover3.literal_num() == 4

    for i in range(ni):
        for inv in (True, False):
            lit = Literal(i, inv=inv)
            exp_num = 0
            if lit == lit0:
                exp_num = 1
            elif lit == ~lit1:
                exp_num = 1
            elif lit == lit2:
                exp_num = 1
            elif lit == ~lit3:
                exp_num = 1
            assert cover3.literal_num(lit) == exp_num

    cube_list = cover3.literal_list()
    assert len(cube_list) == 2
    assert len(cube_list[0]) == 2
    assert len(cube_list[1]) == 2
    assert lit0 == cube_list[0][0]
    assert ~lit1 == cube_list[0][1]
    assert lit2 == cube_list[1][0]
    assert ~lit3 == cube_list[1][1]

def test_cube_cover_sum_bad():
    ni = 10

    lit0 = Literal(0)
    lit1 = Literal(1)
    lit2 = Literal(2)
    lit3 = Literal(3)
    
    cube1 = SopCube(ni + 1, literal_list=[lit0, ~lit1])
    cube2 = SopCube(ni, literal_list=[lit2, ~lit3])

    cover2 = SopCover(ni, cube_list=[cube2])

    with pytest.raises(ValueError) as e:
        cover3 = cube1 + cover2

def test_cover_cover_sum_int():
    ni = 10

    lit0 = Literal(0)
    lit1 = Literal(1)
    lit2 = Literal(2)
    lit3 = Literal(3)
    
    cube1 = SopCube(ni, literal_list=[lit0, ~lit1])
    cube2 = SopCube(ni, literal_list=[lit2, ~lit3])

    cover1 = SopCover(ni, cube_list=[cube1])
    cover2 = SopCover(ni, cube_list=[cube1, cube2])

    cover1 += cover2
    
    assert cover1.variable_num == ni
    assert cover1.cube_num == 2
    assert cover1.literal_num() == 4

    for i in range(ni):
        for inv in (True, False):
            lit = Literal(i, inv=inv)
            exp_num = 0
            if lit == lit0:
                exp_num = 1
            elif lit == ~lit1:
                exp_num = 1
            elif lit == lit2:
                exp_num = 1
            elif lit == ~lit3:
                exp_num = 1
            assert cover1.literal_num(lit) == exp_num

    cube_list = cover1.literal_list()
    assert len(cube_list) == 2
    assert len(cube_list[0]) == 2
    assert len(cube_list[1]) == 2
    assert lit0 == cube_list[0][0]
    assert ~lit1 == cube_list[0][1]
    assert lit2 == cube_list[1][0]
    assert ~lit3 == cube_list[1][1]

def test_cover_cover_sum_int_bad():
    ni = 10

    lit0 = Literal(0)
    lit1 = Literal(1)
    lit2 = Literal(2)
    lit3 = Literal(3)

    cover1 = SopCover(ni, cube_list=[[lit0, ~lit1]])
    cover2 = SopCover(ni + 1, cube_list=[[lit2, ~lit3]])

    with pytest.raises(ValueError) as e:
        cover1 += cover2

def test_cover_cube_sum_int():
    ni = 10

    lit0 = Literal(0)
    lit1 = Literal(1)
    lit2 = Literal(2)
    lit3 = Literal(3)
    
    cube1 = SopCube(ni, literal_list=[lit0, ~lit1])
    cube2 = SopCube(ni, literal_list=[lit2, ~lit3])

    cover1 = SopCover(ni, cube_list=[cube1])

    cover1 += cube2
    
    assert cover1.variable_num == ni
    assert cover1.cube_num == 2
    assert cover1.literal_num() == 4

    for i in range(ni):
        for inv in (True, False):
            lit = Literal(i, inv=inv)
            exp_num = 0
            if lit == lit0:
                exp_num = 1
            elif lit == ~lit1:
                exp_num = 1
            elif lit == lit2:
                exp_num = 1
            elif lit == ~lit3:
                exp_num = 1
            assert cover1.literal_num(lit) == exp_num

    cube_list = cover1.literal_list()
    assert len(cube_list) == 2
    assert len(cube_list[0]) == 2
    assert len(cube_list[1]) == 2
    assert lit0 == cube_list[0][0]
    assert ~lit1 == cube_list[0][1]
    assert lit2 == cube_list[1][0]
    assert ~lit3 == cube_list[1][1]

def test_cover_cube_sum_int_bad():
    ni = 10

    lit0 = Literal(0)
    lit1 = Literal(1)
    lit2 = Literal(2)
    lit3 = Literal(3)
    
    cube1 = SopCube(ni, literal_list=[lit0, ~lit1])
    cube2 = SopCube(ni + 1, literal_list=[lit2, ~lit3])

    cover1 = SopCover(ni, cube_list=[cube1])

    with pytest.raises(ValueError) as e:
        cover1 += cube2

def test_to_expr():
    ni = 10

    lit0 = Literal(0)
    lit1 = Literal(1)
    lit2 = Literal(2)

    cover1 = SopCover(ni, cube_list=[[lit0, lit1], [~lit2]])

    expr = cover1.expr()
    expr_str = str(expr)
    assert expr_str == "( ( 0 & 1 ) | ~2 )"
    
