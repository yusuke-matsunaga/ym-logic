#! /usr/bin/env python3

""" PyLiteral を生成するスクリプト

:file: literal_gen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import PyObjGen, IntArg, BoolArg


class LiteralGen(PyObjGen):

    def __init__(self):
        super().__init__(classname='Literal',
                         pyname='Literal',
                         namespace='YM',
                         header_include_files=['ym/Literal.h'],
                         source_include_files=['pym/PyLiteral.h',
                                               'pym/PyLong.h',
                                               'pym/PyString.h'])

        self.add_dealloc(func_body=None)
        
        def repr_func(writer):
            writer.gen_comment('val から文字列を作る．')
            writer.gen_vardecl(typename='std::ostringstream',
                               varname='buf')
            writer.write_line('buf << val')
            writer.gen_auto_assign('str_val', 'buf.str()')
        self.add_repr(func_body=repr_func)

        def hash_func(writer):
            writer.gen_return('val.hash()')
        self.add_hash(func_body=hash_func)

        def new_body(writer):
            self.gen_alloc_code(writer, varname='self')
            self.gen_obj_conv(writer, objname='self', varname='my_obj')
            with writer.gen_if_block('id == -1'):
                writer.gen_assign('my_obj->mVal', 'Literal::x()')
            with writer.gen_else_block():
                writer.gen_assign('my_obj->mVal', 'Literal(id, inv)')
            writer.gen_return('self')
        self.add_new(func_body=new_body,
                     arg_list=[IntArg(name=None,
                                      option=True,
                                      cvarname='id',
                                      cvardefault='-1'),
                               BoolArg(name='inv',
                                       option=True,
                                       cvarname='inv',
                                       cvardefault='false')])

        def set_method(writer):
            writer.write_line('val.set(id, inv);')
            writer.write_line('Py_RETURN_NONE;')
        self.add_method('set',
                        func_body=set_method,
                        arg_list=[IntArg(name=None,
                                         cvarname='id',
                                         cvardefault='-1'),
                                  BoolArg(name='inv',
                                          option=True,
                                          cvarname='inv',
                                          cvardefault='false')])

        def is_valid_method(writer):
            writer.gen_return_py_bool('val.is_valid()')
        self.add_method('is_valid',
                        func_body=is_valid_method)

        def is_positive_method(writer):
            writer.gen_return_py_bool('val.is_positive()')
        self.add_method('is_positive',
                        func_body=is_positive_method)

        def is_negative_method(writer):
            writer.gen_return_py_bool('val.is_negative()')
        self.add_method('is_negative',
                        func_body=is_negative_method)

        def make_positive_method(writer):
            writer.gen_auto_assign('ans', 'val.make_positive()')
            writer.gen_return('PyLiteral::ToPyObject(ans)')
        self.add_method('make_positive',
                        func_body=make_positive_method)

        def make_negative_method(writer):
            writer.gen_auto_assign('ans', 'val.make_negative()')
            writer.gen_return('PyLiteral::ToPyObject(ans)')
        self.add_method('make_negative',
                        func_body=make_negative_method)

        def varid_getter(writer):
            writer.gen_auto_assign('ans', 'val.varid()')
            writer.gen_return_py_long('ans')
        self.add_getter('varid_getter',
                        func_body=varid_getter)
        self.add_attr('varid',
                      getter_name='varid_getter')

        def index_getter(writer):
            writer.gen_auto_assign('ans', 'val.index()')
            writer.gen_return_py_long('ans')
        self.add_getter('index_getter',
                        func_body=index_getter)
        self.add_attr('index',
                      getter_name='index_getter')

        def richcompare(writer):
            with writer.gen_if_block('PyLiteral::Check(self) && PyLiteral::Check(other)'):
                self.gen_ref_conv(writer, objname='self', refname='val1')
                self.gen_ref_conv(writer, objname='other', refname='val2')
                writer.write_line('Py_RETURN_RICHCOMPARE(val1, val2, op);')
            writer.write_line('Py_RETURN_NOTIMPLEMENTED')
        self.add_richcompare(func_body=richcompare)

        def nb_invert(writer):
            writer.gen_return('PyLiteral::ToPyObject(~val)')
        self.add_number(nb_invert=nb_invert)
        
        self.add_conv('default')

        def deconv_body(writer):
            with writer.gen_if_block('PyLong::Check(obj)'):
                writer.gen_auto_assign('var', 'PyLong::Get(obj)',
                                       casttype='SizeType')
                writer.gen_assign('val', 'Literal(var)')
                writer.gen_return('true')
            self.gen_raw_conv(writer)
            writer.gen_return('false')
        self.add_deconv(deconv_body)


if __name__ == '__main__':

    gen = LiteralGen()

    gen.make_header()

    gen.make_source()
