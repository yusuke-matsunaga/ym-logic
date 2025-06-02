#! /usr/bin/env python3

"""

:file: mk_canon_tbl.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from npn4 import Npn4


if __name__ == '__main__':

    for tv in range(0x10000):
        ctv, npn_tuple = Npn4.canon_tbl[tv]
        oinv, iinv_list, iperm_list = npn_tuple
        npn = Npn4(oinv=oinv,
                   iinv_list=iinv_list,
                   iperm=iperm_list)
        inpn = npn.inv()
        if tv % 4 == 0:
            print('  ', end='')
        else:
            print(' ', end='')
        print(f'0x{ctv:04x}, 0x{inpn.pack():03x}', end='')
        if tv < 0xFFFF:
            print(',', end='')
        if tv % 4 == 3:
            print()
