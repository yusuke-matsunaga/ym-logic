
### @file CXX_TvFunc.pxd
### @brief CXX_TvFunc 用の pxd ファイル
### @author Yusuke Matsunaga (松永 裕介)
###
### Copyright (C) 2018 Yusuke Matsunaga
### All rights reserved.

from libcpp cimport bool
from libcpp.vector cimport vector
from CXX_VarId cimport VarId
from CXX_Literal cimport Literal


cdef extern from "ym/TvFunc.h" namespace "nsYm" :

    cdef cppclass TvFunc :
        TvFunc()
        TvFunc(int)
        TvFunc(int, const vector[int]& values)
        TvFunc(const TvFunc& src)
        @staticmethod
        TvFunc make_zero(int)
        @staticmethod
        TvFunc make_one(int)
        @staticmethod
        TvFunc make_literal(int, VarId, bool)
        TvFunc invert()
        TvFunc and_op(const TvFunc&)
        TvFunc or_op(const TvFunc&)
        TvFunc xor_op(const TvFunc&)
        TvFunc& and_int(const TvFunc&)
        TvFunc& or_int(const TvFunc&)
        TvFunc& xor_int(const TvFunc&)
        int input_num()
        int value(int)
        int count_zero()
        int count_one()
        int walsh_0()
        int walsh_1(VarId)
        int walsh_2(VarId, VarId)
        int walsh_01(int* vec)
        int walsh_012(int* vec1, int* vec2)
        int walsh_w0(int, bool, int)
        int walsh_w1(VarId, int, bool,int)
        bool check_sup(VarId)
        bool check_sym(VarId, VarId, bool)
