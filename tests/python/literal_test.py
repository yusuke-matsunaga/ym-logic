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
        self.test_empty_constructor()
        self.test_simple_constructor()

    def test_empty_constructor(self) :
        lit = Literal()
        self.assertFalse( lit.is_valid() )
        self.assertEqual( str(lit), '-X-' )

    def test_simple_constructor(self) :
        var = VarId(3)
        lit = Literal(var)

        var1 = lit.varid
        self.assertEqual( var, var1 )

        self.assertTrue( lit.is_positive() )
        self.assertFalse( lit.is_negative() )

        lit_n = ~lit
        self.assertNotEqual( lit, lit_n )

        self.assertEqual( lit.varid, lit_n.varid )
        self.assertFalse( lit_n.is_positive() )
        self.assertTrue( lit_n.is_negative() )

        self.assertEqual( lit, lit.make_positive() )
        self.assertEqual( lit_n, lit.make_negative() )

        self.assertEqual( lit, lit_n.make_positive() )
        self.assertEqual( lit_n, lit.make_negative() )

        self.assertEqual( str(lit), 'V_3' )
        self.assertEqual( str(lit_n), "V_3'" )



if __name__ == '__main__' :

    unittest.main()
