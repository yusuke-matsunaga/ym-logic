#! /usr/bin/env python3

"""

:file: mk_compose_tbl.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from npn4 import Npn4


if __name__ == '__main__':

    perm_num = len(Npn4.perm_list)
    for index1 in range(perm_num):
        for index2 in range(perm_num):
            index = Npn4.compose_tbl[index1][index2]
            if index2 % 8 == 0:
                print('  ', end='')
            else:
                print(' ', end='')
            print(f'{index:2}', end='')
            if index1 < perm_num -1 or index2 < perm_num - 1:
                print(',', end='')
            if index2 % 8 == 7:
                print()
        print()
