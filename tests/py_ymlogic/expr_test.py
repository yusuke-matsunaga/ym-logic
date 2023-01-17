#! /usr/bin/env python3

"""Expr のテストプログラム

:file: expr_test.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2023 Yusuke Matsunaga, All rights reserved.
"""

import pytest
from ymlogic import Expr


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
    assert len(expr.operand_list) == 0
    assert not expr.is_simple()
    assert not expr.is_simple_and()
    assert not expr.is_simple_or()
    assert not expr.is_simple_xor()
    assert not expr.is_sop()

    assert expr.literal_num == 0
    assert expr.input_size == 0
    assert expr.sop_literal_num == 0

    rep_str = expr.__repr__()
    expr2 = Expr(rep_str)
    assert expr == expr2
