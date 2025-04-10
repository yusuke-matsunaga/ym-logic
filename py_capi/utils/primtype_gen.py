#! /usr/bin/env python3

""" PyPrimType を生成するスクリプト

:file: primtype_gen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import EnumGen, EnumInfo


class PrimTypeGen(EnumGen):

    def __init__(self):
        super().__init__(classname='PrimType',
                         pyname='PrimType',
                         namespace='YM',
                         enum_list=[EnumInfo('PrimType::C0', 'C0', 'C0'),
                                    EnumInfo('PrimType::C1', 'C1', 'C1'),
                                    EnumInfo('PrimType::Buff', 'Buff', 'Buff'),
                                    EnumInfo('PrimType::Not', 'Not', 'Not'),
                                    EnumInfo('PrimType::And', 'And', 'And'),
                                    EnumInfo('PrimType::Nand', 'Nand', 'Nand'),
                                    EnumInfo('PrimType::Or', 'Or', 'Or'),
                                    EnumInfo('PrimType::Nor', 'Nor', 'Nor'),
                                    EnumInfo('PrimType::Xor', 'Xor', 'Xor'),
                                    EnumInfo('PrimType::Xnor', 'Xnor', 'Xnor')],
                         none_value='PrimType::None',
                         ignore_case=True,
                         header_include_files=['ym/logic.h'],
                         source_include_files=['pym/PyPrimType.h',
                                               'pym/PyString.h'])
        def hash_body(writer):
            writer.gen_auto_assign('hash_val', 'val',
                                   casttype='Py_hash_t')
            writer.gen_return('hash_val')
        self.add_hash(func_body=hash_body)
        

if __name__ == '__main__':

    gen = PrimTypeGen()

    gen.make_header()
    gen.make_source()
