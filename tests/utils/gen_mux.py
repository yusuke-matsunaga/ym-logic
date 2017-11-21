#! /usr/bin/env python3
#
# @file gen_mux.py
# @brief MUX の関数を出力する．
# @author Yusuke Matsunaga (松永 裕介)
#
# Copyright (C) 2017 Yusuke Matsunaga
# All rights reserved.

import sys
import os
import argparse


### @brief MUX の論理関数を出力する．
### @param[in] n 選択信号のビット数
###
### データ入力数は 2^n となる．
def gen_mux(n) :

    # データ入力数
    n_data = 1 << n

    # 関数の入力数
    ni = n + n_data

    # 真理値表の要素数
    ni_exp = 1 << ni

    # 真理値表を作る．
    # 最初の n ビットが選択信号
    func_str = '{}:'.format(ni)
    for p in range(0, ni_exp) :
        d = p & ((1 << n) - 1)
        if p & (1 << (d + n)) :
            v = 1
        else :
            v = 0
        func_str += '{}'.format(v)

    return func_str


if __name__ == '__main__' :

    parser = argparse.ArgumentParser()
    parser.add_argument('size', type = int,
                        help = 'selecter input size')

    args = parser.parse_args()
    if not args :
        exit(-1)

    n = int(args.size)
    func_str = gen_mux(n)

    print('{}'.format(func_str))
