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

def test_cover_cover_diff():
    ni = 10

    lit0 = Literal(0)
    lit1 = Literal(1)
    lit2 = Literal(2)
    lit3 = Literal(3)

    src1 = SopCover(ni, cube_list=[[lit0, lit1], [lit2, lit3]])
    src2 = SopCover(ni, cube_list=[[lit3, lit2], [lit0, ~lit1]])

    cover1 = src1 - src2
    
    assert cover1.variable_num == ni
    assert cover1.cube_num == 1
    assert cover1.literal_num() == 2

    for i in range(ni):
        for inv in (True, False):
            lit = Literal(i, inv=inv)
            exp_num = 0
            if lit == lit0:
                exp_num = 1
            elif lit == lit1:
                exp_num = 1
            assert cover1.literal_num(lit) == exp_num

    cube_list = cover1.literal_list()
    assert len(cube_list) == 1
    assert len(cube_list[0]) == 2
    assert lit0 == cube_list[0][0]
    assert lit1 == cube_list[0][1]

def test_cover_cover_diff_bad():
    ni = 10

    lit0 = Literal(0)
    lit1 = Literal(1)
    lit2 = Literal(2)
    lit3 = Literal(3)

    src1 = SopCover(ni, cube_list=[[lit0, lit1], [lit2, lit3]])
    src2 = SopCover(ni + 1, cube_list=[[lit3, lit2], [lit0, ~lit1]])

    with pytest.raises(ValueError) as e:
        cover1 = src1 - src2

def test_cover_cube_diff():
    ni = 10

    lit0 = Literal(0)
    lit1 = Literal(1)
    lit2 = Literal(2)
    lit3 = Literal(3)

    src1 = SopCover(ni, cube_list=[[lit0, lit1], [lit2, lit3]])
    src2 = SopCube(ni, literal_list=[lit3, lit2])

    cover1 = src1 - src2
    
    assert cover1.variable_num == ni
    assert cover1.cube_num == 1
    assert cover1.literal_num() == 2

    for i in range(ni):
        for inv in (True, False):
            lit = Literal(i, inv=inv)
            exp_num = 0
            if lit == lit0:
                exp_num = 1
            elif lit == lit1:
                exp_num = 1
            assert cover1.literal_num(lit) == exp_num

    cube_list = cover1.literal_list()
    assert len(cube_list) == 1
    assert len(cube_list[0]) == 2
    assert lit0 == cube_list[0][0]
    assert lit1 == cube_list[0][1]

def test_cover_cube_diff_bad():
    ni = 10

    lit0 = Literal(0)
    lit1 = Literal(1)
    lit2 = Literal(2)
    lit3 = Literal(3)

    src1 = SopCover(ni, cube_list=[[lit0, lit1], [lit2, lit3]])
    src2 = SopCube(ni + 1, literal_list=[lit3, lit2])

    with pytest.raises(ValueError) as e:
        cover1 = src1 - src2

def test_cover_cover_diff_int():
    ni = 10

    lit0 = Literal(0)
    lit1 = Literal(1)
    lit2 = Literal(2)
    lit3 = Literal(3)

    cover1 = SopCover(ni, cube_list=[[lit0, lit1], [lit2, lit3]])
    src2 = SopCover(ni, cube_list=[[lit3, lit2], [lit0, ~lit1]])

    cover1 -= src2
    
    assert cover1.variable_num == ni
    assert cover1.cube_num == 1
    assert cover1.literal_num() == 2

    for i in range(ni):
        for inv in (True, False):
            lit = Literal(i, inv=inv)
            exp_num = 0
            if lit == lit0:
                exp_num = 1
            elif lit == lit1:
                exp_num = 1
            assert cover1.literal_num(lit) == exp_num

    cube_list = cover1.literal_list()
    assert len(cube_list) == 1
    assert len(cube_list[0]) == 2
    assert lit0 == cube_list[0][0]
    assert lit1 == cube_list[0][1]

def test_cover_cover_diff_int_bad():
    ni = 10

    lit0 = Literal(0)
    lit1 = Literal(1)
    lit2 = Literal(2)
    lit3 = Literal(3)

    cover1 = SopCover(ni, cube_list=[[lit0, lit1], [lit2, lit3]])
    src2 = SopCover(ni + 1, cube_list=[[lit3, lit2], [lit0, ~lit1]])

    with pytest.raises(ValueError) as e:
        cover1 -= src2

