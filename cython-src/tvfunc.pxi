
### @file tvfunc.pxi
### @brief tvfunc の cython インターフェイス
### @author Yusuke Matsunaga (松永 裕介)
###
### Copyright (C) 2018 Yusuke Matsunaga
### All rights reserved.

from CXX_TvFunc cimport TvFunc as CXX_TvFunc


cdef class TvFunc :

    # データメンバ
    cdef CXX_TvFunc _this

    ### @brief 初期化
    def __init__(self, int ni = 0, values = None) :
        cdef vector[int] c_values
        cdef int n
        if values :
            n = len(values)
            c_values.resize(n)
            for i in range(n) :
                c_values[i] = values[i]
            self._this = CXX_TvFunc(ni, c_values)
        else :
            self._this = CXX_TvFunc(ni)

    ### @brief 恒偽関数を返す．
    @staticmethod
    def zero(int ni) :
        ans = TvFunc()
        ans._this = CXX_TvFunc.zero(ni)
        return ans

    ### @brief 恒真関数を返す．
    @staticmethod
    def one(int ni) :
        ans = TvFunc()
        ans._this = CXX_TvFunc.one(ni)
        return ans

    ### @brief リテラル関数を返す．
    @staticmethod
    def literal(int ni, VarId var, bool inv) :
        cdef CXX_VarId c_var = CXX_VarId(var.val)
        ans = TvFunc()
        ans._this = CXX_TvFunc.literal(ni, c_var, inv)
        return ans

    ### @brief 肯定のリテラル関数を返す．
    @staticmethod
    def posi_literal(int ni, VarId var) :
        return TvFunc.literal(ni, var, False)

    ### @brief 否定のリテラル関数を返す．
    @staticmethod
    def nega_literal(int ni, VarId var) :
        return TvFunc.literal(ni, var, True)

    ### @brief 反転
    def __invert__(self) :
        ans = TvFunc()
        ans._this = self._this.invert()
        return ans

    ### @brief AND演算
    def __and__(TvFunc self, TvFunc other) :
        ans = TvFunc()
        ans._this = self._this.and_op(other._this)
        return ans

    ### @brief 代入付きAND演算
    def __iand__(TvFunc self, TvFunc other) :
        self._this.and_int(other._this)
        return self

    ### @brief OR演算
    def __or__(TvFunc self, TvFunc other) :
        ans = TvFunc()
        ans._this = self._this.or_op(other._this)
        return ans

    ### @brief 代入付きOR演算
    def __ior__(TvFunc self, TvFunc other) :
        self._this.or_int(other._this)
        return self

    ### @brief XOR演算
    def __xor__(TvFunc self, TvFunc other) :
        ans = TvFunc()
        ans._this = self._this.xor_op(other._this)
        return ans

    ### @brief 代入付きXOR演算
    def __ixor__(TvFunc self, TvFunc other) :
        self._this.xor_int(other._this)
        return self

    ### @brief 入力数を返す．
    @property
    def input_num(self) :
        return self._this.input_num()

    ### @brief 値を返す．
    def value(self, int pos) :
        return self._this.value(pos)

    ### @brief 0の数を数える．
    def count_zero(self) :
        return self._this.count_zero()

    ### @brief 1の数を数える．
    def count_one(self) :
        return self._this.count_one()

    ### @brief Walshの0次の係数を求める．
    def walsh_0(self) :
        return self._this.walsh_0()

    ### @brief Walshの1次の係数を求める．
    def walsh_1(self, VarId var) :
        return self._this.walsh_1(CXX_VarId(var.val))

    ### @brief Walshの1次の係数を求める．
    def walsh_1_list(self) :
        cdef int n = self._this.input_num()
        return [ self._this.walsh_1(CXX_VarId(i)) for i in range(n) ]

    ### @brief Walshの2次の係数を求める．
    def walsh_2(self, VarId var1, VarId var2) :
        return self._this.walsh_2(CXX_VarId(var1.val), CXX_VarId(var2.val))

    ### @brief Walshの2次の係数を求める．
    def walsh_2_list(self) :
        cdef int n = self._this.input_num()
        return [ [ self._this.walsh_2(CXX_VarId(i), CXX_VarId(j)) for j in range(n) ] for i in range(n) ]

    ### @brief 重み付きWalsh-0を求める．
    def walsh_w0(self, int w, bool oinv, int ibits) :
        return self._this.walsh_w0(w, oinv, ibits)

    ### @brief 重み付きWalsh-1を求める．
    def walsh_w1(self, VarId var, int w, bool oinv, int ibits) :
        return self._this.walsh_w1(CXX_VarId(var.val), w, oinv, ibits)

    ### @brief 重み付きWalsh-1を求める．
    def walsh_w1_list(self, VarId var, int w, bool oinv, int ibits) :
        cdef int n = self._this.input_num()
        return [ self._this.walsh_w1(CXX_VarId(i), w, oinv, ibits) for i in range(n) ]

    ### @brief サポートの検査を行う．
    def check_sup(self, VarId var) :
        return self._this.check_sup(CXX_VarId(var.val))

    ### @brief 対称性の検査を行う．
    def check_sym(self, VarId var1, VarId var2, bool inv) :
        return self._this.check_sym(CXX_VarId(var1.val), CXX_VarId(var2.val), inv)
