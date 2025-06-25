#! /usr/bin/env python3

""" 関数の正規化用のテーブルを作るスクリプト

:file: mk_tbl.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

import itertools


# 関数のパタンの仕組み
#
# 入力0 のパタン 0xAAAA  1010101010101010
# 入力1 のパタン 0xCCCC  1100110011001100
# 入力2 のパタン 0xF0F0  1111000011110000
# 入力3 のパタン 0xFF00  1111111100000000

class Npn4:
    """４入力関数のNPN変換を表すクラス
    """

    perm_list = [order_list
                 for order_list in itertools.permutations([0, 1, 2, 3], 4)]

    def __init__(self, *,
                 oinv=False,
                 iinv_list=[False, False, False, False],
                 iperm=[0, 1, 2, 3]):
        self.__oinv = oinv
        self.__iinv_list = iinv_list[:]
        self.__iperm = iperm[:]

    def pack(self):
        """内容をパックする．
        """
        ans = Npn4.to_index(self.__iperm)
        if self.__oinv:
            ans |= (1 << 9)
        for i in range(4):
            if self.__iinv_list[i]:
                ans |= (1 << (i + 5))
        return ans

    def oinv(self):
        """出力の反転属性を返す．
        """
        return self.__oinv

    def iinv(self, pos):
        """入力の反転属性を返す．
        """
        if pos < 0 or 4 <= pos:
            raise ValueError
        return self.__iinv_list[pos]

    def iperm(self, pos):
        """入力の置換を返す．
        """
        if pos < 0 or 4 <= pos:
            raise ValueError
        return self.__iperm[pos]

    @staticmethod
    def to_index(order):
        """入力順をインデックスに変換する

        不正な順列の場合には ValueError 例外を送出する．
        """
        if len(order) != 4:
            raise ValueError
        for index, order1 in enumerate(perm_list):
            if order1 == order:
                return index
        raise ValueError


def inv0(func_pat):
    """入力0 の反転
    :param int func_pat: 関数のパタン
    """
    pat1 = func_pat & 0xAAAA
    pat0 = func_pat & 0x5555
    return (pat1 >> 1) | (pat0 << 1)

def inv1(func_pat):
    """入力1 の反転
    :param int func_pat: 関数のパタン
    """
    pat1 = func_pat & 0xCCCC
    pat0 = func_pat & 0x3333
    return (pat1 >> 2) | (pat0 << 2)

def inv2(func_pat):
    """入力2 の反転
    :param int func_pat: 関数のパタン
    """
    pat1 = func_pat & 0xF0F0
    pat0 = func_pat & 0x0F0F
    return (pat1 >> 4) | (pat0 << 4)

def inv3(func_pat):
    """入力3 の反転
    :param int func_pat: 関数のパタン
    """
    pat1 = func_pat & 0xFF00
    pat0 = func_pat & 0x00FF
    return (pat1 >> 8) | (pat0 << 8)

def xform(func_pat, inv_list, order_list):
    """入力順の変更
    :param int func_pat: 関数のパタン
    :param list[bool] inv_list: 極性のリスト
    :param list[int] order_list: 入力順のリスト

    入力 i を inv_list[i] に従って反転した後に order_list[i]
    の位置に入れ替える．
    """
    if inv_list[0]:
        func_pat = inv0(func_pat)
    if inv_list[1]:
        func_pat = inv1(func_pat)
    if inv_list[2]:
        func_pat = inv2(func_pat)
    if inv_list[3]:
        func_pat = inv3(func_pat)
    new_pat = 0
    for b in range(16):
        if func_pat & (1 << b):
            new_b = 0
            for i in range(4):
                mask = 1 << i
                if b & mask:
                    new_b |= 1 << order_list[i]
            new_pat |= (1 << new_b)
    return new_pat

def normalize(func_pat):
    max_pat = 0
    max_inv_list = [ 0 for _ in range(4) ]
    max_order_list = [ 0, 1, 2, 3 ]
    for b in range(16):
        inv_list = [ (b >> i) & 1 for i in range(4) ]
        for order_list in itertools.permutations([0, 1, 2, 3], 4):
            func_pat1 = xform(func_pat, inv_list, order_list)
            if max_pat < func_pat1:
                max_pat = func_pat1
                max_inv_list = inv_list
                max_order_list = order_list
            ifunc_pat = ~func_pat & 0xffff
            func_pat2 = xform(ifunc_pat, inv_list, order_list)
            if max_pat < func_pat2:
                max_pat = func_pat2
                max_inv_list = inv_list
                max_order_list = order_list
    return max_pat, max_inv_list, max_order_list

rep_pat_list = [ None for _ in range(0x10000) ]
rep_perm_list = [ [] for _ in range(0x10000) ]
rep_inv_list = [ [] for _ in range(0x10000) ]
rep_num = 0
for func_pat in range(0x10000):
    if rep_pat_list[func_pat] is not None:
        continue
    rep_num += 1
    print(f'{func_pat:04x}')
    for b in range(16):
        inv_list = [ (b >> i) & 1 for i in range(4) ]
        for order_list in itertools.permutations([0, 1, 2, 3], 4):
            func_pat1 = xform(func_pat, inv_list, order_list)
            if rep_pat_list[func_pat1] is None:
                rep_pat_list[func_pat1] = func_pat
                rep_perm_list[func_pat1] = order_list
                rep_inv_list[func_pat1] = inv_list + [0]
            ifunc_pat = ~func_pat & 0xffff
            func_pat2 = xform(ifunc_pat, inv_list, order_list)
            if rep_pat_list[func_pat2] is None:
                rep_pat_list[func_pat2] = func_pat
                rep_perm_list[func_pat2] = order_list
                rep_inv_list[func_pat2] = inv_list + [1]

print(f'Total {rep_num} functions')
