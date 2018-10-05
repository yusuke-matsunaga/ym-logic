
### @file varid.pxi
### @brief varid の cython インターフェイス
### @author Yusuke Matsunaga (松永 裕介)
###
### Copyright (C) 2018 Yusuke Matsunaga
### All rights reserved.


### @brief VarId クラスの Python バージョン
cdef class VarId :

    # データメンバ
    cdef int _val

    ### @brief 初期化
    def __init__(self, int val = -1) :
        self._val = val

    ### @brief 値を返す．
    @property
    def val(self) :
        return self._val
