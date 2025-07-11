#! /usr/bin/env python3

""" ExprGen のクラス定義ファイル

:file: expr_gen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import PyObjGen, OptArg, KwdArg, RawObjArg, ObjConvArg
from mk_py_capi import IntArg, Uint64Arg, UlongArg
from mk_py_capi import BoolArg, StringArg


class ExprGen(PyObjGen):

    def __init__(self):
        super().__init__(classname='Expr',
                         pyname='Expr',
                         namespace='YM',
                         header_include_files=['ym/Expr.h',
                                               'ym/Literal.h'],
                         source_include_files=['pym/PyExpr.h',
                                               'pym/PyTvFunc.h',
                                               'pym/PyLiteral.h',
                                               'pym/PyPrimType.h',
                                               'pym/PyUint64.h',
                                               'pym/PyLong.h',
                                               'pym/PyUlong.h',
                                               'pym/PyString.h',
                                               'ym/TvFunc.h'])

        def new_func(writer):
            writer.gen_auto_assign('self', 'type->tp_alloc(type, 0)')
            self.gen_obj_conv(writer, objname='self', varname='my_obj')
            writer.gen_vardecl(typename='Expr',
                               varname='src')
            with writer.gen_if_block('expr_str != std::string{}'):
                with writer.gen_try_block():
                    writer.gen_assign('src', 'Expr::from_rep_string(expr_str)')
                writer.gen_catch_invalid_argument()
            writer.write_line('new (&my_obj->mVal) Expr(src);')
            writer.gen_return_self()
        self.add_new(func_body=new_func,
                     arg_list=[OptArg(),
                               StringArg(cvarname='expr_str')])

        self.add_dealloc()

        def repr_func(writer):
            writer.gen_return_py_string('val.rep_string()')
        self.add_repr(func_body=repr_func)

        def str_func(writer):
            writer.gen_return_py_string('val.to_string()')
        self.add_str(func_body=str_func)

        def meth_invalid(writer):
            writer.gen_return_pyobject('PyExpr', 'Expr::invalid()')
        self.add_method('invalid',
                        is_static=True,
                        func_body=meth_invalid,
                        doc_str='make an invalid object')

        def meth_zero(writer):
            writer.gen_return_pyobject('PyExpr', 'Expr::zero()')
        self.add_method('zero',
                        is_static=True,
                        func_body=meth_zero,
                        doc_str='make a ZERO expression')

        def meth_one(writer):
            writer.gen_return_pyobject('PyExpr', 'Expr::one()')
        self.add_method('one',
                        is_static=True,
                        func_body=meth_one,
                        doc_str='make a ONE expression')

        def meth_literal(writer):
            with writer.gen_if_block('inv'):
                writer.gen_assign('lit', '~lit')
            writer.gen_return_pyobject('PyExpr', 'Expr::literal(lit)')
        self.add_method('literal',
                        is_static=True,
                        func_body=meth_literal,
                        arg_list=[ObjConvArg(cvarname='lit',
                                             cvartype='Literal',
                                             cvardefault='Literal::x()',
                                             pyclassname='PyLiteral'),
                                  OptArg(),
                                  KwdArg(),
                                  BoolArg(name='inv',
                                          cvarname='inv',
                                          cvardefault='false')],
                        doc_str='make a literal object')

        def meth_posi_literal(writer):
            writer.gen_return_pyobject('PyExpr',
                                       'Expr::posi_literal(varid)')
        self.add_method('posi_literal',
                        is_static=True,
                        func_body=meth_posi_literal,
                        arg_list=[UlongArg(cvarname='varid')],
                        doc_str='make a positive literal object')

        def meth_nega_literal(writer):
            writer.gen_return_pyobject('PyExpr',
                                       'Expr::nega_literal(varid)')
        self.add_method('nega_literal',
                        is_static=True,
                        func_body=meth_nega_literal,
                        arg_list=[UlongArg(cvarname='varid')],
                        doc_str='make a negative literal object')

        def meth_and_op(writer):
            writer.gen_vardecl(typename='std::vector<Expr>',
                               varname='expr_list')
            with writer.gen_if_block('!PyList<Expr, PyExpr>::FromPyObject(obj, expr_list)'):
                writer.gen_type_error('"argument 1 must be a list of \'Expr\'s"')
            writer.gen_return_pyobject('PyExpr', 'Expr::and_op(expr_list)')
        self.add_method('and_op',
                        is_static=True,
                        func_body=meth_and_op,
                        arg_list=[RawObjArg(cvarname='obj')],
                        doc_str='AND op')

        def meth_or_op(writer):
            writer.gen_vardecl(typename='std::vector<Expr>',
                               varname='expr_list')
            with writer.gen_if_block('!PyList<Expr, PyExpr>::FromPyObject(obj, expr_list)'):
                writer.gen_type_error('"argument 1 must be a list of \'Expr\'s"')
            writer.gen_return_pyobject('PyExpr', 'Expr::or_op(expr_list)')
        self.add_method('or_op',
                        is_static=True,
                        func_body=meth_or_op,
                        arg_list=[RawObjArg(cvarname='obj')],
                        doc_str='OR op')

        def meth_xor_op(writer):
            writer.gen_vardecl(typename='std::vector<Expr>',
                               varname='expr_list')
            with writer.gen_if_block('!PyList<Expr, PyExpr>::FromPyObject(obj, expr_list)'):
                writer.gen_type_error('"argument 1 must be a list of \'Expr\'s"')
            writer.gen_return_pyobject('PyExpr', 'Expr::xor_op(expr_list)')
        self.add_method('xor_op',
                        is_static=True,
                        func_body=meth_xor_op,
                        arg_list=[RawObjArg(cvarname='obj')],
                        doc_str='XOR op')

        def meth_from_string(writer):
            with writer.gen_try_block():
                writer.gen_return_pyobject('PyExpr',
                                           'Expr::from_string(expr_str)')
            writer.gen_catch_invalid_argument('"syntax error in argument string"')
        self.add_method('from_string',
                        is_static=True,
                        func_body=meth_from_string,
                        arg_list=[StringArg(cvarname='expr_str')],
                        doc_str='make an Expr object from string')

        def meth_compose(writer):
            with writer.gen_if_block('!PyDict_Check(dict_obj)'):
                writer.gen_type_error('"argument 1 must be a dictonary object"')
            writer.gen_vardecl(typename='std::unordered_map<SizeType, Expr>',
                               varname='comp_map')
            writer.gen_vardecl(typename='PyObject*',
                               varname='key')
            writer.gen_vardecl(typename='PyObject*',
                               varname='value')
            writer.gen_vardecl(typename='Py_ssize_t',
                               varname='pos',
                               initializer='0')
            with writer.gen_while_block('PyDict_Next(dict_obj, &pos, &key, &value)'):
                writer.gen_auto_assign('id', 'PyLong::Get(key)')
                with writer.gen_if_block('id == -1 && PyErr_Occurred()'):
                    writer.gen_return('nullptr')
                with writer.gen_if_block('!PyExpr::Check(value)'):
                    writer.gen_type_error('"dictionary value must be an \'Expr\' type"')
                self.gen_ref_conv(writer, objname='value',
                                  refname='expr1')
                writer.write_line('comp_map.emplace(static_cast<SizeType>(id), expr1);')
            writer.gen_return_pyobject('PyExpr', 'val.compose(comp_map)')
        self.add_method('compose',
                        func_body=meth_compose,
                        arg_list=[RawObjArg(cvarname='dict_obj')],
                        doc_str='COMPOSE op')

        def meth_remap_var(writer):
            with writer.gen_if_block('!PyDict_Check(dict_obj)'):
                writer.gen_type_error('"argument 1 must be a dictonary object"')
            writer.gen_vardecl(typename='std::unordered_map<SizeType, SizeType>',
                               varname='comp_map')
            writer.gen_vardecl(typename='PyObject*',
                               varname='key')
            writer.gen_vardecl(typename='PyObject*',
                               varname='value')
            writer.gen_vardecl(typename='Py_ssize_t',
                               varname='pos',
                               initializer='0')
            with writer.gen_while_block('PyDict_Next(dict_obj, &pos, &key, &value)'):
                writer.gen_auto_assign('id', 'PyUlong::Get(key)')
                with writer.gen_if_block('id == -1 && PyErr_Occurred()'):
                    writer.gen_return('nullptr')
                writer.gen_auto_assign('val', 'PyLong::Get(value)')
                with writer.gen_if_block('val == -1 && PyErr_Occurred()'):
                    writer.gen_return('nullptr')
                writer.write_line('comp_map.emplace(static_cast<SizeType>(id), static_cast<SizeType>(val));')
            writer.gen_return_pyobject('PyExpr', 'val.remap_var(comp_map)')
        self.add_method('remap_var',
                        func_body=meth_remap_var,
                        arg_list=[RawObjArg(cvarname='dict_obj')],
                        doc_str='remap variable assignment')

        def meth_simplify(writer):
            writer.write_line('val.simplify();')
            writer.gen_return_py_none()
        self.add_method('simplify',
                        func_body=meth_simplify,
                        doc_str='simplify')

        def meth_eval(writer):
            writer.gen_vardecl(typename='std::vector<std::uint64_t>',
                               varname='vals')
            with writer.gen_if_block('!PyList<std::uint64_t, PyUint64>::FromPyObject(vect_obj, vals)'):
                writer.gen_type_error('"argument 1 must be a vector of integer"')

            writer.gen_auto_assign('ans', 'val.eval(vals, static_cast<Expr::BitVectType>(mask))')
            writer.gen_return_py_long('ans')
        self.add_method('eval',
                        func_body=meth_eval,
                        arg_list=[RawObjArg(cvarname='vect_obj'),
                                  OptArg(),
                                  KwdArg(),
                                  Uint64Arg(name='mask',
                                            cvarname='mask',
                                            cvardefault='~0UL')],
                        doc_str='evaluate input vector')

        def meth_tvfunc(writer):
            writer.gen_return_pyobject('PyTvFunc', 'val.tvfunc(ni)')
        self.add_method('tvfunc',
                        func_body=meth_tvfunc,
                        arg_list=[OptArg(),
                                  UlongArg(cvarname='ni',
                                           cvardefault='0')],
                        doc_str='convert to TvFunc')

        def meth_is_valid(writer):
            writer.gen_return_py_bool('val.is_valid()')
        self.add_method('is_valid',
                        func_body=meth_is_valid,
                        doc_str='check if valid')

        def meth_is_invalid(writer):
            writer.gen_return_py_bool('val.is_invalid()')
        self.add_method('is_invalid',
                        func_body=meth_is_invalid,
                        doc_str='check if invalid')

        def meth_is_zero(writer):
            writer.gen_return_py_bool('val.is_zero()')
        self.add_method('is_zero',
                        func_body=meth_is_zero,
                        doc_str='check if zero')

        def meth_is_one(writer):
            writer.gen_return_py_bool('val.is_one()')
        self.add_method('is_one',
                        func_body=meth_is_one,
                        doc_str='check if one')

        def meth_is_constant(writer):
            writer.gen_return_py_bool('val.is_constant()')
        self.add_method('is_constant',
                        func_body=meth_is_constant,
                        doc_str='check if constant')

        def meth_is_positive_literal(writer):
            writer.gen_return_py_bool('val.is_positive_literal()')
        self.add_method('is_positive_literal',
                        func_body=meth_is_positive_literal,
                        doc_str='check if positive literal')

        def meth_is_negative_literal(writer):
            writer.gen_return_py_bool('val.is_negative_literal()')
        self.add_method('is_negative_literal',
                        func_body=meth_is_negative_literal,
                        doc_str='check if negative Literal')

        def meth_is_literal(writer):
            writer.gen_return_py_bool('val.is_literal()')
        self.add_method('is_literal',
                        func_body=meth_is_literal,
                        doc_str='check if Literal type')

        def meth_is_and(writer):
            writer.gen_return_py_bool('val.is_and()')
        self.add_method('is_and',
                        func_body=meth_is_and,
                        doc_str='check if AND type')

        def meth_is_or(writer):
            writer.gen_return_py_bool('val.is_or()')
        self.add_method('is_or',
                        func_body=meth_is_or,
                        doc_str='check if OR type')

        def meth_is_xor(writer):
            writer.gen_return_py_bool('val.is_xor()')
        self.add_method('is_xor',
                        func_body=meth_is_xor,
                        doc_str='check if XOR type')

        def meth_is_op(writer):
            writer.gen_return_py_bool('val.is_op()')
        self.add_method('is_op',
                        func_body=meth_is_op,
                        doc_str='check if OP type')

        def meth_is_simple(writer):
            writer.gen_return_py_bool('val.is_simple()')
        self.add_method('is_simple',
                        func_body=meth_is_simple,
                        doc_str='check if simple type')

        def meth_is_simple_and(writer):
            writer.gen_return_py_bool('val.is_simple_and()')
        self.add_method('is_simple_and',
                        func_body=meth_is_simple_and,
                        doc_str='check if simple AND type')

        def meth_is_simple_or(writer):
            writer.gen_return_py_bool('val.is_simple_or()')
        self.add_method('is_simple_or',
                        func_body=meth_is_simple_or,
                        doc_str='check if simple OR type')

        def meth_is_simple_xor(writer):
            writer.gen_return_py_bool('val.is_simple_xor()')
        self.add_method('is_simple_xor',
                        func_body=meth_is_simple_xor,
                        doc_str='check if simple XOR type')

        def meth_is_sop(writer):
            writer.gen_return_py_bool('val.is_sop()')
        self.add_method('is_sop',
                        func_body=meth_is_sop,
                        doc_str='check if SOP type')

        def meth_analyze(writer):
            writer.gen_return_pyobject('PyPrimType', 'val.analyze()')
        self.add_method('analyze',
                        func_body=meth_analyze,
                        doc_str='analyze type of root node')

        def meth_get_varid(writer):
            writer.gen_return_py_long('val.varid()')
        self.add_method('get_varid',
                        func_body=meth_get_varid,
                        doc_str='get varid')

        def meth_get_literal(writer):
            writer.gen_return_pyobject('PyLiteral', 'val.literal()')
        self.add_method('get_literal',
                        func_body=meth_get_literal,
                        doc_str='get literal')

        def meth_get_operand_list(writer):
            writer.gen_return_pyobject('PyList<Expr, PyExpr>', 'val.operand_list()')
        self.add_method('get_operand_list',
                        func_body=meth_get_operand_list,
                        doc_str='get operand list')

        def meth_literal_num(writer):
            writer.gen_vardecl(typename='Literal',
                               varname='lit')
            writer.gen_vardecl(typename='SizeType',
                               varname='ans')
            with writer.gen_if_block('lit_obj == nullptr'):
                with writer.gen_if_block('inv'):
                    writer.gen_value_error('"\'inv\' should not be specified without \'var\'"')
                writer.gen_assign('ans', 'val.literal_num()')
            with writer.gen_elseif_block('PyLiteral::FromPyObject(lit_obj, lit)'):
                with writer.gen_if_block('inv'):
                    writer.gen_assign('lit', '~lit')
                writer.gen_assign('ans', 'val.literal_num(lit)')
            with writer.gen_else_block():
                writer.gen_type_error('"argument 1 must be an integer or a literal"')
            writer.gen_return_py_long('ans')
        self.add_method('literal_num',
                        func_body=meth_literal_num,
                        arg_list=[OptArg(),
                                  RawObjArg(cvarname='lit_obj'),
                                  KwdArg(),
                                  BoolArg(name='inv',
                                          cvarname='inv',
                                          cvardefault='false')],
                        doc_str='get literal num')

        def meth_sop_literal_num(writer):
            writer.gen_vardecl(typename='Literal',
                               varname='lit')
            writer.gen_vardecl(typename='SizeType',
                               varname='ans')
            with writer.gen_if_block('lit_obj == nullptr'):
                with writer.gen_if_block('inv'):
                    writer.gen_value_error('"\'inv\' should not be specified without \'var\'"')
                writer.gen_assign('ans', 'val.sop_literal_num()')
            with writer.gen_elseif_block('PyLiteral::FromPyObject(lit_obj, lit)'):
                with writer.gen_if_block('inv'):
                    writer.gen_assign('lit', '~lit')
                writer.gen_assign('ans', 'val.sop_literal_num(lit)')
            with writer.gen_else_block():
                writer.gen_type_error('"argument 1 must be an int or a literal"')
            writer.gen_return_py_long('ans')
        self.add_method('sop_literal_num',
                        func_body=meth_sop_literal_num,
                        arg_list=[OptArg(),
                                  RawObjArg(cvarname='lit_obj'),
                                  KwdArg(),
                                  BoolArg(name='inv',
                                          cvarname='inv',
                                          cvardefault='false')],
                        doc_str='get literal num in SOP form')

        def meth_sop_cube_num(writer):
            writer.gen_auto_assign('ans', 'val.sop_cube_num()')
            writer.gen_return_py_long('ans')
        self.add_method('sop_cube_num',
                        func_body=meth_sop_cube_num,
                        doc_str='get cube num in SOP form')

        def getter_input_size(writer):
            writer.gen_auto_assign('ans', 'val.input_size()')
            writer.gen_return_py_long('ans')
        self.add_getter('input_size',
                        func_body=getter_input_size)
        self.add_attr('input_size',
                      getter_name='input_size')

        self.add_nb_invert()
        self.add_nb_and()
        self.add_nb_or()
        self.add_nb_xor()
        self.add_nb_inplace_and()
        self.add_nb_inplace_or()
        self.add_nb_inplace_xor()

        self.add_richcompare('eq_default')

        self.add_conv('default')
        self.add_deconv('default')


if __name__ == '__main__':

    gen = ExprGen()

    gen.make_header()

    gen.make_source()
