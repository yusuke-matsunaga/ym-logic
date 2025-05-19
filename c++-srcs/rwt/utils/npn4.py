#! /usr/bin/env python3

""" Npn4 の定義ファイル

:file: npn4.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

import itertools


def perm2sig(perm):
    """入力順をシグネチャに変換する．
    """
    ans = 0
    for i in range(4):
        ans += perm[i] << (i * 2)
    return ans

def sig2perm(sig):
    """シグネチャを入力順に変換する．
    """
    return [ (sig >> (i * 2)) & 3 for i in range(4) ]

def inv(perm):
    ans = [ 0 for _ in range(4) ]
    for i in range(4):
        ans[perm[i]] = i
    return ans

def compose(perm1, perm2):
    return [ perm2[perm1[i]] for i in range(4) ]

def mk_perm_list():
    perm_list = [order_list
                 for order_list in itertools.permutations([0, 1, 2, 3], 4)]
    return perm_list

def mk_index_tbl():
    """入力順をインデックスに変換するテーブルを作る．
    """
    perm_list = mk_perm_list()
    index_tbl = [ 0xff for _ in range(256) ]
    for index, perm in enumerate(perm_list):
        sig = perm2sig(perm)
        index_tbl[sig] = index
    return index_tbl

def mk_inv_tbl():
    """逆演算用のテーブルを作る．
    """
    perm_list = mk_perm_list()
    perm_num = len(perm_list)
    index_tbl = mk_index_tbl()
    inv_tbl = [ 0 for i in range(perm_num) ]
    for index, perm in enumerate(perm_list):
        inv_perm = inv(perm)
        inv_sig = perm2sig(inv_perm)
        inv_index = index_tbl[inv_sig]
        assert inv_index < 24
        inv_tbl[index] = inv_index
    return inv_tbl

def mk_compose_tbl():
    """入力順の合成用のテーブルを作る．
    """
    perm_list = mk_perm_list()
    index_tbl = mk_index_tbl()
    compose_tbl = [ [ 0 for i in range(24) ] for j in range(24) ]
    for index1, perm1 in enumerate(perm_list):
        for index2, perm2 in enumerate(perm_list):
            perm = compose(perm1, perm2)
            sig = perm2sig(perm)
            index = index_tbl[sig]
            assert index < 24
            compose_tbl[index1][index2] = index
    return compose_tbl

def mk_xform_tbl():
    """位置の変換用のテーブルを作る．
    """
    perm_list = mk_perm_list()
    xform_tbl = []
    for perm in perm_list:
        xform_tbl1 = []
        for b in range(16):
            new_b = 0
            for i in range(4):
                mask = 1 << i
                if b & mask:
                    new_b |= 1 << perm[i]
            xform_tbl1.append(new_b)
        xform_tbl.append(xform_tbl1)
    return xform_tbl


class Npn4:
    """４入力関数のNPN変換を表すクラス
    """

    perm_list = mk_perm_list()
    index_tbl = mk_index_tbl()
    inv_tbl = mk_inv_tbl()
    compose_tbl = mk_compose_tbl()
    xform_tbl = mk_xform_tbl()

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
        sig = perm2sig(self.__iperm)
        ans = Npn4.index_tbl[sig]
        assert index < 24
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
