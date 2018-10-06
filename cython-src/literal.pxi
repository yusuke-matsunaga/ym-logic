
### @file literal.pxi
### @brief literal の cython インターフェイス
### @author Yusuke Matsunaga (松永 裕介)
###
### Copyright (C) 2018 Yusuke Matsunaga
### All rights reserved.

from libcpp cimport bool


### @brief Literal クラスの Python バージョン
###
### 実は C++ の Literal とは無関係なクラス
cdef class Literal :

    # データメンバ
    cdef int _val

    ### @brief 初期化
    def __init__(self, VarId var = VarId(), bool inv = False) :
        self._val = var.val + 1
        if inv :
            self._val = - self._val

    ### @brief 適正な値を持っている時に true を返す．
    def is_valid(self) :
        return self._val != 0

    ### @brief 変数を返す．
    @property
    def varid(self) :
        cdef int val = abs(self._val)
        return VarId(val - 1)

    ### @brief 肯定のリテラルか調べる．
    def is_positive(self) :
        return self._val > 0

    ### @brief 否定のリテラルか調べる．
    def is_negative(self) :
        return self._val < 0

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
        cdef int val = abs(self._val)
        val *= 2
        if self.is_negative() :
            val += 1
        return val

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