def test_cover_cube_diff_int():
    ni = 10

    lit0 = Literal(0)
    lit1 = Literal(1)
    lit2 = Literal(2)
    lit3 = Literal(3)

    cover1 = SopCover(ni, cube_list=[[lit0, lit1], [lit2, lit3]])
    src2 = SopCube(ni, literal_list=[lit3, lit2])

    cover1 -= src2
    
    assert cover1.variable_num == ni
    assert cover1.cube_num == 1
    assert cover1.literal_num() == 2

    for i in range(ni):
        for inv in (True, False):
            lit = Literal(i, inv=inv)
            exp_num = 0
            if lit == lit0:
                exp_num = 1
            elif lit == lit1:
                exp_num = 1
            assert cover1.literal_num(lit) == exp_num

    cube_list = cover1.literal_list()
    assert len(cube_list) == 1
    assert len(cube_list[0]) == 2
    assert lit0 == cube_list[0][0]
    assert lit1 == cube_list[0][1]

def test_cover_cube_diff_int_bad():
    ni = 10

    lit0 = Literal(0)
    lit1 = Literal(1)
    lit2 = Literal(2)
    lit3 = Literal(3)

    cover1 = SopCover(ni, cube_list=[[lit0, lit1], [lit2, lit3]])
    src2 = SopCube(ni + 1, literal_list=[lit3, lit2])

    with pytest.raises(ValueError) as e:
        cover1 -= src2

def test_cover_cover_product():
    ni = 10

    lit0 = Literal(0)
    lit1 = Literal(1)
    lit2 = Literal(2)
    lit3 = Literal(3)

    src1 = SopCover(ni, cube_list=[[lit0], [lit1]])
    src2 = SopCover(ni, cube_list=[[lit2], [lit3]])

    cover1 = src1 * src2

    assert cover1.variable_num == ni
    assert cover1.cube_num == 4
    assert cover1.literal_num() == 8

    cube_list = cover1.literal_list()
    assert len(cube_list) == 4
    assert len(cube_list[0]) == 2
    assert lit0 == cube_list[0][0]
    assert lit2 == cube_list[0][1]
    assert len(cube_list[1]) == 2
    assert lit0 == cube_list[1][0]
    assert lit3 == cube_list[1][1]
    assert len(cube_list[2]) == 2
    assert lit1 == cube_list[2][0]
    assert lit2 == cube_list[2][1]
    assert len(cube_list[3]) == 2
    assert lit1 == cube_list[3][0]
    assert lit3 == cube_list[3][1]

def test_cover_cover_product_bad():
    ni = 10

    lit0 = Literal(0)
    lit1 = Literal(1)
    lit2 = Literal(2)
    lit3 = Literal(3)

    src1 = SopCover(ni, cube_list=[[lit0], [lit1]])
    src2 = SopCover(ni + 1, cube_list=[[lit2], [lit3]])

    with pytest.raises(ValueError) as e:
        cover1 = src1 * src2

def test_cover_cube_product():
    ni = 10

    lit0 = Literal(0)
    lit1 = Literal(1)
    lit2 = Literal(2)

    src1 = SopCover(ni, cube_list=[[lit0], [lit1]])
    src2 = SopCube(ni, literal_list=[lit2])

    cover1 = src1 * src2

    assert cover1.variable_num == ni
    assert cover1.cube_num == 2
    assert cover1.literal_num() == 4

    cube_list = cover1.literal_list()
    assert len(cube_list) == 2
    assert len(cube_list[0]) == 2
    assert lit0 == cube_list[0][0]
    assert lit2 == cube_list[0][1]
    assert len(cube_list[1]) == 2
    assert lit1 == cube_list[1][0]
    assert lit2 == cube_list[1][1]

def test_cover_cube_product_bad():
    ni = 10

    lit0 = Literal(0)
    lit1 = Literal(1)
    lit2 = Literal(2)

    src1 = SopCover(ni, cube_list=[[lit0], [lit1]])
    src2 = SopCube(ni + 1, literal_list=[lit2])

    with pytest.raises(ValueError) as e:
        cover1 = src1 * src2

def test_cover_literal_product():
    ni = 10

    lit0 = Literal(0)
    lit1 = Literal(1)
    lit2 = Literal(2)

    src1 = SopCover(ni, cube_list=[[lit0], [lit1]])
    src2 = lit2

    cover1 = src1 * src2

    assert cover1.variable_num == ni
    assert cover1.cube_num == 2
    assert cover1.literal_num() == 4

    cube_list = cover1.literal_list()
    assert len(cube_list) == 2
    assert len(cube_list[0]) == 2
    assert lit0 == cube_list[0][0]
    assert lit2 == cube_list[0][1]
    assert len(cube_list[1]) == 2
    assert lit1 == cube_list[1][0]
    assert lit2 == cube_list[1][1]
                                
