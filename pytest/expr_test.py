#! /usr/bin/env python3

"""Expr のテストプログラム

:file: expr_test.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2023 Yusuke Matsunaga, All rights reserved.
"""

import pytest
from ymlogic import Expr, Literal, PrimType


def test_empty_constr():
    expr = Expr()

    assert not expr.is_valid()
    assert expr.is_invalid()
    assert not expr.is_one()
    assert not expr.is_zero()
    assert not expr.is_constant()
    assert not expr.is_posi_literal()
    assert not expr.is_nega_literal()
    assert not expr.is_literal()
    assert not expr.is_and()
    assert not expr.is_or()
    assert not expr.is_xor()
    assert not expr.is_op()
    assert len(expr.get_operand_list()) == 0
    assert not expr.is_simple()
    assert not expr.is_simple_and()
    assert not expr.is_simple_or()
    assert not expr.is_simple_xor()
    assert not expr.is_sop()

    assert expr.literal_num() == 0
    assert expr.sop_literal_num() == 0

    assert expr.input_size == 0

    rep_str = expr.__repr__()
    expr2 = Expr(rep_str)
    assert expr == expr2


def test_invalid():
    expr = Expr.invalid()

    assert not expr.is_valid()
    assert expr.is_invalid()
    assert not expr.is_one()
    assert not expr.is_zero()
    assert not expr.is_constant()
    assert not expr.is_posi_literal()
    assert not expr.is_nega_literal()
    assert not expr.is_literal()
    assert not expr.is_and()
    assert not expr.is_or()
    assert not expr.is_xor()
    assert not expr.is_op()
    assert len(expr.get_operand_list()) == 0
    assert not expr.is_simple()
    assert not expr.is_simple_and()
    assert not expr.is_simple_or()
    assert not expr.is_simple_xor()
    assert not expr.is_sop()

    assert expr.literal_num() == 0
    assert expr.sop_literal_num() == 0

    assert expr.input_size == 0

    rep_str = expr.__repr__()
    expr2 = Expr(rep_str)
    assert expr == expr2

    rep_str = expr.__repr__()
    dup_expr = Expr(rep_str)
    assert expr == dup_expr


def test_zero():
    expr = Expr.zero()

    assert expr.is_valid()
    assert not expr.is_invalid()
    assert not expr.is_one()
    assert expr.is_zero()
    assert expr.is_constant()
    assert not expr.is_posi_literal()
    assert not expr.is_nega_literal()
    assert not expr.is_literal()
    assert not expr.is_and()
    assert not expr.is_or()
    assert not expr.is_xor()
    assert not expr.is_op()
    assert len(expr.get_operand_list()) == 0
    assert expr.is_simple()
    assert not expr.is_simple_and()
    assert not expr.is_simple_or()
    assert not expr.is_simple_xor()
    assert expr.is_sop()

    assert expr.literal_num() == 0
    assert expr.sop_literal_num() == 0

    assert expr.input_size == 0

    rep_str = expr.__repr__()
    expr2 = Expr(rep_str)
    assert expr == expr2

    rep_str = expr.__repr__()
    dup_expr = Expr(rep_str)
    assert expr == dup_expr


def test_one():
    expr = Expr.one()

    assert expr.is_valid()
    assert not expr.is_invalid()
    assert expr.is_one()
    assert not expr.is_zero()
    assert expr.is_constant()
    assert not expr.is_posi_literal()
    assert not expr.is_nega_literal()
    assert not expr.is_literal()
    assert not expr.is_and()
    assert not expr.is_or()
    assert not expr.is_xor()
    assert not expr.is_op()
    assert len(expr.get_operand_list()) == 0
    assert expr.is_simple()
    assert not expr.is_simple_and()
    assert not expr.is_simple_or()
    assert not expr.is_simple_xor()
    assert expr.is_sop()

    assert expr.literal_num() == 0
    assert expr.sop_literal_num() == 0

    assert expr.input_size == 0

    rep_str = expr.__repr__()
    expr2 = Expr(rep_str)
    assert expr == expr2

    rep_str = expr.__repr__()
    dup_expr = Expr(rep_str)
    assert expr == dup_expr


