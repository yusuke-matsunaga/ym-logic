#! /usr/bin/env python3

"""

:file: mk_xform_tbl.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from npn4 import Npn4


if __name__ == '__main__':

    num_index = len(Npn4.perm_list)
    for index in range(num_index):
        for b in range(16):
            if b == 0:
                print('  ', end='')
            else:
                print(' ', end='')
            print(f'{Npn4.xform_tbl[index][b]:2}', end='')
            if index < num_index - 1 or b < 15:
                print(',', end='')
        print()
