#! /usr/bin/env python3

""" インデックステーブルを作るスクリプト

:file: mk_index_tbl.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from npn4 import Npn4


if __name__ == '__main__':

    num_index = len(Npn4.index_tbl)
    for i, index in enumerate(Npn4.index_tbl):
        if i % 8 == 0:
            print('  ', end='')
        else:
            print(' ', end='')
        print(f'{index:3}', end='')
        if i < num_index - 1:
            print(',', end='')
        if i % 8 == 7:
            print()
