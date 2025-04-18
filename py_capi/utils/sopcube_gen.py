#! /usr/bin/env python3

""" PySopCube を生成するスクリプト

:file: sopcube_gen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import PyObjGen
from mk_py_capi import IntArg, RawObjArg, TypedRawObjArg, TypedObjConvArg
from mk_py_capi import OptArg, KwdArg
from mk_py_capi import AndOp, AndIop, DivOp, DivIop


class SopCubeGen(PyObjGen):

    def __init__(self):
        super().__init__(classname='SopCube',
                         pyname='SopCube',
                         namespace='YM',
                         header_include_files=['ym/SopCube.h'],
                         source_include_files=['pym/PySopCube.h',
                                               'pym/PyLiteral.h',
                                               'pym/PyExpr.h',
                                               'pym/PyTvFunc.h',
                                               'pym/PyLong.h',
                                               'pym/PyString.h'])

        def new_body(writer):
            writer.gen_vardecl(typename='std::vector<Literal>',
                               varname='lit_list')
            with writer.gen_if_block('list_obj != nullptr'):
                with writer.gen_if_block('!PyList<Literal, PyLiteral>::FromPyObject(list_obj, lit_list)'):
                    writer.gen_type_error('"argument 2 should be a sequence of \'Literal\'"')
            writer.gen_auto_assign('self', 'type->tp_alloc(type, 0)')
            self.gen_obj_conv(writer, objname='self', varname='my_obj')
            writer.write_line('new (&my_obj->mVal) SopCube(ni, lit_list);')
            writer.gen_return_self()
        self.add_new(new_body,
                     arg_list=[IntArg(name='input_num',
                                      cvarname='ni'),
                               OptArg(),
                               KwdArg(),
                               RawObjArg(name='literal_list',
                                         cvarname='list_obj')])

        self.add_dealloc('default')

        def meth_copy(writer):
            writer.gen_return_pyobject('PySopCube', 'val')
        self.add_method('copy',
                        func_body=meth_copy,
                        doc_str='make a copy')

        def meth_is_valid(writer):
            writer.gen_return_py_bool('val.is_valid()')
        self.add_method('is_valid',
                        func_body=meth_is_valid,
                        doc_str='True if valid')

        def meth_is_invalid(writer):
            writer.gen_return_py_bool('val.is_invalid()')
        self.add_method('is_invalid',
                        func_body=meth_is_invalid,
                        doc_str='True if invalid')

        def meth_is_tautology(writer):
            writer.gen_return_py_bool('val.is_tautology()')
        self.add_method('is_tautology',
                        func_body=meth_is_tautology,
                        doc_str='True if tautology')

        def meth_get_pat(writer):
            writer.gen_auto_assign('pat', 'val.get_pat(pos)')
            writer.gen_vardecl(typename='const char*',
                               varname='ans',
                               initializer='nullptr')
            with writer.gen_if_block('pat == SopPat::_X'):
                writer.gen_assign('ans', '"-"')
            with writer.gen_elseif_block('pat == SopPat::_0'):
                writer.gen_assign('ans', '"0"')
            with writer.gen_elseif_block('pat == SopPat::_1'):
                writer.gen_assign('ans', '"1"')
            writer.gen_return_py_string('ans')
        self.add_method('get_pat',
                        func_body=meth_get_pat,
                        arg_list=[IntArg(name='pos',
                                         cvarname='pos')],
                        doc_str='get pat')

        def meth_check_literal(writer):
            writer.gen_return_py_bool('val.check_literal(lit)')
        self.add_method('check_literal',
                        func_body=meth_check_literal,
                        arg_list=[TypedObjConvArg(name='literal',
                                                  cvarname='lit',
                                                  cvartype='Literal',
                                                  cvardefault=None,
                                                  pyclassname='PyLiteral')],
                        doc_str='check if the specified literal is present')

        def meth_literal_list(writer):
            writer.gen_return_pyobject('PyList<Literal, PyLiteral>',
                                       'val.literal_list()')
        self.add_method('literal_list',
                        func_body=meth_literal_list,
                        doc_str='return a list of literals')

        def meth_check_containment(writer):
            self.gen_ref_conv(writer, objname='obj1', refname='val1')
            with writer.gen_if_block('val.variable_num() != val1.variable_num()'):
                writer.gen_value_error('"\'variable_num()\' mismatch"')
            writer.gen_return_py_bool('val.check_containment(val1)')
        self.add_method('check_containment',
                        func_body=meth_check_containment,
                        arg_list=[TypedRawObjArg(name='cube',
                                                 cvarname='obj1',
                                                 pytypename='PySopCube::_typeobject()')],
                        doc_str='check containment')

        def meth_expr(writer):
            writer.gen_return_pyobject('PyExpr', 'val.expr()')
        self.add_method('expr',
                        func_body=meth_expr,
                        doc_str='convert to \'Expr\'')

        def meth_tvfunc(writer):
            writer.gen_return_pyobject('PyTvFunc', 'val.tvfunc()')
        self.add_method('tvfunc',
                        func_body=meth_tvfunc,
                        doc_str='convert to \'TvFunc\'')

        def get_variable_num(writer):
            writer.gen_return_py_long('val.variable_num()')
        self.add_getter('get_variable_num',
                        func_body=get_variable_num)
        self.add_attr('variable_num',
                      getter_name='get_variable_num')

        def get_literal_num(writer):
            writer.gen_return_py_long('val.literal_num()')
        self.add_getter('get_literal_num',
                        func_body=get_literal_num)
        self.add_attr('literal_num',
                      getter_name='get_literal_num')

        self.add_richcompare('cmp_default')

        def nb_div(writer):
            with writer.gen_if_block('PySopCube::Check(self)'):
                writer.gen_autoref_assign('val1', 'PySopCube::_get_ref(self)')
                with writer.gen_if_block('PySopCube::Check(other)'):
                    writer.gen_autoref_assign('val2', 'PySopCube::_get_ref(other)')
                    writer.gen_return_pyobject('PySopCube', 'val1 / val2')
                with writer.gen_if_block('PyLiteral::Check(other)'):
                    writer.gen_autoref_assign('val2', 'PyLiteral::_get_ref(other)')
                    writer.gen_return_pyobject('PySopCube', 'val1 / val2')
            writer.gen_return_py_notimplemented()

        def nb_idiv(writer):
            with writer.gen_if_block('PySopCube::Check(self)'):
                writer.gen_autoref_assign('val1', 'PySopCube::_get_ref(self)')
                with writer.gen_if_block('PySopCube::Check(other)'):
                    writer.gen_autoref_assign('val2', 'PySopCube::_get_ref(other)')
                    writer.write_line('val1 /= val2;')
                    writer.gen_return_self(incref=True)
                with writer.gen_if_block('PyLiteral::Check(other)'):
                    writer.gen_autoref_assign('val2', 'PyLiteral::_get_ref(other)')
                    writer.write_line('val1 /= val2;')
                    writer.gen_return_self(incref=True)
            writer.gen_return_py_notimplemented()

        self.add_nb_and(op_list1=[AndOp('PyLiteral')],
                        op_list2=[AndOp('PyLiteral')])
        self.add_nb_inplace_and(op_list1=[AndIop('PyLiteral')])
        self.add_nb_true_divide(op_list1=[DivOp('PyLiteral')])
        self.add_nb_inplace_true_divide(op_list1=[DivIop('PyLiteral')])

        def hash_func(writer):
            writer.gen_return('val.hash()')
        self.add_hash(hash_func)

        self.add_conv('default')
        self.add_deconv('default')


if __name__ == '__main__':

    gen = SopCubeGen()

    gen.make_header()

    gen.make_source()
