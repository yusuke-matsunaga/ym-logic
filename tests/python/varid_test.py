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
        self.test_illegal_val()
        self.test_int_val()
        self.test_compare()

    def test_illegal_val(self) :
        var = VarId()
        self.assertFalse( var.is_valid() )
        self.assertEqual( var.val, -1 )
        self.assertEqual( str(var), '---' )

    def test_int_val(self) :
        val = 2
        var = VarId(val)
        self.assertTrue( var.is_valid() )
        self.assertEqual( var.val, val )
        self.assertEqual( str(var), 'V_2' )

    def test_compare(self) :
        val_list = [ 3, 5, 9 ]
        var_list = [ VarId(val) for val in val_list ]
        n = len(val_list)
        for i1 in range(n) :
            val1 = val_list[i1]
            var1 = var_list[i1]
            for i2 in range(n) :
                val2 = val_list[i2]
                var2 = var_list[i2]
                self.assertEqual( var1 == var2, val1 == val2 )
                self.assertEqual( var1 != var2, val1 != val2 )
                self.assertEqual( var1 <  var2, val1 <  val2 )
                self.assertEqual( var1 >  var2, val1 >  val2 )
                self.assertEqual( var1 <= var2, val1 <= val2 )
                self.assertEqual( var1 >= var2, val1 >= val2 )


if __name__ == '__main__' :

    unittest.main()