def test_literal1():
    var0 = 0
    expr = Expr.literal(var0)

    assert expr.is_valid()
    assert not expr.is_invalid()
    assert not expr.is_one()
    assert not expr.is_zero()
    assert not expr.is_constant()
    assert expr.is_posi_literal()
    assert not expr.is_nega_literal()
    assert expr.is_literal()
    assert not expr.is_and()
    assert not expr.is_or()
    assert not expr.is_xor()
    assert not expr.is_op()
    assert len(expr.get_operand_list()) == 0
    assert expr.is_simple()
    assert not expr.is_simple_and()
    assert not expr.is_simple_or()
    assert not expr.is_simple_xor()
    assert expr.is_sop()

    assert expr.literal_num() == 1
    for i in range(10):
        exp_val = 1 if i == var0 else 0
        assert expr.literal_num(var=i) == exp_val
    assert expr.literal_num(var0, inv=False) == 1
    assert expr.literal_num(Literal(var0, inv=False)) == 1
    assert expr.literal_num(var=var0, inv=True) == 0
    assert expr.literal_num(var=Literal(var0, inv=False)) == 1

    assert expr.sop_literal_num() == 1
    for i in range(10):
        exp_val = 1 if i == var0 else 0
        assert expr.sop_literal_num(var=i) == exp_val
    assert expr.sop_literal_num(var0, inv=False) == 1
    assert expr.sop_literal_num(var0, inv=True) == 0
    assert expr.sop_literal_num(Literal(var0, inv=False)) == 1

    assert expr.input_size == var0 + 1
    assert expr.get_varid() == var0
    assert expr.get_literal() == Literal(var0)

    rep_str = expr.__repr__()
    expr2 = Expr(rep_str)
    assert expr == expr2

    rep_str = expr.__repr__()
    dup_expr = Expr(rep_str)
    assert expr == dup_expr


def test_literal2():
    var0 = 0
    expr = Expr.literal(var0, inv=False)

    assert expr.is_valid()
    assert not expr.is_invalid()
    assert not expr.is_one()
    assert not expr.is_zero()
    assert not expr.is_constant()
    assert expr.is_posi_literal()
    assert not expr.is_nega_literal()
    assert expr.is_literal()
    assert not expr.is_and()
    assert not expr.is_or()
    assert not expr.is_xor()
    assert not expr.is_op()
    assert len(expr.get_operand_list()) == 0
    assert expr.is_simple()
    assert not expr.is_simple_and()
    assert not expr.is_simple_or()
    assert not expr.is_simple_xor()
    assert expr.is_sop()

    assert expr.literal_num() == 1
    assert expr.sop_literal_num() == 1

    assert expr.input_size == var0 + 1
    assert expr.get_varid() == var0
    assert expr.get_literal() == Literal(var0)

    rep_str = expr.__repr__()
    expr2 = Expr(rep_str)
    assert expr == expr2

    rep_str = expr.__repr__()
    dup_expr = Expr(rep_str)
    assert expr == dup_expr


def test_literal3():
    var0 = 0
    expr = Expr.literal(var=var0, inv=True)

    assert expr.is_valid()
    assert not expr.is_invalid()
    assert not expr.is_one()
    assert not expr.is_zero()
    assert not expr.is_constant()
    assert not expr.is_posi_literal()
    assert expr.is_nega_literal()
    assert expr.is_literal()
    assert not expr.is_and()
    assert not expr.is_or()
    assert not expr.is_xor()
    assert not expr.is_op()
    assert len(expr.get_operand_list()) == 0
    assert expr.is_simple()
    assert not expr.is_simple_and()
    assert not expr.is_simple_or()
    assert not expr.is_simple_xor()
    assert expr.is_sop()

    assert expr.literal_num() == 1
    assert expr.sop_literal_num() == 1

    assert expr.input_size == var0 + 1
    assert expr.get_varid() == var0
    assert expr.get_literal() == Literal(var0, inv=True)

    rep_str = expr.__repr__()
    expr2 = Expr(rep_str)
    assert expr == expr2

    rep_str = expr.__repr__()
    dup_expr = Expr(rep_str)
    assert expr == dup_expr


