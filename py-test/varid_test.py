#! /usr/bin/env python3

### @file varid_test.py
### @brief
### @author Yusuke Matsunaga (松永 裕介)
###
### Copyright (C) 2018 Yusuke Matsunaga
### All rights reserved.

import unittest
from pym_logic import VarId

# VarId のテスト用クラス
class VarIdTest(unittest.TestCase) :

    def runTest(self) :
        self.assertEqual( VarId(0).val, 0 )
        self.assertEqual( VarId(1).val, 1 )


if __name__ == '__main__' :

    unittest.main()
