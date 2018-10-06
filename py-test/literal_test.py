#! /usr/bin/env python3

### @file literal_test.py
### @brief
### @author Yusuke Matsunaga (松永 裕介)
###
### Copyright (C) 2018 Yusuke Matsunaga
### All rights reserved.

import unittest
from pym_logic import VarId
from pym_logic import Literal


# Literal のテスト用クラス
class LiteralTest(unittest.TestCase) :

    def runTest(self) :
        self.test_constructor()

    def test_constructor(self) :
        pass



if __name__ == '__main__' :

    unittest.main()