def test_cube_cover_product():
    ni = 10

    lit0 = Literal(0)
    lit1 = Literal(1)
    lit2 = Literal(2)

    src1 = SopCover(ni, cube_list=[[lit0], [lit1]])
    src2 = SopCube(ni, literal_list=[lit2])

    cover1 = src2 * src1

    assert cover1.variable_num == ni
    assert cover1.cube_num == 2
    assert cover1.literal_num() == 4

    cube_list = cover1.literal_list()
    assert len(cube_list) == 2
    assert len(cube_list[0]) == 2
    assert lit0 == cube_list[0][0]
    assert lit2 == cube_list[0][1]
    assert len(cube_list[1]) == 2
    assert lit1 == cube_list[1][0]
    assert lit2 == cube_list[1][1]
                                
def test_cube_cover_product_bad():
    ni = 10

    lit0 = Literal(0)
    lit1 = Literal(1)
    lit2 = Literal(2)

    src1 = SopCover(ni, cube_list=[[lit0], [lit1]])
    src2 = SopCube(ni + 1, literal_list=[lit2])

    with pytest.raises(ValueError) as e:
        cover1 = src2 * src1

def test_literal_cover_product():
    ni = 10

    lit0 = Literal(0)
    lit1 = Literal(1)
    lit2 = Literal(2)

    src1 = SopCover(ni, cube_list=[[lit0], [lit1]])
    src2 = lit2

    cover1 = src2 * src1

    assert cover1.variable_num == ni
    assert cover1.cube_num == 2
    assert cover1.literal_num() == 4

    cube_list = cover1.literal_list()
    assert len(cube_list) == 2
    assert len(cube_list[0]) == 2
    assert lit0 == cube_list[0][0]
    assert lit2 == cube_list[0][1]
    assert len(cube_list[1]) == 2
    assert lit1 == cube_list[1][0]
    assert lit2 == cube_list[1][1]
                                
def test_cover_cover_product_int():
    ni = 10

    lit0 = Literal(0)
    lit1 = Literal(1)
    lit2 = Literal(2)
    lit3 = Literal(3)

    cover1 = SopCover(ni, cube_list=[[lit0], [lit1]])
    src2 = SopCover(ni, cube_list=[[lit2], [lit3]])

    cover1 *= src2

    assert cover1.variable_num == ni
    assert cover1.cube_num == 4
    assert cover1.literal_num() == 8

    cube_list = cover1.literal_list()
    assert len(cube_list) == 4
    assert len(cube_list[0]) == 2
    assert lit0 == cube_list[0][0]
    assert lit2 == cube_list[0][1]
    assert len(cube_list[1]) == 2
    assert lit0 == cube_list[1][0]
    assert lit3 == cube_list[1][1]
    assert len(cube_list[2]) == 2
    assert lit1 == cube_list[2][0]
    assert lit2 == cube_list[2][1]
    assert len(cube_list[3]) == 2
    assert lit1 == cube_list[3][0]
    assert lit3 == cube_list[3][1]
                                
def test_cover_cover_product_int_bad():
    ni = 10

    lit0 = Literal(0)
    lit1 = Literal(1)
    lit2 = Literal(2)
    lit3 = Literal(3)

    cover1 = SopCover(ni, cube_list=[[lit0], [lit1]])
    src2 = SopCover(ni + 1, cube_list=[[lit2], [lit3]])

    with pytest.raises(ValueError) as e:
        cover1 *= src2

def test_cover_cube_product_int():
    ni = 10

    lit0 = Literal(0)
    lit1 = Literal(1)
    lit2 = Literal(2)

    cover1 = SopCover(ni, cube_list=[[lit0], [lit1]])
    src2 = SopCube(ni, literal_list=[lit2])

    cover1 *= src2

    assert cover1.variable_num == ni
    assert cover1.cube_num == 2
    assert cover1.literal_num() == 4

    cube_list = cover1.literal_list()
    assert len(cube_list) == 2
    assert len(cube_list[0]) == 2
    assert lit0 == cube_list[0][0]
    assert lit2 == cube_list[0][1]
    assert len(cube_list[1]) == 2
    assert lit1 == cube_list[1][0]
    assert lit2 == cube_list[1][1]

