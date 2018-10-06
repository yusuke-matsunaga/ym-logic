
### @file CXX_VarId.pxd
### @brief CXX_VarId 用の pxd ファイル
### @author Yusuke Matsunaga (松永 裕介)
###
### Copyright (C) 2018 Yusuke Matsunaga
### All rights reserved.

from libcpp cimport bool


cdef extern from "ym/VarId.h" namespace "nsYm" :

    cdef cppclass VarId :
        @staticmethod
        VarId illegal()
        VarId()
        VarId(int)
        bool is_valid()
        int val()

    int compare(const VarId& left, const VarId& right)
