#! /usr/bin/env python3

""" PySopCover を生成するスクリプト

:file: sopcover_gen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import PyObjGen
from mk_py_capi import IntArg, BoolArg, RawObjArg
from mk_py_capi import OptArg, KwdArg
from mk_py_capi import SubOp, SubIop
from mk_py_capi import AndOp, AndIop
from mk_py_capi import OrOp, OrIop
from mk_py_capi import Op, Iop


class SopCoverGen(PyObjGen):

    def __init__(self):
        super().__init__(classname='SopCover',
                         pyname='SopCover',
                         namespace='YM',
                         header_include_files=['ym/SopCover.h'],
                         source_include_files=['pym/PySopCover.h',
                                               'pym/PySopCube.h',
                                               'pym/PyLiteral.h',
                                               'pym/PyExpr.h',
                                               'pym/PyTvFunc.h',
                                               'pym/PyLong.h',
                                               'pym/PyString.h'])

        def new_body(writer):
            with writer.gen_if_block('list_obj != nullptr'):
                with writer.gen_block(comment='SopCube のリスト'):
                    writer.gen_vardecl(typename='std::vector<SopCube>',
                                       varname='cube_list')
                    with writer.gen_if_block('PyList<SopCube, PySopCube>::FromPyObject(list_obj, cube_list)'):
                        writer.gen_auto_assign('self', 'type->tp_alloc(type, 0)')
                        self.gen_obj_conv(writer, objname='self', varname='my_obj')
                        writer.write_line('new (&my_obj->mVal) SopCover(ni, cube_list);')
                        writer.gen_return_self()
                with writer.gen_block(comment='Literal のリストのリスト'):
                    writer.gen_vardecl(typename='std::vector<std::vector<Literal>>',
                                       varname='literal_list')
                    with writer.gen_if_block('PyList<std::vector<Literal>, PyList<Literal, PyLiteral>>::FromPyObject(list_obj, literal_list)'):
                        writer.gen_auto_assign('self', 'type->tp_alloc(type, 0)')
                        self.gen_obj_conv(writer, objname='self', varname='my_obj')
                        writer.write_line('new (&my_obj->mVal) SopCover(ni, literal_list);')
                        writer.gen_return_self()

                writer.gen_type_error('"argument 2 should be a sequence of \'SopCube\'"')
            with writer.gen_else_block():
                writer.gen_auto_assign('self', 'type->tp_alloc(type, 0)')
                self.gen_obj_conv(writer, objname='self', varname='my_obj')
                writer.write_line('new (&my_obj->mVal) SopCover(ni);')
                writer.gen_return_self()

        self.add_new(new_body,
                     arg_list=[IntArg(name='input_num',
                                      cvarname='ni'),
                               OptArg(),
                               KwdArg(),
                               RawObjArg(name='cube_list',
                                         cvarname='list_obj')])

        self.add_dealloc('default')

        def meth_copy(writer):
            writer.gen_return_pyobject('PySopCover', 'val')
        self.add_method('copy',
                        func_body=meth_copy,
                        doc_str='make a copy')

        def meth_literal_num(writer):
            with writer.gen_if_block('var_obj == nullptr'):
                with writer.gen_if_block('inv'):
                    writer.gen_type_error('"\'inv\' shoud not be specified without \'var\'"')
                writer.gen_return_py_long('val.literal_num()')
            with writer.gen_elseif_block('PyLiteral::Check(var_obj)'):
                writer.gen_auto_assign('lit', 'PyLiteral::_get_ref(var_obj)')
                with writer.gen_if_block('inv'):
                    writer.gen_assign('lit', '~lit')
                writer.gen_return_py_long('val.literal_num(lit)')
            with writer.gen_else_block():
                writer.gen_type_error('"\'var\' must be an integer or a \'Literal\'"')
        self.add_method('literal_num',
                        func_body=meth_literal_num,
                        arg_list=[OptArg(),
                                  RawObjArg(name='var',
                                            cvarname='var_obj'),
                                  KwdArg(),
                                  BoolArg(name='inv',
                                          cvarname='inv',
                                          cvardefault='false')],
                        doc_str='return number of literals')

        def meth_literal_list(writer):
            writer.gen_return_pyobject('PyList<std::vector<Literal>, PyList<Literal, PyLiteral>>', 'val.literal_list()')
        self.add_method('literal_list',
                        func_body=meth_literal_list,
                        doc_str='convert to list of list of literals')

        def meth_get_pat(writer):
            writer.gen_auto_assign('pat', 'val.get_pat(cube_pos, var_pos)')
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
                        arg_list=[IntArg(name='cube_pos',
                                         cvarname='cube_pos'),
                                  IntArg(name='var_pos',
                                         cvarname='var_pos')],
                        doc_str='get pat')

        def meth_common_cube(writer):
            writer.gen_return_pyobject('PySopCube', 'val.common_cube()')
        self.add_method('common_cube',
                        func_body=meth_common_cube,
                        doc_str='return common cube')

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

        def get_cube_num(writer):
            writer.gen_return_py_long('val.cube_num()')
        self.add_getter('get_cube_num',
                        func_body=get_cube_num)
        self.add_attr('cube_num',
                      getter_name='get_cube_num')

        self.add_richcompare('cmp_default')

        self.add_nb_or(op_list1=[OrOp('PySopCube')],
                       op_list2=[OrOp('PySopCube')])
        self.add_nb_inplace_or(op_list1=[OrIop('PySopCube')])
        self.add_nb_subtract(op_list1=[SubOp('PySopCube')])
        self.add_nb_inplace_subtract(op_list1=[SubIop('PySopCube')])
        self.add_nb_and(op_list1=[AndOp('PySopCube'),
                                  AndOp('PyLiteral')],
                        op_list2=[AndOp('PySopCube'),
                                  AndOp('PyLiteral')])
        self.add_nb_inplace_and(op_list1=[AndIop('PySopCube'),
                                          AndIop('PyLiteral')])
        self.add_nb_true_divide(expr='val1.algdiv(val2)',
                                op_list1=[Op('PySopCube',
                                             expr='val1.algdiv(val2)'),
                                          Op('PyLiteral',
                                             expr='val1.algdiv(val2)')])
        self.add_nb_inplace_true_divide(stmt='val1.algdiv_int(val2)',
                                        op_list1=[Iop('PySopCube',
                                                      stmt='val1.algdiv_int(val2)'),
                                                  Iop('PyLiteral',
                                                      stmt='val1.algdiv_int(val2)')])

        def hash_func(writer):
            writer.gen_return('val.hash()')
        self.add_hash(hash_func)

        self.add_conv('default')
        self.add_deconv('default')


if __name__ == '__main__':

    gen = SopCoverGen()

    gen.make_header()

    gen.make_source()
