#! /usr/bin/env python3

""" 最も左にあるビット位置を返すテーブルを作るプログラム

:file: gen_soplbit_tbl.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from gen_16x16tbl import gen_16x16tbl

def lbit(pat):
    """
    pat を SopPat を表す8ビットの符号なし整数とみなして
    最も左にあるビット位置を返す．
    ただし，SopPat は _X が '11' なので 0 の位置を求める． 
    pat = 255 の場合は呼ばれることはないが 0 としておく．
    """
    for i in range(8):
        if (pat & (1 << (7 - i))) == 0:
            return f'{i:1}'
    else:
        return '0'

if __name__ == '__main__':
    gen_16x16tbl(lbit)