def test_literal4():
    var0 = 0
    lit = Literal(var0, inv=False)
    expr = Expr.literal(lit)

    assert expr.is_valid()
    assert not expr.is_invalid()
    assert not expr.is_one()
    assert not expr.is_zero()
    assert not expr.is_constant()
    assert expr.is_posi_literal()
    assert not expr.is_nega_literal()
    assert expr.is_literal()
    assert not expr.is_and()
    assert not expr.is_or()
    assert not expr.is_xor()
    assert not expr.is_op()
    assert len(expr.get_operand_list()) == 0
    assert expr.is_simple()
    assert not expr.is_simple_and()
    assert not expr.is_simple_or()
    assert not expr.is_simple_xor()
    assert expr.is_sop()

    assert expr.literal_num() == 1
    for i in range(10):
        exp_val = 1 if i == var0 else 0
        assert expr.literal_num(i) == exp_val
    assert expr.literal_num(var0, inv=False) == 1
    assert expr.literal_num(var0, inv=True) == 0
    assert expr.literal_num(Literal(var0, inv=False)) == 1

    assert expr.sop_literal_num() == 1
    for i in range(10):
        exp_val = 1 if i == var0 else 0
        assert expr.sop_literal_num(i) == exp_val
    assert expr.sop_literal_num(var0, inv=False) == 1
    assert expr.sop_literal_num(var0, inv=True) == 0
    assert expr.sop_literal_num(Literal(var0, inv=False)) == 1

    assert expr.input_size == var0 + 1
    assert expr.get_varid() == var0
    assert expr.get_literal() == Literal(var0)

    rep_str = expr.__repr__()
    expr2 = Expr(rep_str)
    assert expr == expr2

    rep_str = expr.__repr__()
    dup_expr = Expr(rep_str)
    assert expr == dup_expr


def test_posi_literal():
    var0 = 1
    expr = Expr.posi_literal(var=var0)

    assert expr.is_valid()
    assert not expr.is_invalid()
    assert not expr.is_one()
    assert not expr.is_zero()
    assert not expr.is_constant()
    assert expr.is_posi_literal()
    assert not expr.is_nega_literal()
    assert expr.is_literal()
    assert not expr.is_and()
    assert not expr.is_or()
    assert not expr.is_xor()
    assert not expr.is_op()
    assert len(expr.get_operand_list()) == 0
    assert expr.is_simple()
    assert not expr.is_simple_and()
    assert not expr.is_simple_or()
    assert not expr.is_simple_xor()
    assert expr.is_sop()

    assert expr.literal_num() == 1
    assert expr.sop_literal_num() == 1

    assert expr.input_size == var0 + 1
    assert expr.get_varid() == var0

    rep_str = expr.__repr__()
    expr2 = Expr(rep_str)
    assert expr == expr2

    rep_str = expr.__repr__()
    dup_expr = Expr(rep_str)
    assert expr == dup_expr


def test_nega_literal():
    var0 = 2
    expr = Expr.nega_literal(var0)

    assert expr.is_valid()
    assert not expr.is_invalid()
    assert not expr.is_one()
    assert not expr.is_zero()
    assert not expr.is_constant()
    assert not expr.is_posi_literal()
    assert expr.is_nega_literal()
    assert expr.is_literal()
    assert not expr.is_and()
    assert not expr.is_or()
    assert not expr.is_xor()
    assert not expr.is_op()
    assert len(expr.get_operand_list()) == 0
    assert expr.is_simple()
    assert not expr.is_simple_and()
    assert not expr.is_simple_or()
    assert not expr.is_simple_xor()
    assert expr.is_sop()

    assert expr.literal_num() == 1
    assert expr.sop_literal_num() == 1

    assert expr.input_size == var0 + 1
    assert expr.get_varid() == var0

    rep_str = expr.__repr__()
    dup_expr = Expr(rep_str)
    assert expr == dup_expr


def test_and_op1():
    var0 = 2
    var1 = 5
    lit0p = Expr.posi_literal(var0)
    lit1n = Expr.nega_literal(var1)
    expr = lit0p & lit1n

    assert expr.is_valid()
    assert not expr.is_invalid()
    assert not expr.is_one()
    assert not expr.is_zero()
    assert not expr.is_constant()
    assert not expr.is_posi_literal()
    assert not expr.is_nega_literal()
    assert not expr.is_literal()
    assert expr.is_and()
    assert not expr.is_or()
    assert not expr.is_xor()
    assert expr.is_op()
    assert len(expr.get_operand_list()) == 2
    assert expr.is_simple()
    assert expr.is_simple_and()
    assert not expr.is_simple_or()
    assert not expr.is_simple_xor()
    assert expr.is_sop()

    assert expr.literal_num() == 2
    assert expr.sop_literal_num() == 2

    assert expr.input_size == 6

    expr0, expr1 = expr.get_operand_list()

    assert expr0.is_posi_literal()
    assert expr0.get_varid() == var0

    assert expr1.is_nega_literal()
    assert expr1.get_varid() == var1

    rep_str = expr.__repr__()
    dup_expr = Expr(rep_str)
    assert expr == dup_expr


