#! /usr/bin/env python3

""" PyBddVarSet を生成するスクリプト

:file: bddvarset_gen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import PyObjGen
from mk_py_capi import RawObjArg, TypedRawObjArg, OptArg


class BddVarSetGen(PyObjGen):

    def __init__(self):
        super().__init__(classname='BddVarSet',
                         pyname='PyBddVarSet',
                         namespace='YM',
                         header_include_files=['ym/BddVarSet.h'],
                         source_include_files=['pym/PyBddVarSet.h'])

        def new_body(writer):
            writer.gen_auto_assign('self', 'type->tp_alloc(type, 0)')
            self.gen_obj_conv(writer, objname='self', varname='my_obj')
            with writer.gen_if_block('mgr_obj == nullptr'):
                writer.write_line('new (&my_obj->mVal) BddVarSet();')
            with writer.gen_else_block():
                writer.gen_autoref_assign('mgr', 'PyBddMgr::_get_ref(mgr_obj)')
                writer.gen_vardecl(typename='std::vector<Bddvar>',
                                   varname='var_set')
                with writer.gen_if_block('!PyList<BddVar, PyBddVar>::FromPyObject(list_obj, var_set)'):
                    writer.gen_type_error('"argument 2 should be a sequence of \'BddVar\'"')
                writer.write_line('new (&my_obj->mVal) BddVarSet(mgr, var_set);')
            writer.gen_return_self()
        self.add_new(new_body,
                     arg_list=[OptArg(),
                               TypedRawObjArg(name='mgr',
                                              cvarname='mgr_obj',
                                              pytypename='PyBddMgr::_typeobject()'),
                               RawObjArg(name='varset',
                                         cvarname='list_obj')])
        self.add_dealloc('default')

        self.add_number(nb_add='default',
                        nb_subtract='default',
                        nb_and='default',
                        nb_inplace_add='default',
                        nb_inplace_subtract='default',
                        nb_inplace_and='default')

        self.add_richcompare('eq_default')

        def meth_size(writer):
            writer.gen_return_py_long('val.size()')
        self.add_method('size',
                        func_body=meth_size,
                        doc_str='return size')

        def meth_to_varlist(writer):
            writer.gen_return_pyobject('PyList<BddVar, PyBddVar>',
                                       'val.to_varlist()')
        self.add_method('to_varlist',
                        func_body=meth_to_varlist,
                        doc_str='convert to list of \'BddVar\'s')

        def meth_top_var(writer):
            writer.gen_return_pyobject('PyBddVar', 'val.top_var()')
        self.add_method('top_var',
                        func_body=meth_top_var,
                        doc_str='return top variable')

        self.add_conv('default')

        self.add_deconv('default')
        

if __name__ == '__main__':

    gen = BddVarSetGen()

    gen.make_header()

    gen.make_source()
    
