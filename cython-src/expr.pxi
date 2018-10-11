
### @file expr.pxi
### @brief expr の cython インターフェイス
### @author Yusuke Matsunaga (松永 裕介)
###
### Copyright (C) 2018 Yusuke Matsunaga
### All rights reserved.

from libcpp cimport bool
from libcpp.pair cimport pair
from libcpp.string cimport string
from libcpp.vector cimport vector
from CXX_VarId cimport VarId as CXX_VarId
from CXX_Literal cimport Literal as CXX_Literal
from CXX_Expr cimport Expr as CXX_Expr
from CXX_Expr cimport to_string

### @brief Expr クラスの Python バージョン
cdef class Expr :

    # データメンバ
    cdef CXX_Expr _this

    ### @brief 初期化
    def __init__(self) :
        pass

    ### @brief 定数０を作る．
    @staticmethod
    def zero() :
        ans = Expr()
        ans._this = CXX_Expr.zero()
        return ans

    ### @brief 定数1を作る．
    @staticmethod
    def one() :
        ans = Expr()
        ans._this = CXX_Expr.one()
        return ans

    ### @brief リテラルを作る．
    @staticmethod
    def literal(VarId varid, bool inv = False) :
        cdef CXX_VarId c_varid = CXX_VarId(varid.val)
        ans = Expr()
        ans._this = CXX_Expr.literal(c_varid, inv)
        return ans

    ### @brief 肯定のリテラルを作る．
    @staticmethod
    def posi_literal(VarId varid) :
        return Expr.literal(varid, False)

    ### @brief 否定のリテラルを作る．
    @staticmethod
    def nega_literal(VarId varid) :
        return Expr.literal(varid, True)

    ### @brief AND 演算子
    def __and__(Expr self, Expr other) :
        if type(other) != Expr :
            return NotImplemented
        ans = Expr()
        ans._this = CXX_Expr.and_op(self._this, other._this)
        return ans

    ### @brief OR 演算子
    def __or__(Expr self, Expr other) :
        if type(other) != Expr :
            return NotImplemented
        ans = Expr()
        ans._this = CXX_Expr.or_op(self._this, other._this)
        return ans

    ### @brief XOR 演算子
    def __xor__(Expr self, Expr other) :
        if type(other) != Expr :
            return NotImplemented
        ans = Expr()
        ans._this = CXX_Expr.xor_op(self._this, other._this)
        return ans

    ### @brief 否定演算子
    def __invert__(self) :
        ans = Expr()
        ans._this = self._this.invert()
        return ans

    ### @brief 複数オペランドの AND
    @staticmethod
    def make_and(expr_list) :
        cdef int n = len(expr_list)
        cdef vector[CXX_Expr] c_expr_list = vector[CXX_Expr](n)
        cdef Expr expr
        for i in range(n) :
            expr = expr_list[i]
            c_expr_list[i] = expr._this
        ans = Expr()
        ans._this = CXX_Expr.make_and(c_expr_list)
        return ans

    ### @brief 複数オペランドの OR
    @staticmethod
    def make_or(expr_list) :
        cdef int n = len(expr_list)
        cdef vector[CXX_Expr] c_expr_list = vector[CXX_Expr](n)
        cdef Expr expr
        for i in range(n) :
            expr = expr_list[i]
            c_expr_list[i] = expr._this
        ans = Expr()
        ans._this = CXX_Expr.make_or(c_expr_list)
        return ans

    ### @brief 複数オペランドの XOR
    @staticmethod
    def make_xor(expr_list) :
        cdef int n = len(expr_list)
        cdef vector[CXX_Expr] c_expr_list = vector[CXX_Expr](n)
        cdef Expr expr
        for i in range(n) :
            expr = expr_list[i]
            c_expr_list[i] = expr._this
        ans = Expr()
        ans._this = CXX_Expr.make_xor(c_expr_list)
        return ans

    ### @brief 文字列からの変換
    @staticmethod
    def from_string(str expr_str) :
        cdef string c_expr_str = expr_str.encode('UTF-8')
        cdef string c_err_msg
        cdef CXX_Expr c_expr = CXX_Expr.from_string(c_expr_str, c_err_msg)
        if c_expr.is_valid() :
            ans = Expr()
            ans._this = c_expr
            return ans
        else :
            return None

    ### @brief 変数を論理式に置き換える．
    def compose(self, VarId var, Expr sub_expr) :
        cdef CXX_VarId c_var = CXX_VarId(var.val)
        ans = Expr()
        ans._this = self._this.compose(c_var, sub_expr._this)
        return ans

    ### @brief 複数の変数を論理式に置き換える．
    def compose_map(self, comp_map) :
        cdef VarId var
        cdef Expr sub_expr
        cdef pair[CXX_VarId, CXX_Expr] c_pair
        cdef vector[pair[CXX_VarId, CXX_Expr]] c_comp_list
        for var, sub_expr in comp_map.items() :
            c_pair.first = CXX_VarId(var.val)
            c_pair.second = sub_expr._this
            c_comp_list.push_back(c_pair)
        ans = Expr()
        ans._this = self._this.compose(c_comp_list)
        return ans

    ### @brief 変数の置き換えを行う．
    def remap_var(self, var_map) :
        cdef VarId var1
        cdef VarId var2
        cdef pair[CXX_VarId, CXX_VarId] c_pair
        cdef vector[pair[CXX_VarId, CXX_VarId]] c_var_list
        for var1, var2 in var_map.items() :
            c_pair.first = CXX_VarId(var1.val)
            c_pair.second = CXX_VarId(var2.val)
            c_var_list.push_back(c_pair)
        ans = Expr()
        ans._this = self._this.remap_var(c_var_list)
        return ans

    ### @brief 定数０か調べる．
    def is_zero(self) :
        return self._this.is_zero()

    ### @brief 定数1か調べる．
    def is_one(self) :
        return self._this.is_one()

    ### @brief 定数か調べる．
    def is_constant(self) :
        return self._this.is_constant()

    ### @brief 肯定リテラルか調べる．
    def is_posi_literal(self) :
        return self._this.is_posi_literal()

    ### @brief 否定リテラルか調べる．
    def is_nega_literal(self) :
        return self._this.is_nega_literal()

    ### @brief リテラルか調べる．
    def is_literal(self) :
        return self._this.is_literal()

    ### @brief 変数番号を返す．
    @property
    def varid(self) :
        cdef CXX_VarId c_varid = self._this.varid()
        return VarId(c_varid.val())

    ### @brief 根本の演算がANDかどうか調べる．
    def is_and(self) :
        return self._this.is_and()

    ### @brief 根本の演算がORかどうか調べる．
    def is_or(self) :
        return self._this.is_or()

    ### @brief 根本の演算がXORかどうか調べる．
    def is_xor(self) :
        return self._this.is_xor()

    ### @brief 根本が演算かどうか調べる．
    def is_op(self) :
        return self._this.is_op()

    ### @brief 根本が演算の場合，子供の数を返す．
    @property
    def child_num(self) :
        if self.is_op() :
            return self._this.child_num()
        else :
            return 0

    ### @brief 根本が演算の場合，子供の論理式を返す．
    ### @param[in] pos ( 0 <= pos < child_num() )
    def child(self, int pos) :
        ans = Expr()
        if self.is_op() :
            ans._this = self._this.child(pos)
        return ans

    ### @brief 根本が演算の場合，子供の論理式のリストを返す．
    def child_list(self) :
        cdef int n = self._this.child_num()
        if self.is_op() :
            ans = [ self.child(i) for i in range(n) ]

    ### @brief 単純な式かどうか調べる．
    def is_simple(self) :
        return self._this.is_simple()

    ### @brief 単純なANDかどうか調べる．
    def is_simple_and(self) :
        return self._this.is_simple_and()

    ### @brief 単純なORかどうか調べる．
    def is_simple_or(self) :
        return self._this.is_simple_or()

    ### @brief 単純なXORかどうか調べる．
    def is_simple_xor(self) :
        return self._this.is_simple_xor()

    ### @brief SOP形式かどうか調べる．
    def is_sop(self) :
        return self._this.is_sop()

    ### @brief リテラル数を返す．
    @property
    def litnum(self) :
        return self._this.litnum()

    ### @brief 特定の変数に関するリテラル数を返す．
    def litnum_var(self, VarId var) :
        cdef CXX_VarId c_var = CXX_VarId(var.val)
        return self._this.litnum(c_var)

    ### @brief 特定のリテラルに関するリテラル数を返す．
    def litnum_literal(self, VarId var, bool inv) :
        cdef CXX_VarId c_var = CXX_VarId(var.val)
        return self._this.litnum(c_var, inv)

    ### @brief 特定のリテラルに関するリテラル数を返す．
    def litnum_literal(self, Literal lit) :
        cdef CXX_VarId c_var = CXX_VarId(lit.varid.val)
        cdef bool inv = lit.is_negative()
        return self._this.litnum(c_var, inv)

    ### @brief 入力数を返す．
    @property
    def input_size(self) :
        return self._this.input_size()

    ### @brief SOP形式に直した場合のキューブ数を返す．
    @property
    def sop_cubenum(self) :
        return self._this.sop_cubenum()

    ### @brief SOP形式に直した場合のリテラル数を返す．
    @property
    def sop_litnum(self) :
        return self._this.sop_litnum()

    ### @brief SOP形式に直した場合の特定の変数に関するリテラル数を返す．
    def sop_litnum_var(self, VarId var) :
        cdef CXX_VarId c_var = CXX_VarId(var.val)
        return self._this.sop_litnum(c_var)

    ### @brief SOP形式に直した場合の特定のリテラルに関するリテラル数を返す．
    def sop_litnum_literal(self, VarId var, bool inv) :
        cdef CXX_VarId c_var = CXX_VarId(var.val)
        return self._this.sop_litnum(c_var, inv)

    ### @brief SOP形式に直した場合の特定のリテラルに関するリテラル数を返す．
    def sop_litnum_literal(self, Literal lit) :
        cdef CXX_VarId c_var = CXX_VarId(lit.varid.val)
        cdef bool inv = lit.is_negative()
        return self._this.sop_litnum(c_var, inv)

    ### @brief 内容を表す文字列を返す．
    def __repr__(self) :
        cdef string c_str = to_string(self._this)
        return c_str.decode('UTF-8')
