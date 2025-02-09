#! /usr/bin/env python3

""" 最も左にあるビット位置を返すテーブルを作るプログラム

:file: gen_alglbit_tbl.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from gen_16x16tbl import gen_16x16tbl

def lbit(pat):
    """
    pat を AlgPat を表す8ビットの符号なし整数とみなして
    最も左にあるビット位置を返す．
    """
    for i in range(8):
        if (pat & (1 << (7 - i))) != 0:
            return f'{i:1}'
    else:
        return '0'

if __name__ == '__main__':
    gen_16x16tbl(lbit)