def test_and_op2():
    var0 = 2
    var1 = 5
    lit0p = Expr.posi_literal(var0)
    lit1n = Expr.nega_literal(var1)
    expr = lit0p
    expr &= lit1n

    assert expr.is_valid()
    assert not expr.is_invalid()
    assert not expr.is_one()
    assert not expr.is_zero()
    assert not expr.is_constant()
    assert not expr.is_posi_literal()
    assert not expr.is_nega_literal()
    assert not expr.is_literal()
    assert expr.is_and()
    assert not expr.is_or()
    assert not expr.is_xor()
    assert expr.is_op()
    assert len(expr.get_operand_list()) == 2
    assert expr.is_simple()
    assert expr.is_simple_and()
    assert not expr.is_simple_or()
    assert not expr.is_simple_xor()
    assert expr.is_sop()

    assert expr.literal_num() == 2
    assert expr.sop_literal_num() == 2

    assert expr.input_size == 6

    expr0, expr1 = expr.get_operand_list()

    assert expr0.is_posi_literal()
    assert expr0.get_varid() == var0

    assert expr1.is_nega_literal()
    assert expr1.get_varid() == var1

    rep_str = expr.__repr__()
    dup_expr = Expr(rep_str)
    assert expr == dup_expr


def test_and1():
    var0 = 2
    var1 = 5
    lit0p = Expr.posi_literal(var0)
    lit1n = Expr.nega_literal(var1)
    expr = Expr.and_op([lit0p, lit1n])

    assert expr.is_valid()
    assert not expr.is_invalid()
    assert not expr.is_one()
    assert not expr.is_zero()
    assert not expr.is_constant()
    assert not expr.is_posi_literal()
    assert not expr.is_nega_literal()
    assert not expr.is_literal()
    assert expr.is_and()
    assert not expr.is_or()
    assert not expr.is_xor()
    assert expr.is_op()
    assert len(expr.get_operand_list()) == 2
    assert expr.is_simple()
    assert expr.is_simple_and()
    assert not expr.is_simple_or()
    assert not expr.is_simple_xor()
    assert expr.is_sop()

    assert expr.literal_num() == 2
    assert expr.sop_literal_num() == 2

    assert expr.input_size == 6
    expr0, expr1 = expr.get_operand_list()

    assert expr0.is_posi_literal()
    assert expr0.get_varid() == var0

    assert expr1.is_nega_literal()
    assert expr1.get_varid() == var1

    rep_str = expr.__repr__()
    dup_expr = Expr(rep_str)
    assert expr == dup_expr


def test_or_op1():
    var0 = 1
    var1 = 7
    lit0p = Expr.posi_literal(var0)
    lit1n = Expr.nega_literal(var1)
    expr = lit0p | lit1n

    assert expr.is_valid()
    assert not expr.is_invalid()
    assert not expr.is_one()
    assert not expr.is_zero()
    assert not expr.is_constant()
    assert not expr.is_posi_literal()
    assert not expr.is_nega_literal()
    assert not expr.is_literal()
    assert not expr.is_and()
    assert expr.is_or()
    assert not expr.is_xor()
    assert expr.is_op()
    assert len(expr.get_operand_list()) == 2
    assert expr.is_simple()
    assert not expr.is_simple_and()
    assert expr.is_simple_or()
    assert not expr.is_simple_xor()
    assert expr.is_sop()

    assert expr.literal_num() == 2
    assert expr.sop_literal_num() == 2

    assert expr.input_size == 8
    expr0, expr1 = expr.get_operand_list()

    assert expr0.is_posi_literal()
    assert expr0.get_varid() == var0

    assert expr1.is_nega_literal()
    assert expr1.get_varid() == var1

    rep_str = expr.__repr__()
    dup_expr = Expr(rep_str)
    assert expr == dup_expr


