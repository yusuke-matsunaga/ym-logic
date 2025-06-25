#! /usr/bin/env python3

"""

:file: mk_sup_tbl.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

def check_sup0(tv):
    """入力0がサポートかどうか調べる．
    """
    tv0 = tv & 0xAAAA
    tv1 = tv & 0x5555
    return (tv0 >> 1) != tv1

def check_sup1(tv):
    """入力1がサポートかどうか調べる．
    """
    tv0 = tv & 0xCCCC
    tv1 = tv & 0x3333
    return (tv0 >> 2) != tv1

def check_sup2(tv):
    """入力2がサポートかどうか調べる．
    """
    tv0 = tv & 0xF0F0
    tv1 = tv & 0x0F0F
    return (tv0 >> 4) != tv1

def check_sup3(tv):
    """入力3がサポートかどうか調べる．
    """
    tv0 = tv & 0xFF00
    tv1 = tv & 0x00FF
    return (tv0 >> 8) != tv1

def mk_sup_tbl():
    for tv in range(0x10000):
        bits = 0
        if check_sup0(tv):
            bits |= 1
        if check_sup1(tv):
            bits |= 2
        if check_sup2(tv):
            bits |= 4
        if check_sup3(tv):
            bits |= 8
        if tv % 16 == 0:
            print('  ', end='')
        else:
            print(' ', end='')
        print(f'0x{bits:1x}', end='')
        if tv < 0xFFFF:
            print(',', end='')
        if tv % 16 == 15:
            print()

if __name__ == '__main__':
    mk_sup_tbl()
