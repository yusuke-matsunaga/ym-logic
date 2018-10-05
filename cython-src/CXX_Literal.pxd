
### @file CXX_Literal.pxd
### @brief CXX_Literal 用の pxd ファイル
### @author Yusuke Matsunaga (松永 裕介)
###
### Copyright (C) 2018 Yusuke Matsunaga
### All rights reserved.

from libcpp cimport bool
from CXX_VarId cimport VarId


cdef extern from "ym/Literal.h" namespace "nsYm" :

    ### @brief Literal の cython バージョン
    cdef cppclass Literal :
        Literal()
        @staticmethod
        Literal index2literal(int)
        void set(VarId, bool)
        VarId varid()
        bool is_positive()
        bool is_negative()
        Literal invert()
        Literal make_positive()
        Literal make_negative()
        int index()
