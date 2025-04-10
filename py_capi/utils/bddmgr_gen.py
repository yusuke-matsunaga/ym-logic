#! /usr/bin/env python3

""" PyBddMgr を生成するスクリプト

:file: bddmgr_gen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import PyObjGen
from mk_py_capi import IntArg, StringArg, RawObjArg, OptArg, KwdArg
from bdd_gen import BddArg


class BddMgrGen(PyObjGen):

    def __init__(self):
        super().__init__(classname='BddMgr',
                         pyname='BddMgr',
                         namespace='YM',
                         header_include_files=['ym/BddMgr.h'],
                         source_include_files=['pym/PyBddMgr.h',
                                               'pym/PyBdd.h',
                                               'pym/PyLong.h',
                                               'pym/PyJsonValue.h',
                                               'pym/PyList.h',
                                               'ym/BddVar.h'])

        def new_body(writer):
            writer.gen_auto_assign('self', 'type->tp_alloc(type, 0)')
            self.gen_obj_conv(writer, objname='self', varname='my_obj')
            writer.write_line('new (&my_obj->mVal) BddMgr();')
            writer.gen_return_self()
        self.add_new(new_body)

        self.add_dealloc('default')

        def meth_variable(writer):
            writer.gen_return_pyobject('PyBdd', 'val.variable(varid)')
        self.add_method('variable',
                        func_body=meth_variable,
                        arg_list=[IntArg(name='varid',
                                         cvarname='varid')],
                        doc_str='make variable BDD')

        def meth_copy(writer):
            writer.gen_return_pyobject('PyBdd', 'val.copy(src)')
        self.add_method('copy',
                        func_body=meth_copy,
                        arg_list=[BddArg(name='src',
                                         cvarname='src')],
                        doc_str='make a copy of BDD')

        def meth_zero(writer):
            writer.gen_return_pyobject('PyBdd', 'val.zero()')
        self.add_method('zero',
                        func_body=meth_zero,
                        doc_str='make ZERO BDD')

        def meth_one(writer):
            writer.gen_return_pyobject('PyBdd', 'val.one()')
        self.add_method('one',
                        func_body=meth_one,
                        doc_str='make ONE BDD')

        def meth_from_truth(writer):
            with writer.gen_if_block('list_obj == nullptr'):
                with writer.gen_try_block():
                    writer.gen_return_pyobject('PyBdd', 'val.from_truth(func_str)')
                writer.gen_catch_invalid_argument('"invalid string"')
            with writer.gen_else_block():
                writer.gen_vardecl(typename='std::vector<Bdd>',
                                   varname='tmp_list')
                with writer.gen_if_block('!PyList<Bdd, PyBdd>::FromPyObject(list_obj, tmp_list)'):
                    writer.gen_type_error('"argument 2 should be a sequence of \'BddVar\'"')
                writer.gen_vardecl(typename='std::vector<BddVar>',
                                   varname='var_list')
                with writer.gen_if_block('!BddVar::from_bdd_list(tmp_list, var_list)'):
                    writer.gen_type_error('"argument 2 should be a sequence of \'BddVar\'"')
                with writer.gen_try_block():
                    writer.gen_return_pyobject('PyBdd', 'val.from_truth(func_str, var_list)')
                writer.gen_catch_invalid_argument('"invalid string"')
        self.add_method('from_truth',
                        func_body=meth_from_truth,
                        arg_list=[StringArg(name='func_str',
                                            cvarname='func_str'),
                                  OptArg(),
                                  KwdArg(),
                                  RawObjArg(name='var_list',
                                            cvarname='list_obj',
                                            cvardefault='nullptr')],
                        doc_str='make a BDD from truth table')

        def meth_enable_gc(writer):
            writer.write_line('val.enable_gc();')
            writer.gen_return_py_none()
        self.add_method('enable_gc',
                        func_body=meth_enable_gc,
                        doc_str='enable GC')

        def meth_disable_gc(writer):
            writer.write_line('val.disable_gc();')
            writer.gen_return_py_none()
        self.add_method('disable_gc',
                        func_body=meth_enable_gc,
                        doc_str='disable GC')

        def get_node_num(writer):
            writer.gen_return_py_long('val.node_num()')
        self.add_getter('get_node_num',
                        func_body=get_node_num)
        self.add_attr('node_num',
                      getter_name='get_node_num')

        def get_gc_limit(writer):
            writer.gen_return_py_long('val.gc_limit()')
        self.add_getter('get_gc_limit',
                        func_body=get_gc_limit)
        def set_gc_limit(writer):
            with writer.gen_if_block('!PyLong::Check(obj)'):
                writer.gen_type_error('"integer type is expected"',
                                      error_val='-1')
            writer.gen_auto_assign('gc_limit', 'PyLong::Get(obj)')
            writer.write_line('val.set_gc_limit(gc_limit);')
            writer.gen_return('0')
        self.add_setter('set_gc_limit',
                        func_body=set_gc_limit)
        self.add_attr('gc_limit',
                      getter_name='get_gc_limit',
                      setter_name='set_gc_limit')

        self.add_conv('default')

        self.add_deconv('default')
        

if __name__ == '__main__':

    gen = BddMgrGen()

    gen.make_header()

    gen.make_source()
