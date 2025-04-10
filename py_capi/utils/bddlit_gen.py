#! /usr/bin/env python3

""" PyBddLit を生成するスクリプト

:file: bddlit_gen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import PyObjGen
from mk_py_capi import TypedObjConvArg
from bdd_gen import BddArg
from bddvar_gen import BddVarArg


class BddLitArg(TypedObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='BddLit',
                         cvardefault=None,
                         pyclassname='PyBddLit')


class BddLitGen(PyObjGen):

    def __init__(self):
        super().__init__(classname='BddLit',
                         pyname='BddLit',
                         namespace='YM',
                         header_include_files=['ym/BddLit.h'],
                         source_include_files=['pym/PyBdd.h',
                                               'pym/PyBddLit.h',
                                               'pym/PyBddVar.h'])

        self.add_new('disabled')
        
        self.add_dealloc('default')

        """
        def repr_func(writer):
            writer.gen_auto_assign('str_val', 'val.str()')
        self.add_repr(func_body=repr_func)
        """

        def meth_invalid(writer):
            writer.gen_return_pyobject('PyBddLit', 'BddLit::invalid()')
        self.add_static_method('invalid',
                               func_body=meth_invalid,
                               doc_str='make invalid BddLit object')

        def meth_from_bdd(writer):
            writer.gen_return_pyobject('PyBddLit', 'BddLit::from_bdd(bdd)')
        self.add_method('from_bdd',
                        func_body=meth_from_bdd,
                        arg_list=[BddArg(name='bdd',
                                         cvarname='bdd')],
                        doc_str='convert from BDD')
        
        def meth_positive_literal(writer):
            writer.gen_return_pyobject('PyBddLit', 'BddLit::positive_literal(var)')
        self.add_static_method('positive_literal',
                               func_body=meth_positive_literal,
                               arg_list=[BddVarArg(name='var',
                                                   cvarname='var')],
                               doc_str='make positive literal')

        def meth_negative_literal(writer):
            writer.gen_return_pyobject('PyBddLit', 'BddLit::negative_literal(var)')
        self.add_static_method('negative_literal',
                               func_body=meth_negative_literal,
                               arg_list=[BddVarArg(name='var',
                                                   cvarname='var')],
                               doc_str='make negative literal')

        def meth_make_positive(writer):
            writer.gen_return_pyobject('PyBddLit', 'val.make_positive()')
        self.add_method('make_positive',
                        func_body=meth_make_positive,
                        doc_str='return positive literal of same variable')

        def meth_make_negative(writer):
            writer.gen_return_pyobject('PyBddLit', 'val.make_negative()')
        self.add_method('make_negative',
                        func_body=meth_make_negative,
                        doc_str='return negative literal of same variable')

        def meth_is_positive(writer):
            writer.gen_return_py_bool('val.is_positive()')
        self.add_method('is_positive',
                        func_body=meth_is_positive,
                        doc_str='True if positive literal')

        def meth_is_negative(writer):
            writer.gen_return_py_bool('val.is_negative()')
        self.add_method('is_negative',
                        func_body=meth_is_negative,
                        doc_str='True if negative literal')

        def get_var(writer):
            writer.gen_return_pyobject('PyBddVar', 'val.var()')
        self.add_getter('get_var',
                        func_body=get_var)
        self.add_attr('var',
                      getter_name='get_var')
        
        self.add_richcompare('eq_default')

        def nb_invert(writer):
            writer.gen_return_pyobject('PyBddLit', '~val')

        self.add_number(nb_invert=nb_invert)

        def hash_func(writer):
            writer.gen_return('val.hash()')
        self.add_hash(hash_func)
        
        def conv_func(writer):
            self.gen_alloc_code(writer, varname='obj')
            self.gen_obj_conv(writer, objname='obj', varname='my_obj')
            writer.write_line('new (&my_obj->mVal) BddLit(val);')
            writer.gen_return('obj')
        self.add_conv(conv_func)

        self.add_deconv('default')
            
            
if __name__ == '__main__':

    gen = BddLitGen()

    gen.make_header()

    gen.make_source()
