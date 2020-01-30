
### @file CXX_Expr.pxd
### @brief CXX_Expr 用の pxd ファイル
### @author Yusuke Matsunaga (松永 裕介)
###
### Copyright (C) 2018 Yusuke Matsunaga
### All rights reserved.

from libcpp cimport bool
from libcpp.pair cimport pair
from libcpp.string cimport string
from libcpp.vector cimport vector
from CXX_VarId cimport VarId


cdef extern from "ym/Expr.h" namespace "nsYm::nsLogic" :

    ### @brief Expr の cython バージョン
    cdef cppclass Expr :
        Expr()
        @staticmethod
        Expr make_zero()
        @staticmethod
        Expr make_one()
        @staticmethod
        Expr make_literal(VarId, bool)
        @staticmethod
        Expr make_posi_literal(VarId)
        @staticmethod
        Expr make_nega_literal(VarId)
        @staticmethod
        Expr and_op(const Expr& opr1, const Expr& opr2)
        @staticmethod
        Expr make_and(const vector[Expr]& child_list)
        @staticmethod
        Expr or_op(const Expr& opr1, const Expr& opr2)
        @staticmethod
        Expr make_or(const vector[Expr]& child_list)
        @staticmethod
        Expr xor_op(const Expr& opr1, const Expr& opr2)
        @staticmethod
        Expr make_xor(const vector[Expr]& child_list)
        @staticmethod
        Expr from_string(const string&, string&)
        Expr invert()
        Expr compose(VarId, const Expr&)
        Expr compose(const vector[pair[VarId, Expr]]&)
        Expr remap_var(const vector[pair[VarId, VarId]]&)
        bool is_valid()
        bool is_zero()
        bool is_one()
        bool is_constant()
        bool is_posi_literal()
        bool is_nega_literal()
        bool is_literal()
        VarId varid()
        bool is_and()
        bool is_or()
        bool is_xor()
        bool is_op()
        int child_num()
        Expr child(int)
        bool is_simple()
        bool is_simple_and()
        bool is_simple_or()
        bool is_simple_xor()
        bool is_sop()

        int literal_num()
        int literal_num(VarId)
        int literal_num(VarId, bool)

        int input_size()
        int sop_cube_num()
        int sop_literal_num()
        int sop_literal_num(VarId)
        int sop_literal_num(VarId, bool)

    string to_string(const Expr&)
