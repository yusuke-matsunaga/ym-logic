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
                                       'PyLiteral',
                                       'PyNpnMap',
                                       'PyTvFunc',
                                       'PyExpr',
                                       'PyBdd',
                                       'PyBddMgr',
                                       'PySopCover',
                                       'PySopCube',
                                       'PyAigHandle',
                                       'PyAigMgr'
                                      ],
                         include_files=['pym/PyPrimType.h',
                                        'pym/PyLiteral.h',
                                        'pym/PyNpnMap.h',
                                        'pym/PyTvFunc.h',
                                        'pym/PyExpr.h',
                                        'pym/PyBdd.h',
                                        'pym/PyBddMgr.h',
                                        'pym/PySopCover.h',
                                        'pym/PySopCube.h',
                                        'pym/PyAigHandle.h',
                                        'pym/PyAigMgr.h'
                                       ],
                         namespace='YM')


if __name__ == '__main__':

    gen = YmlogicGen()
    gen.make_header()
    gen.make_source()
