
### @file literal.pxi
### @brief literal の cython インターフェイス
### @author Yusuke Matsunaga (松永 裕介)
###
### Copyright (C) 2018 Yusuke Matsunaga
### All rights reserved.

from libcpp cimport bool
from CXX_VarId cimport VarId as CXX_VarId
from CXX_Literal cimport Literal as CXX_Literal


### @brief Literal クラスの Python バージョン
cdef class Literal :

    # データメンバ
    cdef CXX_Literal _this

    ### @brief 初期化
    def __init__(self, VarId var, bool inv = False) :
        cdef CXX_VarId c_var = CXX_VarId(var.val)
        self._this.set(c_var, inv)

    ### @brief 変数を返す．
    @property
    def varid(self) :
        cdef CXX_VarId c_var = self._this.varid()
        cdef int val = c_var.val()
        return VarId(val)

    ### @brief 肯定のリテラルか調べる．
    def is_positive(self) :
        return self._this.is_positive()

    ### @brief 否定のリテラルか調べる．
    def is_negative(self) :
        return self._this.is_negative()

    ### @brief 極性を反転したリテラルを返す．
    def __invert__(self) :
        ans = Literal()
        ans._this = self._this.invert()
        return ans

    ### @brief 変数が同じで極性が肯定のリテラルを返す．
    def make_positive(self) :
        ans = Literal()
        ans._this = self._this.make_positive()
        return ans

    ### @brief 変数が同じで極性が否定のリテラルを返す．
    def make_negative(self) :
        ans = Literal()
        ans._this = self._this.make_negative()
        return ans

    ### @brief インデックスを返す．
    @property
    def index(self) :
        return self._this.index()