def test_cover_cube_product_int_bad():
    ni = 10

    lit0 = Literal(0)
    lit1 = Literal(1)
    lit2 = Literal(2)

    cover1 = SopCover(ni, cube_list=[[lit0], [lit1]])
    src2 = SopCube(ni + 1, literal_list=[lit2])

    with pytest.raises(ValueError) as e:
        cover1 *= src2

def test_cover_literal_product_int():
    ni = 10

    lit0 = Literal(0)
    lit1 = Literal(1)
    lit2 = Literal(2)

    cover1 = SopCover(ni, cube_list=[[lit0], [lit1]])
    src2 = lit2

    cover1 *= src2

    assert cover1.variable_num == ni
    assert cover1.cube_num == 2
    assert cover1.literal_num() == 4

    cube_list = cover1.literal_list()
    assert len(cube_list) == 2
    assert len(cube_list[0]) == 2
    assert lit0 == cube_list[0][0]
    assert lit2 == cube_list[0][1]
    assert len(cube_list[1]) == 2
    assert lit1 == cube_list[1][0]
    assert lit2 == cube_list[1][1]

def test_cover_cover_quotient():
    ni = 10

    lit0 = Literal(0)
    lit1 = Literal(1)
    lit2 = Literal(2)
    lit3 = Literal(3)

    src1 = SopCover(ni, cube_list=[[lit0, lit2],
                                   [lit0, lit3],
                                   [lit1, lit2],
                                   [lit1, lit3]])
    src2 = SopCover(ni, cube_list=[[lit0],
                                   [lit1]])

    cover1 = src1 / src2

    assert cover1.variable_num == ni
    assert cover1.cube_num == 2
    assert cover1.literal_num() == 2

    cube_list = cover1.literal_list()
    assert len(cube_list) == 2
    assert len(cube_list[0]) == 1
    assert lit2 == cube_list[0][0]
    assert len(cube_list[1]) == 1
    assert lit3 == cube_list[1][0]

def test_cover_cover_quotient_bad():
    ni = 10

    lit0 = Literal(0)
    lit1 = Literal(1)
    lit2 = Literal(2)
    lit3 = Literal(3)

    src1 = SopCover(ni, cube_list=[[lit0, lit2],
                                   [lit0, lit3],
                                   [lit1, lit2],
                                   [lit1, lit3]])
    src2 = SopCover(ni + 1, cube_list=[[lit0],
                                   [lit1]])

    with pytest.raises(ValueError) as e:
        cover1 = src1 / src2

def test_cover_cover_quotient_int():
    ni = 10

    lit0 = Literal(0)
    lit1 = Literal(1)
    lit2 = Literal(2)
    lit3 = Literal(3)

    cover1 = SopCover(ni, cube_list=[[lit0, lit2],
                                     [lit0, lit3],
                                     [lit1, lit2],
                                     [lit1, lit3]])
    src2 = SopCover(ni, cube_list=[[lit0],
                                   [lit1]])
    
    cover1 /= src2

    assert cover1.variable_num == ni
    assert cover1.cube_num == 2
    assert cover1.literal_num() == 2

    cube_list = cover1.literal_list()
    assert len(cube_list) == 2
    assert len(cube_list[0]) == 1
    assert lit2 == cube_list[0][0]
    assert len(cube_list[1]) == 1
    assert lit3 == cube_list[1][0]

def test_cover_cover_quotient_int_bad():
    ni = 10

    lit0 = Literal(0)
    lit1 = Literal(1)
    lit2 = Literal(2)
    lit3 = Literal(3)

    cover1 = SopCover(ni, cube_list=[[lit0, lit2],
                                     [lit0, lit3],
                                     [lit1, lit2],
                                     [lit1, lit3]])
    src2 = SopCover(ni + 1, cube_list=[[lit0],
                                   [lit1]])

    with pytest.raises(ValueError) as e:
        cover1 /= src2

def test_cover_cube_quotient():
    ni = 10

    lit0 = Literal(0)
    lit1 = Literal(1)
    lit2 = Literal(2)
    lit3 = Literal(3)

    src1 = SopCover(ni, cube_list=[[lit0, lit2],
                                   [lit0, lit3],
                                   [lit1, lit2],
                                   [lit1, lit3]])
    src2 = SopCube(ni, literal_list=[lit0])

    cover1 = src1 / src2

    assert cover1.variable_num == ni
    assert cover1.cube_num == 2
    assert cover1.literal_num() == 2

    cube_list = cover1.literal_list()
    assert len(cube_list) == 2
    assert len(cube_list[0]) == 1
    assert lit2 == cube_list[0][0]
    assert len(cube_list[1]) == 1
    assert lit3 == cube_list[1][0]

