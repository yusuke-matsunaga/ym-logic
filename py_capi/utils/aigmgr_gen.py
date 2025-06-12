#! /usr/bin/env python3

""" PyAigMgr を生成するスクリプト

:file: aigmgr_gen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import PyObjGen
from mk_py_capi import IntArg, BoolArg, StringArg
from mk_py_capi import RawObjArg
from mk_py_capi import ObjConvArg, TypedObjConvArg
from mk_py_capi import OptArg, KwdArg
from aighandle_gen import AigHandleArg


class ExprArg(TypedObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='Expr',
                         cvardefault=None,
                         pyclassname='PyExpr')


class AigMgrGen(PyObjGen):

    def __init__(self):
        super().__init__(classname='AigMgr',
                         pyname='AigMgr',
                         namespace='YM',
                         header_include_files=['ym/AigMgr.h'],
                         source_include_files=['pym/PyAigMgr.h',
                                               'pym/PyAigHandle.h',
                                               'pym/PyExpr.h',
                                               'pym/PyList.h',
                                               'pym/PyLong.h'])

        self.add_new('default')

        self.add_dealloc('default')

        def meth_input(writer):
            with writer.gen_try_block():
                writer.gen_return_pyobject('PyAigHandle',
                                           'val.input(input_id)')
            writer.gen_catch_invalid_argument()
        self.add_method('input',
                        func_body=meth_input,
                        arg_list=[IntArg(name='input_id',
                                         cvarname='input_id')],
                        doc_str='return specified INPUT')

        def meth_zero(writer):
            writer.gen_return_pyobject('PyAigHandle',
                                       'val.make_zero()')
        self.add_method('zero',
                        func_body=meth_zero,
                        doc_str='make ZERO')

        def meth_one(writer):
            writer.gen_return_pyobject('PyAigHandle',
                                       'val.make_one()')
        self.add_method('one',
                        func_body=meth_one,
                        doc_str='make ONE')

        def meth_and_op(writer):
            writer.gen_vardecl(typename='std::vector<AigHandle>',
                               varname='fanin_list')
            with writer.gen_if_block('!PyList<AigHandle, PyAigHandle>::FromPyObject(list_obj, fanin_list)'):
                writer.gen_type_error('"argument 1 should be a sequence of \'AigHandle\'"')
            writer.gen_return_pyobject('PyAigHandle',
                                       'val.and_op(fanin_list)')
        self.add_method('and_op',
                        func_body=meth_and_op,
                        arg_list=[RawObjArg(name='fanin_list',
                                            cvarname='list_obj')],
                        doc_str='AND op')

        def meth_or_op(writer):
            writer.gen_vardecl(typename='std::vector<AigHandle>',
                               varname='fanin_list')
            with writer.gen_if_block('!PyList<AigHandle, PyAigHandle>::FromPyObject(list_obj, fanin_list)'):
                writer.gen_type_error('"argument 1 should be a sequence of \'AigHandle\'"')
            writer.gen_return_pyobject('PyAigHandle',
                                       'val.or_op(fanin_list)')
        self.add_method('or_op',
                        func_body=meth_or_op,
                        arg_list=[RawObjArg(name='fanin_list',
                                            cvarname='list_obj')],
                        doc_str='OR op')

        def meth_xor_op(writer):
            writer.gen_vardecl(typename='std::vector<AigHandle>',
                               varname='fanin_list')
            with writer.gen_if_block('!PyList<AigHandle, PyAigHandle>::FromPyObject(list_obj, fanin_list)'):
                writer.gen_type_error('"argument 1 should be a sequence of \'AigHandle\'"')
            writer.gen_return_pyobject('PyAigHandle',
                                       'val.xor_op(fanin_list)')
        self.add_method('xor_op',
                        func_body=meth_and_op,
                        arg_list=[RawObjArg(name='fanin_list',
                                            cvarname='list_obj')],
                        doc_str='XOR op')

        def meth_from_expr(writer):
            writer.gen_return_pyobject('PyAigHandle',
                                       'val.from_expr(expr)')
        self.add_method('from_expr',
                        func_body=meth_from_expr,
                        arg_list=[ExprArg(name='expr',
                                          cvarname='expr')],
                        doc_str='make AIG from EXPR')

        def get_node_num(writer):
            writer.gen_return_py_long('val.node_num()')
        self.add_getter('get_node_num',
                        func_body=get_node_num)
        self.add_attr('node_num',
                      getter_name='get_node_num')

        def get_input_num(writer):
            writer.gen_return_py_long('val.input_num()')
        self.add_getter('get_input_num',
                        func_body=get_input_num)
        self.add_attr('input_num',
                      getter_name='get_input_num')

        def get_and_num(writer):
            writer.gen_return_py_long('val.and_num()')
        self.add_getter('get_and_num',
                        func_body=get_and_num)
        self.add_attr('and_num',
                      getter_name='get_and_num')

        #self.add_conv('default')

        #self.add_deconv('default')


if __name__ == '__main__':

    gen = AigMgrGen()

    gen.make_header()

    gen.make_source()
