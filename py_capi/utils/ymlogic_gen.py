#! /usr/bin/env python3

""" ymlogic モジュールを生成するスクリプト

:file: ymlogic_gen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import ModuleGen


class YmlogicGen(ModuleGen):

    def __init__(self):
        super().__init__(modulename='ymlogic',
                         pyclass_list=['PyPrimType',
                                      ],
                         include_files=['pym/PyPrimType.h',
                                       ],
                         namespace='YM')

        
if __name__ == '__main__':

    gen = YmlogicGen()
    gen.make_header()
    gen.make_source()
