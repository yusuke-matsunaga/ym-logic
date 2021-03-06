#! /usr/bin/env python3

### @file expr_test.py
### @brief Expr のテストスクリプト
### @author Yusuke Matsunaga (松永 裕介)
###
### Copyright (C) 2018 Yusuke Matsunaga
### All rights reserved.

import unittest
from pym_logic import VarId
from pym_logic import Literal
from pym_logic import Expr


# Expr のテスト用クラス
class ExprTest(unittest.TestCase) :

    def runTest(self) :
        self.test_empty_constr()
        self.test_zero()
        self.test_one()
        self.test_literal1()
        self.test_literal2()
        self.test_posi_literal()
        self.test_nega_literal()
        self.test_and1()
        self.test_and2()
        self.test_or1()
        self.test_or2()
        self.test_xor1()
        self.test_xor2()

    def test_empty_constr(self) :
        expr = Expr()
        self.assertTrue( expr != None )
        self.assertTrue( expr.is_zero() )
        self.assertFalse( expr.is_one() )
        self.assertTrue( expr.is_constant() )
        self.assertFalse( expr.is_posi_literal() )
        self.assertFalse( expr.is_nega_literal() )
        self.assertFalse( expr.is_literal() )
        self.assertFalse( expr.is_and() )
        self.assertFalse( expr.is_or() )
        self.assertFalse( expr.is_xor() )
        self.assertFalse( expr.is_op() )
        self.assertEqual( 0, len(expr.child_list) )
        self.assertTrue( expr.is_simple() )
        self.assertFalse( expr.is_simple_and() )
        self.assertFalse( expr.is_simple_or() )
        self.assertFalse( expr.is_simple_xor() )
        self.assertTrue( expr.is_sop() )
        self.assertEqual( 0, expr.literal_num() )
        self.assertEqual( 0, expr.input_size )
        self.assertEqual( 0, expr.sop_literal_num() )

    def test_zero(self) :
        expr = Expr.make_zero()
        self.assertTrue( expr != None )
        self.assertTrue( expr.is_zero() )
        self.assertFalse( expr.is_one() )
        self.assertTrue( expr.is_constant() )
        self.assertFalse( expr.is_posi_literal() )
        self.assertFalse( expr.is_nega_literal() )
        self.assertFalse( expr.is_literal() )
        self.assertFalse( expr.is_and() )
        self.assertFalse( expr.is_or() )
        self.assertFalse( expr.is_xor() )
        self.assertFalse( expr.is_op() )
        self.assertEqual( 0, len(expr.child_list) )
        self.assertTrue( expr.is_simple() )
        self.assertFalse( expr.is_simple_and() )
        self.assertFalse( expr.is_simple_or() )
        self.assertFalse( expr.is_simple_xor() )
        self.assertTrue( expr.is_sop() )
        self.assertEqual( 0, expr.literal_num() )
        self.assertEqual( 0, expr.input_size )
        self.assertEqual( 0, expr.sop_literal_num() )

    def test_one(self) :
        expr = Expr.make_one()
        self.assertTrue( expr != None )
        self.assertFalse( expr.is_zero() )
        self.assertTrue( expr.is_one() )
        self.assertTrue( expr.is_constant() )
        self.assertFalse( expr.is_posi_literal() )
        self.assertFalse( expr.is_nega_literal() )
        self.assertFalse( expr.is_literal() )
        self.assertFalse( expr.is_and() )
        self.assertFalse( expr.is_or() )
        self.assertFalse( expr.is_xor() )
        self.assertFalse( expr.is_op() )
        self.assertEqual( 0, len(expr.child_list) )
        self.assertTrue( expr.is_simple() )
        self.assertFalse( expr.is_simple_and() )
        self.assertFalse( expr.is_simple_or() )
        self.assertFalse( expr.is_simple_xor() )
        self.assertTrue( expr.is_sop() )
        self.assertEqual( 0, expr.literal_num() )
        self.assertEqual( 0, expr.input_size )
        self.assertEqual( 0, expr.sop_literal_num() )

    def test_literal1(self) :
        var = VarId(0)
        expr = Expr.make_literal(var, False)
        self.assertTrue( expr != None )
        self.assertFalse( expr.is_zero() )
        self.assertFalse( expr.is_one() )
        self.assertFalse( expr.is_constant() )
        self.assertTrue( expr.is_posi_literal() )
        self.assertFalse( expr.is_nega_literal() )
        self.assertTrue( expr.is_literal() )
        self.assertEqual( var, expr.varid )
        self.assertFalse( expr.is_and() )
        self.assertFalse( expr.is_or() )
        self.assertFalse( expr.is_xor() )
        self.assertFalse( expr.is_op() )
        self.assertEqual( 0, len(expr.child_list) )
        self.assertTrue( expr.is_simple() )
        self.assertFalse( expr.is_simple_and() )
        self.assertFalse( expr.is_simple_or() )
        self.assertFalse( expr.is_simple_xor() )
        self.assertTrue( expr.is_sop() )
        self.assertEqual( 1, expr.literal_num() )
        self.assertEqual( 1, expr.input_size )
        self.assertEqual( 1, expr.sop_literal_num() )

    def test_literal2(self) :
        var = VarId(0)
        expr = Expr.make_literal(var, True)
        self.assertTrue( expr != None )
        self.assertFalse( expr.is_zero() )
        self.assertFalse( expr.is_one() )
        self.assertFalse( expr.is_constant() )
        self.assertFalse( expr.is_posi_literal() )
        self.assertTrue( expr.is_nega_literal() )
        self.assertTrue( expr.is_literal() )
        self.assertEqual( var, expr.varid )
        self.assertFalse( expr.is_and() )
        self.assertFalse( expr.is_or() )
        self.assertFalse( expr.is_xor() )
        self.assertFalse( expr.is_op() )
        self.assertEqual( 0, len(expr.child_list) )
        self.assertTrue( expr.is_simple() )
        self.assertFalse( expr.is_simple_and() )
        self.assertFalse( expr.is_simple_or() )
        self.assertFalse( expr.is_simple_xor() )
        self.assertTrue( expr.is_sop() )
        self.assertEqual( 1, expr.literal_num() )
        self.assertEqual( 1, expr.input_size )
        self.assertEqual( 1, expr.sop_literal_num() )

    def test_posi_literal(self) :
        var = VarId(0)
        expr = Expr.make_posi_literal(var)
        self.assertTrue( expr != None )
        self.assertFalse( expr.is_zero() )
        self.assertFalse( expr.is_one() )
        self.assertFalse( expr.is_constant() )
        self.assertTrue( expr.is_posi_literal() )
        self.assertFalse( expr.is_nega_literal() )
        self.assertTrue( expr.is_literal() )
        self.assertEqual( var, expr.varid )
        self.assertFalse( expr.is_and() )
        self.assertFalse( expr.is_or() )
        self.assertFalse( expr.is_xor() )
        self.assertFalse( expr.is_op() )
        self.assertEqual( 0, len(expr.child_list) )
        self.assertTrue( expr.is_simple() )
        self.assertFalse( expr.is_simple_and() )
        self.assertFalse( expr.is_simple_or() )
        self.assertFalse( expr.is_simple_xor() )
        self.assertTrue( expr.is_sop() )
        self.assertEqual( 1, expr.literal_num() )
        self.assertEqual( 1, expr.input_size )
        self.assertEqual( 1, expr.sop_literal_num() )

    def test_nega_literal(self) :
        var = VarId(0)
        expr = Expr.make_nega_literal(var)
        self.assertTrue( expr != None )
        self.assertFalse( expr.is_zero() )
        self.assertFalse( expr.is_one() )
        self.assertFalse( expr.is_constant() )
        self.assertFalse( expr.is_posi_literal() )
        self.assertTrue( expr.is_nega_literal() )
        self.assertTrue( expr.is_literal() )
        self.assertEqual( var, expr.varid )
        self.assertFalse( expr.is_and() )
        self.assertFalse( expr.is_or() )
        self.assertFalse( expr.is_xor() )
        self.assertFalse( expr.is_op() )
        self.assertEqual( 0, len(expr.child_list) )
        self.assertTrue( expr.is_simple() )
        self.assertFalse( expr.is_simple_and() )
        self.assertFalse( expr.is_simple_or() )
        self.assertFalse( expr.is_simple_xor() )
        self.assertTrue( expr.is_sop() )
        self.assertEqual( 1, expr.literal_num() )
        self.assertEqual( 1, expr.input_size )
        self.assertEqual( 1, expr.sop_literal_num() )

    def test_and1(self) :
        var0 = VarId(0)
        var1 = VarId(1)
        lit0p = Expr.make_posi_literal(var0)
        lit1n = Expr.make_nega_literal(var1)
        expr = lit0p & lit1n
        self.assertTrue( expr != None )
        self.assertFalse( expr.is_zero() )
        self.assertFalse( expr.is_one() )
        self.assertFalse( expr.is_constant() )
        self.assertFalse( expr.is_posi_literal() )
        self.assertFalse( expr.is_nega_literal() )
        self.assertFalse( expr.is_literal() )
        self.assertTrue( expr.is_and() )
        self.assertFalse( expr.is_or() )
        self.assertFalse( expr.is_xor() )
        self.assertTrue( expr.is_op() )
        self.assertEqual( 2, len(expr.child_list) )

        expr0 = expr.child_list[0]
        self.assertTrue( expr0.is_posi_literal() )
        self.assertEqual( var0, expr0.varid )

        expr1 = expr.child_list[1]
        self.assertTrue( expr1.is_nega_literal() )
        self.assertEqual( var1, expr1.varid )

        self.assertTrue( expr.is_simple() )
        self.assertTrue( expr.is_simple_and() )
        self.assertFalse( expr.is_simple_or() )
        self.assertFalse( expr.is_simple_xor() )
        self.assertTrue( expr.is_sop() )

        self.assertEqual( 2, expr.literal_num() )
        self.assertEqual( 1, expr.literal_num(var0) )
        self.assertEqual( 1, expr.literal_num(var0, False) )
        self.assertEqual( 0, expr.literal_num(var0, True) )
        self.assertEqual( 1, expr.literal_num(var1) )
        self.assertEqual( 0, expr.literal_num(var1, False) )
        self.assertEqual( 1, expr.literal_num(var1, True) )
        self.assertEqual( 2, expr.input_size )
        self.assertEqual( 2, expr.sop_literal_num() )

    def test_and2(self) :
        var0 = VarId(0)
        var1 = VarId(1)
        lit0p = Expr.make_posi_literal(var0)
        lit1n = Expr.make_nega_literal(var1)
        expr = Expr.make_and([lit0p, lit1n])
        self.assertTrue( expr != None )
        self.assertFalse( expr.is_zero() )
        self.assertFalse( expr.is_one() )
        self.assertFalse( expr.is_constant() )
        self.assertFalse( expr.is_posi_literal() )
        self.assertFalse( expr.is_nega_literal() )
        self.assertFalse( expr.is_literal() )
        self.assertTrue( expr.is_and() )
        self.assertFalse( expr.is_or() )
        self.assertFalse( expr.is_xor() )
        self.assertTrue( expr.is_op() )
        self.assertEqual( 2, len(expr.child_list) )

        expr0 = expr.child_list[0]
        self.assertTrue( expr0.is_posi_literal() )
        self.assertEqual( var0, expr0.varid )

        expr1 = expr.child_list[1]
        self.assertTrue( expr1.is_nega_literal() )
        self.assertEqual( var1, expr1.varid )

        self.assertTrue( expr.is_simple() )
        self.assertTrue( expr.is_simple_and() )
        self.assertFalse( expr.is_simple_or() )
        self.assertFalse( expr.is_simple_xor() )
        self.assertTrue( expr.is_sop() )

        self.assertEqual( 2, expr.literal_num() )
        self.assertEqual( 1, expr.literal_num(var0) )
        self.assertEqual( 1, expr.literal_num(var0, False) )
        self.assertEqual( 0, expr.literal_num(var0, True) )
        self.assertEqual( 1, expr.literal_num(var1) )
        self.assertEqual( 0, expr.literal_num(var1, False) )
        self.assertEqual( 1, expr.literal_num(var1, True) )
        self.assertEqual( 2, expr.input_size )
        self.assertEqual( 2, expr.sop_literal_num() )

    def test_and3(self) :
        var0 = VarId(0)
        var1 = VarId(1)
        lit0p = Expr.make_posi_literal(var0)
        lit1n = Expr.make_nega_literal(var1)
        expr = Expr.make_and(lit0p, lit1n)
        self.assertTrue( expr != None )
        self.assertFalse( expr.is_zero() )
        self.assertFalse( expr.is_one() )
        self.assertFalse( expr.is_constant() )
        self.assertFalse( expr.is_posi_literal() )
        self.assertFalse( expr.is_nega_literal() )
        self.assertFalse( expr.is_literal() )
        self.assertTrue( expr.is_and() )
        self.assertFalse( expr.is_or() )
        self.assertFalse( expr.is_xor() )
        self.assertTrue( expr.is_op() )
        self.assertEqual( 2, len(expr.child_list) )

        expr0 = expr.child_list[0]
        self.assertTrue( expr0.is_posi_literal() )
        self.assertEqual( var0, expr0.varid )

        expr1 = expr.child_list[1]
        self.assertTrue( expr1.is_nega_literal() )
        self.assertEqual( var1, expr1.varid )

        self.assertTrue( expr.is_simple() )
        self.assertTrue( expr.is_simple_and() )
        self.assertFalse( expr.is_simple_or() )
        self.assertFalse( expr.is_simple_xor() )
        self.assertTrue( expr.is_sop() )

        self.assertEqual( 2, expr.literal_num() )
        self.assertEqual( 1, expr.literal_num(var0) )
        self.assertEqual( 1, expr.literal_num(var0, False) )
        self.assertEqual( 0, expr.literal_num(var0, True) )
        self.assertEqual( 1, expr.literal_num(var1) )
        self.assertEqual( 0, expr.literal_num(var1, False) )
        self.assertEqual( 1, expr.literal_num(var1, True) )
        self.assertEqual( 2, expr.input_size )
        self.assertEqual( 2, expr.sop_literal_num() )

    def test_or1(self) :
        var0 = VarId(0)
        var1 = VarId(1)
        lit0p = Expr.make_posi_literal(var0)
        lit1n = Expr.make_nega_literal(var1)
        expr = lit0p | lit1n
        self.assertTrue( expr != None )
        self.assertFalse( expr.is_zero() )
        self.assertFalse( expr.is_one() )
        self.assertFalse( expr.is_constant() )
        self.assertFalse( expr.is_posi_literal() )
        self.assertFalse( expr.is_nega_literal() )
        self.assertFalse( expr.is_literal() )
        self.assertFalse( expr.is_and() )
        self.assertTrue( expr.is_or() )
        self.assertFalse( expr.is_xor() )
        self.assertTrue( expr.is_op() )
        self.assertEqual( 2, len(expr.child_list) )

        expr0 = expr.child_list[0]
        self.assertTrue( expr0.is_posi_literal() )
        self.assertEqual( var0, expr0.varid )

        expr1 = expr.child_list[1]
        self.assertTrue( expr1.is_nega_literal() )
        self.assertEqual( var1, expr1.varid )

        self.assertTrue( expr.is_simple() )
        self.assertFalse( expr.is_simple_and() )
        self.assertTrue( expr.is_simple_or() )
        self.assertFalse( expr.is_simple_xor() )
        self.assertTrue( expr.is_sop() )

        self.assertEqual( 2, expr.literal_num() )
        self.assertEqual( 1, expr.literal_num(var0) )
        self.assertEqual( 1, expr.literal_num(var0, False) )
        self.assertEqual( 0, expr.literal_num(var0, True) )
        self.assertEqual( 1, expr.literal_num(var1) )
        self.assertEqual( 0, expr.literal_num(var1, False) )
        self.assertEqual( 1, expr.literal_num(var1, True) )
        self.assertEqual( 2, expr.input_size )
        self.assertEqual( 2, expr.sop_literal_num() )

    def test_or2(self) :
        var0 = VarId(0)
        var1 = VarId(1)
        lit0p = Expr.make_posi_literal(var0)
        lit1n = Expr.make_nega_literal(var1)
        expr = Expr.make_or([lit0p, lit1n])
        self.assertTrue( expr != None )
        self.assertFalse( expr.is_zero() )
        self.assertFalse( expr.is_one() )
        self.assertFalse( expr.is_constant() )
        self.assertFalse( expr.is_posi_literal() )
        self.assertFalse( expr.is_nega_literal() )
        self.assertFalse( expr.is_literal() )
        self.assertFalse( expr.is_and() )
        self.assertTrue( expr.is_or() )
        self.assertFalse( expr.is_xor() )
        self.assertTrue( expr.is_op() )
        self.assertEqual( 2, len(expr.child_list) )

        expr0 = expr.child_list[0]
        self.assertTrue( expr0.is_posi_literal() )
        self.assertEqual( var0, expr0.varid )

        expr1 = expr.child_list[1]
        self.assertTrue( expr1.is_nega_literal() )
        self.assertEqual( var1, expr1.varid )

        self.assertTrue( expr.is_simple() )
        self.assertFalse( expr.is_simple_and() )
        self.assertTrue( expr.is_simple_or() )
        self.assertFalse( expr.is_simple_xor() )
        self.assertTrue( expr.is_sop() )

        self.assertEqual( 2, expr.literal_num() )
        self.assertEqual( 1, expr.literal_num(var0) )
        self.assertEqual( 1, expr.literal_num(var0, False) )
        self.assertEqual( 0, expr.literal_num(var0, True) )
        self.assertEqual( 1, expr.literal_num(var1) )
        self.assertEqual( 0, expr.literal_num(var1, False) )
        self.assertEqual( 1, expr.literal_num(var1, True) )
        self.assertEqual( 2, expr.input_size )
        self.assertEqual( 2, expr.sop_literal_num() )

    def test_or3(self) :
        var0 = VarId(0)
        var1 = VarId(1)
        lit0p = Expr.make_posi_literal(var0)
        lit1n = Expr.make_nega_literal(var1)
        expr = Expr.make_or(lit0p, lit1n)
        self.assertTrue( expr != None )
        self.assertFalse( expr.is_zero() )
        self.assertFalse( expr.is_one() )
        self.assertFalse( expr.is_constant() )
        self.assertFalse( expr.is_posi_literal() )
        self.assertFalse( expr.is_nega_literal() )
        self.assertFalse( expr.is_literal() )
        self.assertFalse( expr.is_and() )
        self.assertTrue( expr.is_or() )
        self.assertFalse( expr.is_xor() )
        self.assertTrue( expr.is_op() )
        self.assertEqual( 2, len(expr.child_list) )

        expr0 = expr.child_list[0]
        self.assertTrue( expr0.is_posi_literal() )
        self.assertEqual( var0, expr0.varid )

        expr1 = expr.child_list[1]
        self.assertTrue( expr1.is_nega_literal() )
        self.assertEqual( var1, expr1.varid )

        self.assertTrue( expr.is_simple() )
        self.assertFalse( expr.is_simple_and() )
        self.assertTrue( expr.is_simple_or() )
        self.assertFalse( expr.is_simple_xor() )
        self.assertTrue( expr.is_sop() )

        self.assertEqual( 2, expr.literal_num() )
        self.assertEqual( 1, expr.literal_num(var0) )
        self.assertEqual( 1, expr.literal_num(var0, False) )
        self.assertEqual( 0, expr.literal_num(var0, True) )
        self.assertEqual( 1, expr.literal_num(var1) )
        self.assertEqual( 0, expr.literal_num(var1, False) )
        self.assertEqual( 1, expr.literal_num(var1, True) )
        self.assertEqual( 2, expr.input_size )
        self.assertEqual( 2, expr.sop_literal_num() )

    def test_xor1(self) :
        var0 = VarId(0)
        var1 = VarId(1)
        lit0p = Expr.make_posi_literal(var0)
        lit1n = Expr.make_nega_literal(var1)
        expr = lit0p ^ lit1n
        self.assertTrue( expr != None )
        self.assertFalse( expr.is_zero() )
        self.assertFalse( expr.is_one() )
        self.assertFalse( expr.is_constant() )
        self.assertFalse( expr.is_posi_literal() )
        self.assertFalse( expr.is_nega_literal() )
        self.assertFalse( expr.is_literal() )
        self.assertFalse( expr.is_and() )
        self.assertFalse( expr.is_or() )
        self.assertTrue( expr.is_xor() )
        self.assertTrue( expr.is_op() )
        self.assertEqual( 2, len(expr.child_list) )

        expr0 = expr.child_list[0]
        self.assertTrue( expr0.is_posi_literal() )
        self.assertEqual( var0, expr0.varid )

        expr1 = expr.child_list[1]
        self.assertTrue( expr1.is_nega_literal() )
        self.assertEqual( var1, expr1.varid )

        self.assertTrue( expr.is_simple() )
        self.assertFalse( expr.is_simple_and() )
        self.assertFalse( expr.is_simple_or() )
        self.assertTrue( expr.is_simple_xor() )
        self.assertFalse( expr.is_sop() )

        self.assertEqual( 2, expr.literal_num() )
        self.assertEqual( 1, expr.literal_num(var0) )
        self.assertEqual( 1, expr.literal_num(var0, False) )
        self.assertEqual( 0, expr.literal_num(var0, True) )
        self.assertEqual( 1, expr.literal_num(var1) )
        self.assertEqual( 0, expr.literal_num(var1, False) )
        self.assertEqual( 1, expr.literal_num(var1, True) )
        self.assertEqual( 2, expr.input_size )
        self.assertEqual( 4, expr.sop_literal_num() )

    def test_xor2(self) :
        var0 = VarId(0)
        var1 = VarId(1)
        lit0p = Expr.make_posi_literal(var0)
        lit1n = Expr.make_nega_literal(var1)
        expr = Expr.make_xor([lit0p, lit1n])
        self.assertTrue( expr != None )
        self.assertFalse( expr.is_zero() )
        self.assertFalse( expr.is_one() )
        self.assertFalse( expr.is_constant() )
        self.assertFalse( expr.is_posi_literal() )
        self.assertFalse( expr.is_nega_literal() )
        self.assertFalse( expr.is_literal() )
        self.assertFalse( expr.is_and() )
        self.assertFalse( expr.is_or() )
        self.assertTrue( expr.is_xor() )
        self.assertTrue( expr.is_op() )
        self.assertEqual( 2, len(expr.child_list) )

        expr0 = expr.child_list[0]
        self.assertTrue( expr0.is_posi_literal() )
        self.assertEqual( var0, expr0.varid )

        expr1 = expr.child_list[1]
        self.assertTrue( expr1.is_nega_literal() )
        self.assertEqual( var1, expr1.varid )

        self.assertTrue( expr.is_simple() )
        self.assertFalse( expr.is_simple_and() )
        self.assertFalse( expr.is_simple_or() )
        self.assertTrue( expr.is_simple_xor() )
        self.assertFalse( expr.is_sop() )

        self.assertEqual( 2, expr.literal_num() )
        self.assertEqual( 1, expr.literal_num(var0) )
        self.assertEqual( 1, expr.literal_num(var0, False) )
        self.assertEqual( 0, expr.literal_num(var0, True) )
        self.assertEqual( 1, expr.literal_num(var1) )
        self.assertEqual( 0, expr.literal_num(var1, False) )
        self.assertEqual( 1, expr.literal_num(var1, True) )
        self.assertEqual( 2, expr.input_size )
        self.assertEqual( 4, expr.sop_literal_num() )

    def test_xor3(self) :
        var0 = VarId(0)
        var1 = VarId(1)
        lit0p = Expr.make_posi_literal(var0)
        lit1n = Expr.make_nega_literal(var1)
        expr = Expr.make_xor(lit0p, lit1n)
        self.assertTrue( expr != None )
        self.assertFalse( expr.is_zero() )
        self.assertFalse( expr.is_one() )
        self.assertFalse( expr.is_constant() )
        self.assertFalse( expr.is_posi_literal() )
        self.assertFalse( expr.is_nega_literal() )
        self.assertFalse( expr.is_literal() )
        self.assertFalse( expr.is_and() )
        self.assertFalse( expr.is_or() )
        self.assertTrue( expr.is_xor() )
        self.assertTrue( expr.is_op() )
        self.assertEqual( 2, len(expr.child_list) )

        expr0 = expr.child_list[0]
        self.assertTrue( expr0.is_posi_literal() )
        self.assertEqual( var0, expr0.varid )

        expr1 = expr.child_list[1]
        self.assertTrue( expr1.is_nega_literal() )
        self.assertEqual( var1, expr1.varid )

        self.assertTrue( expr.is_simple() )
        self.assertFalse( expr.is_simple_and() )
        self.assertFalse( expr.is_simple_or() )
        self.assertTrue( expr.is_simple_xor() )
        self.assertFalse( expr.is_sop() )

        self.assertEqual( 2, expr.literal_num() )
        self.assertEqual( 1, expr.literal_num(var0) )
        self.assertEqual( 1, expr.literal_num(var0, False) )
        self.assertEqual( 0, expr.literal_num(var0, True) )
        self.assertEqual( 1, expr.literal_num(var1) )
        self.assertEqual( 0, expr.literal_num(var1, False) )
        self.assertEqual( 1, expr.literal_num(var1, True) )
        self.assertEqual( 2, expr.input_size )
        self.assertEqual( 4, expr.sop_literal_num() )

    def test_inv(self) :
        var0 = VarId(0)
        var1 = VarId(1)
        lit0p = Expr.make_posi_literal(var0)
        lit1n = Expr.make_nega_literal(var1)

        lit0n = ~lit0p
        self.assertEqual( '~V_0', str(lit0n) )

        lit1p = ~lit1n
        self.assertEqual( 'V_1', str(lit1p) )

        expr = lit0p & lit1n
        expr_n = ~expr;
        self.assertEqual( '( ~V_0 | V_1 )', str(expr_n) )

    def test_compose1(self) :
        var0 = VarId(0)
        var1 = VarId(1)
        var2 = VarId(2)
        var3 = VarId(3)
        lit0p = Expr.make_posi_literal(var0)
        lit1p = Expr.make_posi_literal(var1)
        lit1n = Expr.make_nega_literal(var1)
        lit2p = Expr.make_posi_literal(var2)
        lit3p = Expr.make_posi_literal(var3)

        expr0 = lit0p | (lit1n & lit2p)
        expr1 = lit1p & lit3p
        expr = expr0.compose(var0, expr1)

        self.assertEqual( '( ( V_1 & V_3 ) | ( ~V_1 & V_2 ) )', str(expr) )

    def test_compose2(self) :
        var0 = VarId(0)
        var1 = VarId(1)
        var2 = VarId(2)
        var3 = VarId(3)
        var4 = VarId(4)
        lit0p = Expr.make_posi_literal(var0)
        lit1p = Expr.make_posi_literal(var1)
        lit1n = Expr.make_nega_literal(var1)
        lit2p = Expr.make_posi_literal(var2)
        lit3p = Expr.make_posi_literal(var3)
        lit4n = Expr.make_nega_literal(var4)

        expr0 = lit0p | lit4n
        expr1 = lit1p & lit3p
        expr2 = ~(lit1n & lit2p)
        comp_map = dict()
        comp_map[var0] = expr1
        comp_map[var4] = expr2
        expr = expr0.compose_map(comp_map)

        self.assertEqual( '( ( V_1 & V_3 ) | ( ~V_1 & V_2 ) )', str(expr) )

    def test_remap_var1(self) :
        var0 = VarId(0)
        var1 = VarId(1)
        var2 = VarId(2)
        var3 = VarId(3)
        lit0p = Expr.make_posi_literal(var0)
        lit1p = Expr.make_posi_literal(var1)
        lit1n = Expr.make_nega_literal(var1)
        lit2p = Expr.make_posi_literal(var2)
        lit3p = Expr.make_posi_literal(var3)

        expr0 = (lit0p & lit3p) | (lit1n & lit2p)
        var_map = dict()
        var_map[var0] = var1
        expr = expr0.remap_var(var_map)

        self.assertEqual( '( ( V_1 & V_3 ) | ( ~V_1 & V_2 ) )', str(expr) )


if __name__ == '__main__' :

    unittest.main()
