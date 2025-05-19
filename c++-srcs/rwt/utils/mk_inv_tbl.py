#! /usr/bin/env python3

"""

:file: mk_inv_tbl.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from npn4 import Npn4


if __name__ == '__main__':

    num_index = len(Npn4.perm_list)
    for index in range(num_index):
        inv_index = Npn4.inv_tbl[index]
        if index % 8 == 0:
            print('  ', end='')
        else:
            print(' ', end='')
        print(f'{inv_index:2}', end='')
        if index < num_index - 1:
            print(',', end='')
        if index % 8 == 7:
            print()
