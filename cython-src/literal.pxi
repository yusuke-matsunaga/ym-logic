
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
    def __init__(self, VarId var = VarId(), bool inv = False) :
        cdef CXX_VarId c_var = CXX_VarId(var.val)
        self._this.set(c_var, inv)

    ### @brief 適正な値を持っている時に true を返す．
    def is_valid(self) :
        return self._this.is_valid()

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
        return Literal(self.varid, self.is_positive())

    ### @brief 変数が同じで極性が肯定のリテラルを返す．
    def make_positive(self) :
        return Literal(self.varid, False)

    ### @brief 変数が同じで極性が否定のリテラルを返す．
    def make_negative(self) :
        return Literal(self.varid, True)

    ### @brief インデックスを返す．
    @property
    def index(self) :
        return self._this.index()

    ### @brief 等価比較演算子
    def __eq__(self, other) :
        return self.index == other.index

    ### @brief 非等価比較演算子
    def __ne__(self, other) :
        return self.index != other.index

    ### @brief < 比較演算子
    def __lt__(self, other) :
        return self.index < other.index

    ### @brief > 比較演算子
    def __gt__(self, other) :
        return self.index > other.index

    ### @brief <= 比較演算子
    def __le__(self, other) :
        return self.index <= other.index

    ### @brief >= 比較演算子
    def __ge__(self, other) :
        return self.index >= other.index

    ### @brief 内容を表す文字列を返す．
    def __repr__(self) :
        if self.is_valid() :
            s = str(self.varid)
            if self.is_negative() :
                s += "'"
        else :
            s = '-X-'
        return s