def test_cover_cube_quotient_bad():
    ni = 10

    lit0 = Literal(0)
    lit1 = Literal(1)
    lit2 = Literal(2)
    lit3 = Literal(3)

    src1 = SopCover(ni, cube_list=[[lit0, lit2],
                                   [lit0, lit3],
                                   [lit1, lit2],
                                   [lit1, lit3]])
    src2 = SopCube(ni + 1, literal_list=[lit0])

    with pytest.raises(ValueError) as e:
        cover1 = src1 / src2

def test_cover_cube_quotient_int():
    ni = 10

    lit0 = Literal(0)
    lit1 = Literal(1)
    lit2 = Literal(2)
    lit3 = Literal(3)

    cover1 = SopCover(ni, cube_list=[[lit0, lit2],
                                     [lit0, lit3],
                                     [lit1, lit2],
                                     [lit1, lit3]])
    src2 = SopCube(ni, literal_list=[lit0])

    cover1 /= src2

    assert cover1.variable_num == ni
    assert cover1.cube_num == 2
    assert cover1.literal_num() == 2

    cube_list = cover1.literal_list()
    assert len(cube_list) == 2
    assert len(cube_list[0]) == 1
    assert lit2 == cube_list[0][0]
    assert len(cube_list[1]) == 1
    assert lit3 == cube_list[1][0]

def test_cover_cube_quotient_int_bad():
    ni = 10

    lit0 = Literal(0)
    lit1 = Literal(1)
    lit2 = Literal(2)
    lit3 = Literal(3)

    cover1 = SopCover(ni, cube_list=[[lit0, lit2],
                                     [lit0, lit3],
                                     [lit1, lit2],
                                     [lit1, lit3]])
    src2 = SopCube(ni + 1, literal_list=[lit0])

    with pytest.raises(ValueError) as e:
        cover1 /= src2

def test_cover_literal_quotient():
    ni = 10

    lit0 = Literal(0)
    lit1 = Literal(1)
    lit2 = Literal(2)
    lit3 = Literal(3)

    src1 = SopCover(ni, cube_list=[[lit0, lit2],
                                   [lit0, lit3],
                                   [lit1, lit2],
                                   [lit1, lit3]])
    src2 = lit0

    cover1 = src1 / src2

    assert cover1.variable_num == ni
    assert cover1.cube_num == 2
    assert cover1.literal_num() == 2

    cube_list = cover1.literal_list()
    assert len(cube_list) == 2
    assert len(cube_list[0]) == 1
    assert lit2 == cube_list[0][0]
    assert len(cube_list[1]) == 1
    assert lit3 == cube_list[1][0]

def test_cover_literal_quotient_int():
    ni = 10

    lit0 = Literal(0)
    lit1 = Literal(1)
    lit2 = Literal(2)
    lit3 = Literal(3)

    cover1 = SopCover(ni, cube_list=[[lit0, lit2],
                                     [lit0, lit3],
                                     [lit1, lit2],
                                     [lit1, lit3]])
    src2 = lit0

    cover1 /= src2

    assert cover1.variable_num == ni
    assert cover1.cube_num == 2
    assert cover1.literal_num() == 2

    cube_list = cover1.literal_list()
    assert len(cube_list) == 2
    assert len(cube_list[0]) == 1
    assert lit2 == cube_list[0][0]
    assert len(cube_list[1]) == 1
    assert lit3 == cube_list[1][0]

def test_common_cube():
    ni = 10

    lit0 = Literal(0)
    lit1 = Literal(1)
    lit2 = Literal(2)
    lit3 = Literal(3)
    lit4 = Literal(4)

    src1 = SopCover(ni, cube_list=[[lit0, lit2],
                                   [lit0, lit3],
                                   [lit0, ~lit4]])

    cube1 = src1.common_cube()

    assert cube1.variable_num == ni
    assert cube1.literal_num == 1

    lit_list = cube1.literal_list()
    assert len(lit_list) == 1
    assert lit0 == lit_list[0]

def test_to_expr():
    ni = 10

    lit0 = Literal(0)
    lit1 = Literal(1)
    lit2 = Literal(2)

    cover1 = SopCover(ni, cube_list=[[lit0, lit1], [~lit2]])

    expr = cover1.expr()
    expr_str = str(expr)
    assert expr_str == "( ( 0 & 1 ) | ~2 )"
    