def test_or_op2():
    var0 = 1
    var1 = 7
    lit0p = Expr.posi_literal(var0)
    lit1n = Expr.nega_literal(var1)
    expr = lit0p
    expr |= lit1n

    assert expr.is_valid()
    assert not expr.is_invalid()
    assert not expr.is_one()
    assert not expr.is_zero()
    assert not expr.is_constant()
    assert not expr.is_posi_literal()
    assert not expr.is_nega_literal()
    assert not expr.is_literal()
    assert not expr.is_and()
    assert expr.is_or()
    assert not expr.is_xor()
    assert expr.is_op()
    assert len(expr.get_operand_list()) == 2
    assert expr.is_simple()
    assert not expr.is_simple_and()
    assert expr.is_simple_or()
    assert not expr.is_simple_xor()
    assert expr.is_sop()

    assert expr.literal_num() == 2
    assert expr.sop_literal_num() == 2

    assert expr.input_size == max(var0, var1) + 1

    expr0, expr1 = expr.get_operand_list()

    assert expr0.is_posi_literal()
    assert expr0.get_varid() == var0

    assert expr1.is_nega_literal()
    assert expr1.get_varid() == var1

    rep_str = expr.__repr__()
    dup_expr = Expr(rep_str)
    assert expr == dup_expr


def test_or1():
    var0 = 1
    var1 = 2
    lit0p = Expr.posi_literal(var0)
    lit1n = Expr.nega_literal(var1)
    lit_list = [lit0p, lit1n]
    expr = Expr.or_op(lit_list)

    assert expr.is_valid()
    assert not expr.is_invalid()
    assert not expr.is_one()
    assert not expr.is_zero()
    assert not expr.is_constant()
    assert not expr.is_posi_literal()
    assert not expr.is_nega_literal()
    assert not expr.is_literal()
    assert not expr.is_and()
    assert expr.is_or()
    assert not expr.is_xor()
    assert expr.is_op()
    assert len(expr.get_operand_list()) == 2
    assert expr.is_simple()
    assert not expr.is_simple_and()
    assert expr.is_simple_or()
    assert not expr.is_simple_xor()
    assert expr.is_sop()

    assert expr.literal_num() == 2
    assert expr.sop_literal_num() == 2

    assert expr.input_size == max(var0, var1) + 1

    expr0, expr1 = expr.get_operand_list()

    assert expr0.is_posi_literal()
    assert expr0.get_varid() == var0

    assert expr1.is_nega_literal()
    assert expr1.get_varid() == var1

    rep_str = expr.__repr__()
    dup_expr = Expr(rep_str)
    assert expr == dup_expr


def test_xor_op1():
    var0 = 3
    var1 = 4
    lit0p = Expr.posi_literal(var0)
    lit1n = Expr.nega_literal(var1)
    expr = lit0p ^ lit1n

    assert expr.is_valid()
    assert not expr.is_invalid()
    assert not expr.is_one()
    assert not expr.is_zero()
    assert not expr.is_constant()
    assert not expr.is_posi_literal()
    assert not expr.is_nega_literal()
    assert not expr.is_literal()
    assert not expr.is_and()
    assert not expr.is_or()
    assert expr.is_xor()
    assert expr.is_op()
    assert len(expr.get_operand_list()) == 2
    assert expr.is_simple()
    assert not expr.is_simple_and()
    assert not expr.is_simple_or()
    assert expr.is_simple_xor()
    assert not expr.is_sop()

    assert expr.literal_num() == 2
    assert expr.sop_literal_num() == 4

    assert expr.input_size == max(var0, var1) + 1

    expr0, expr1 = expr.get_operand_list()

    assert expr0.is_posi_literal()
    assert expr0.get_varid() == var0

    assert expr1.is_nega_literal()
    assert expr1.get_varid() == var1

    rep_str = expr.__repr__()
    dup_expr = Expr(rep_str)
    assert expr == dup_expr


