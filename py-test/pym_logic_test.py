#! /usr/bin/env python3

### @file pym_logic_test.py
### @brief ym_logic のテストプログラム
### @author Yusuke Matsunaga (松永 裕介)
###
### Copyright (C) 2018 Yusuke Matsunaga
### All rights reserved.

import unittest
from varid_test import VarIdTest
from literal_test import LiteralTest
from expr_test import ExprTest

suite = unittest.TestSuite()
suite.addTest(VarIdTest())
suite.addTest(LiteralTest())
suite.addTest(ExprTest())
