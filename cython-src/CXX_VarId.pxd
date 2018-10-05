
### @file CXX_VarId.pxd
### @brief CXX_VarId 用の pxd ファイル
### @author Yusuke Matsunaga (松永 裕介)
###
### Copyright (C) 2018 Yusuke Matsunaga
### All rights reserved.


cdef extern from "ym/VarId.h" namespace "nsYm" :

    cdef cppclass VarId :
        VarId()
        VarId(int)
        int val()