def test_xor_op2():
    var0 = 3
    var1 = 4
    lit0p = Expr.posi_literal(var0)
    lit1n = Expr.nega_literal(var1)
    expr = lit0p
    expr ^= lit1n

    assert expr.is_valid()
    assert not expr.is_invalid()
    assert not expr.is_one()
    assert not expr.is_zero()
    assert not expr.is_constant()
    assert not expr.is_posi_literal()
    assert not expr.is_nega_literal()
    assert not expr.is_literal()
    assert not expr.is_and()
    assert not expr.is_or()
    assert expr.is_xor()
    assert expr.is_op()
    assert len(expr.get_operand_list()) == 2
    assert expr.is_simple()
    assert not expr.is_simple_and()
    assert not expr.is_simple_or()
    assert expr.is_simple_xor()
    assert not expr.is_sop()

    assert expr.literal_num() == 2
    assert expr.sop_literal_num() == 4

    assert expr.input_size == max(var0, var1) + 1

    expr0, expr1 = expr.get_operand_list()

    assert expr0.is_posi_literal()
    assert expr0.get_varid() == var0

    assert expr1.is_nega_literal()
    assert expr1.get_varid() == var1

    rep_str = expr.__repr__()
    dup_expr = Expr(rep_str)
    assert expr == dup_expr


def test_xor1():
    var0 = 1
    var1 = 2
    lit0p = Expr.posi_literal(var0)
    lit1n = Expr.nega_literal(var1)
    lit_list = [lit0p, lit1n]
    expr = Expr.xor_op(lit_list)

    assert expr.is_valid()
    assert not expr.is_invalid()
    assert not expr.is_one()
    assert not expr.is_zero()
    assert not expr.is_constant()
    assert not expr.is_posi_literal()
    assert not expr.is_nega_literal()
    assert not expr.is_literal()
    assert not expr.is_and()
    assert not expr.is_or()
    assert expr.is_xor()
    assert expr.is_op()
    assert len(expr.get_operand_list()) == 2
    assert expr.is_simple()
    assert not expr.is_simple_and()
    assert not expr.is_simple_or()
    assert expr.is_simple_xor()
    assert not expr.is_sop()

    assert expr.literal_num() == 2
    assert expr.sop_literal_num() == 4

    assert expr.input_size == max(var0, var1) + 1

    expr0, expr1 = expr.get_operand_list()

    assert expr0.is_posi_literal()
    assert expr0.get_varid() == var0

    assert expr1.is_nega_literal()
    assert expr1.get_varid() == var1

    rep_str = expr.__repr__()
    dup_expr = Expr(rep_str)
    assert expr == dup_expr


def test_invert1():
    var0 = 5
    lit0p = Expr.posi_literal(var0)

    expr = ~lit0p

    assert expr.is_valid()
    assert not expr.is_invalid()
    assert not expr.is_one()
    assert not expr.is_zero()
    assert not expr.is_constant()
    assert not expr.is_posi_literal()
    assert expr.is_nega_literal()
    assert expr.is_literal()
    assert not expr.is_and()
    assert not expr.is_or()
    assert not expr.is_xor()
    assert not expr.is_op()
    assert len(expr.get_operand_list()) == 0
    assert expr.is_simple()
    assert not expr.is_simple_and()
    assert not expr.is_simple_or()
    assert not expr.is_simple_xor()
    assert expr.is_sop()

    assert expr.literal_num() == 1
    assert expr.sop_literal_num() == 1

    assert expr.input_size == var0 + 1

    rep_str = expr.__repr__()
    dup_expr = Expr(rep_str)
    assert expr == dup_expr


def test_invert2():
    var0 = 1
    var1 = 2
    lit0p = Expr.posi_literal(var0)
    lit1p = Expr.posi_literal(var1)
    expr0 = lit0p & lit1p

    expr = ~expr0

    assert expr.is_valid()
    assert not expr.is_invalid()
    assert not expr.is_one()
    assert not expr.is_zero()
    assert not expr.is_constant()
    assert not expr.is_posi_literal()
    assert not expr.is_nega_literal()
    assert not expr.is_literal()
    assert not expr.is_and()
    assert expr.is_or()
    assert not expr.is_xor()
    assert expr.is_op()
    assert len(expr.get_operand_list()) == 2
    assert expr.is_simple()
    assert not expr.is_simple_and()
    assert expr.is_simple_or()
    assert not expr.is_simple_xor()
    assert expr.is_sop()

    assert expr.literal_num() == 2
    assert expr.sop_literal_num() == 2

    assert expr.input_size == max(var0, var1) + 1

    expr0, expr1 = expr.get_operand_list()

    assert expr0.is_nega_literal()
    assert expr0.get_varid() == var0

    assert expr1.is_nega_literal()
    assert expr1.get_varid() == var1


