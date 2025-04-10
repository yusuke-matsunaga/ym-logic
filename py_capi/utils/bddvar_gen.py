#! /usr/bin/env python3

""" PyBdd を生成するスクリプト

:file: tvfunc_gen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import PyObjGen
from mk_py_capi import TypedObjConvArg
from bdd_gen import BddArg


class BddVarArg(TypedObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='BddVar',
                         cvardefault=None,
                         pyclassname='PyBddVar')


class BddVarGen(PyObjGen):

    def __init__(self):
        super().__init__(classname='BddVar',
                         pyname='BddVar',
                         namespace='YM',
                         header_include_files=['ym/BddVar.h'],
                         source_include_files=['pym/PyBdd.h',
                                               'pym/PyBddVar.h',
                                               'pym/PyBddLit.h',
                                               'pym/PyLong.h'])

        self.add_new('disabled')
        
        self.add_dealloc('default')

        """
        def repr_func(writer):
            writer.gen_auto_assign('str_val', 'val.str()')
        self.add_repr(func_body=repr_func)
        """

        def meth_invalid(writer):
            writer.gen_return_pyobject('PyBdd', 'Bdd::invalid()')
        self.add_static_method('invalid',
                               func_body=meth_invalid,
                               doc_str='make invalid Bdd object')

        def meth_from_bdd(writer):
            writer.gen_return_pyobject('PyBddVar', 'BddVar::from_bdd(bdd)')
        self.add_static_method('from_bdd',
                               func_body=meth_from_bdd,
                               arg_list=[BddArg(name='bdd',
                                                cvarname='bdd')],
                               doc_str='convert from BDD')
        
        def meth_positive_literal(writer):
            writer.gen_return_pyobject('PyBddLit', 'val.positive_literal()')
        self.add_method('positive_literal',
                        func_body=meth_positive_literal,
                        doc_str='return positive literal of this variable')

        def meth_negative_literal(writer):
            writer.gen_return_pyobject('PyBddLit', 'val.negative_literal()')
        self.add_method('negative_literal',
                        func_body=meth_positive_literal,
                        doc_str='return negative literal of this variable')

        def get_id(writer):
            writer.gen_return_py_long('val.id()')
        self.add_getter('get_id',
                        func_body=get_id)
        self.add_attr('id',
                      getter_name='get_id')
            
        def hash_func(writer):
            writer.gen_return('val.hash()')
        self.add_hash(hash_func)

        self.add_richcompare('eq_default')
        
        def conv_func(writer):
            self.gen_alloc_code(writer, varname='obj')
            self.gen_obj_conv(writer, objname='obj', varname='my_obj')
            writer.write_line(f'new (&my_obj->mVal) {self.classname}(std::move(val));')
            writer.gen_return('obj')
        self.add_conv(conv_func)

        self.add_deconv('default')
            
            
if __name__ == '__main__':

    gen = BddVarGen()

    gen.make_header()

    gen.make_source()
