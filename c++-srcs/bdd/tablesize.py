#! /usr/bin/env python3

"""
2のべき乗数より小さい最大の素数を求めるプログラム

:file: tablesize.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2022 Yusuke Matsunaga, All rights reserved.
"""

import math

def is_prime(x):
    rx = math.floor(math.sqrt(x))
    for i in range(2, rx + 1):
        if x % i == 0:
            return False
    return True

for b in range(10, 21):
    n = 1 << b
    while not is_prime(n):
        n -= 1
    print(n)
