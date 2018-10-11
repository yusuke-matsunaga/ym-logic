
### @file varid.pxi
### @brief varid の cython インターフェイス
### @author Yusuke Matsunaga (松永 裕介)
###
### Copyright (C) 2018 Yusuke Matsunaga
### All rights reserved.

from libcpp cimport bool


### @brief VarId クラスの Python バージョン
###
### 実はC++のVarIdとは全くの無関係
cdef class VarId :

    # データメンバ
    cdef int _val

    ### @brief 不正値を返す．
    @staticmethod
    def illegal() :
        ans = VarId()
        ans._val = -1
        return ans

    ### @brief 初期化
    def __init__(self, int val = -1) :
        self._val = val

    ### @brief 正しい値の時に true を返す．
    def is_valid(self) :
        return self._val >= 0

    ### @brief 値を返す．
    @property
    def val(self) :
        return self._val

    ### @brief 等価比較演算子
    def __eq__(VarId self, VarId other) :
        return self._val == other._val

    ### @brief 非等価比較演算子
    def __ne__(VarId self, VarId other) :
        return self._val != other._val

    ### @brief < 比較演算子
    def __lt__(VarId self, VarId other) :
        return self._val < other._val

    ### @brief > 比較演算子
    def __gt__(VarId self, VarId other) :
        return self._val > other._val

    ### @brief <= 比較演算子
    def __le__(VarId self, VarId other) :
        return self._val <= other._val

    ### @brief >= 比較演算子
    def __ge__(VarId self, VarId other) :
        return self._val >= other._val

    ### @brief 内容を表す文字列を返す．
    def __repr__(VarId self) :
        if self.is_valid() :
            return 'V_{}'.format(self._val)
        else :
            return '---'

    ### @brief ハッシュ演算子
    def __hash__(VarId self) :
        return self._val