def test_compose1():
    lit0 = Expr.posi_literal(0)
    lit1 = Expr.posi_literal(1)
    lit2 = Expr.posi_literal(2)
    lit3 = Expr.posi_literal(3)

    expr0 = lit0 | (~lit1 & lit2)
    expr1 = lit1 & lit3

    expr = expr0.compose({0: expr1})

    expr_str = str(expr)

    assert expr_str == "( ( 1 & 3 ) | ( ~1 & 2 ) )"


def test_compose2():
    lit0 = Expr.posi_literal(0)
    lit1 = Expr.posi_literal(1)
    lit2 = Expr.posi_literal(2)
    lit3 = Expr.posi_literal(3)
    lit4 = Expr.posi_literal(4)

    expr0 = lit0 | ~lit4
    opr1 = lit1 & lit3
    opr2 = ~(~lit1 & lit2)
    comp_map = {0: opr1, 4: opr2}

    expr = expr0.compose(comp_map)

    expr_str = str(expr)

    assert expr_str == "( ( 1 & 3 ) | ( ~1 & 2 ) )"


def test_remap_var1():
    lit0 = Expr.posi_literal(0)
    lit1 = Expr.posi_literal(1)
    lit2 = Expr.posi_literal(2)
    lit3 = Expr.posi_literal(3)

    expr0 = (lit0 & lit3) | (~lit1 & lit2)
    expr = expr0.remap_var({0: 1})

    expr_str = str(expr)

    assert expr_str == "( ( 1 & 3 ) | ( ~1 & 2 ) )"


def test_eval1():
    lit0 = Expr.posi_literal(0)
    lit1 = Expr.posi_literal(1)
    lit2 = Expr.posi_literal(2)
    lit3 = Expr.posi_literal(3)

    expr = (lit0 & lit3) | (~lit1 & lit2)

    vals = [0 for _ in range(4)]
    mask = 0
    exp_val = 0
    for p in range(16):
        mask |= (1 << p)
        val = [False for _ in range(4)]
        for i in range(4):
            if p & (1 << i):
                vals[i] |= 1 << p
                val[i] = True
        if (val[0] and val[3]) or (not val[1] and val[2]):
            exp_val |= 1 << p

    val = expr.eval(vals, mask=mask)

    assert val == exp_val


def test_tv1():
    lit0 = Expr.posi_literal(0)
    lit1 = Expr.posi_literal(1)
    lit2 = Expr.posi_literal(2)
    lit3 = Expr.posi_literal(3)

    expr = (lit0 & lit3) | (~lit1 & lit2)

    f = expr.to_tv()

    for p in range(16):
        val = [True if p & (1 << i) else False for i in range(4)]
        if (val[0] and val[3]) or (not val[1] and val[2]):
            exp_val = 1
        else:
            exp_val = 0
        fval = f.value(p)

        assert fval == exp_val


def test_from_string1():
    expr_str = "0 + (1 * ~2)"
    expr = Expr.from_string(expr_str)

    assert expr.is_valid()

    tmp_str = str(expr)
    exp_str = "( 0 | ( 1 & ~2 ) )"
    assert tmp_str == exp_str


def test_bad_from_string1():
    expr_str = "0 + + (1 * ~2)"
    with pytest.raises(Exception) as e:
        _ = Expr.from_string(expr_str)
    assert e.type == ValueError
    assert str(e.value) == "invalid argument"

    
def test_analyze_c0():
    expr = Expr.zero()

    ptype = expr.analyze()
    assert ptype == PrimType.C0

    
def test_analyze_c1():
    expr = Expr.one()

    ptype = expr.analyze()
    assert ptype == PrimType.C1

    
def test_analyze_buff():
    expr = Expr.from_string("0")

    ptype = expr.analyze()
    assert ptype == PrimType.Buff

    
def test_analyze_not():
    expr = Expr.from_string("~0")

    ptype = expr.analyze()
    assert ptype == PrimType.Not

    
def test_analyze_and():
    expr = Expr.from_string("0 & 1")

    ptype = expr.analyze()
    assert ptype == PrimType.And

    
def test_analyze_or():
    expr = Expr.from_string("0 | 1")

    ptype = expr.analyze()
    assert ptype == PrimType.Or

    
def test_analyze_xor():
    expr = Expr.from_string("0 ^ 1")

    ptype = expr.analyze()
    assert ptype == PrimType.Xor
