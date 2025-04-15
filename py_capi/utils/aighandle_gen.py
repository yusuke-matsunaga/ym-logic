#! /usr/bin/env python3

""" PyAigHandle を生成するスクリプト

:file: aighandle_gen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import PyObjGen
from mk_py_capi import IntArg, BoolArg, StringArg
from mk_py_capi import RawObjArg, ObjConvArg, TypedObjConvArg
from mk_py_capi import OptArg, KwdArg

        
class JsonValueArg(ObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvartype='JsonValue',
                         cvarname=cvarname,
                         cvardefault=None,
                         pyclassname='PyJsonValue')

        
class AigHandleArg(TypedObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='AigHandle',
                         cvardefault=None,
                         pyclassname='PyAigHandle')


class AigHandleGen(PyObjGen):

    def __init__(self):
        super().__init__(classname='AigHandle',
                         pyname='AigHandle',
                         namespace='YM',
                         header_include_files=['ym/AigHandle.h'],
                         source_include_files=['pym/PyAigHandle.h',
                                               'pym/PyLong.h',
                                               'pym/PyBool.h',
                                               'pym/PyList.h',
                                               'pym/PyJsonValue.h'])

        self.add_new('disabled')
        
        self.add_dealloc('default')

        """
        def repr_func(writer):
            writer.gen_auto_assign('str_val', 'val.str()')
        self.add_repr(func_body=repr_func)
        """

        def meth_inv(writer):
            writer.gen_return_py_bool('val.inv()')
        self.add_method('inv',
                        func_body=meth_inv,
                        doc_str='return INV flag')

        def meth_is_zero(writer):
            writer.gen_return_py_bool('val.is_zero()')
        self.add_method('is_zero',
                        func_body=meth_is_zero,
                        doc_str='True if ZERO')

        def meth_is_one(writer):
            writer.gen_return_py_bool('val.is_one()')
        self.add_method('is_one',
                        func_body=meth_is_one,
                        doc_str='True if ONE')

        def meth_is_const(writer):
            writer.gen_return_py_bool('val.is_const()')
        self.add_method('is_const',
                        func_body=meth_is_const,
                        doc_str='True if constant(ZERO/ONE)')

        def meth_is_input(writer):
            writer.gen_return_py_bool('val.is_input()')
        self.add_method('is_input',
                        func_body=meth_is_input,
                        doc_str='True if INPUT')

        def meth_input_id(writer):
            with writer.gen_try_block():
                writer.gen_return_py_long('val.input_id()')
            writer.gen_catch_invalid_argument()
        self.add_method('input_id',
                        func_body=meth_input_id,
                        doc_str='return INPUT ID')

        def meth_is_and(writer):
            writer.gen_return_py_bool('val.is_and()')
        self.add_method('is_and',
                        func_body=meth_is_and,
                        doc_str='True if AND')

        def meth_fanin(writer):
            with writer.gen_try_block():
                writer.gen_return_pyobject('PyAigHandle', 'val.fanin(pos)')
            writer.gen_catch_invalid_argument()
        self.add_method('fanin',
                        func_body=meth_fanin,
                        arg_list=[IntArg(name='pos',
                                         cvarname='pos')],
                        doc_str='return fanin handle')

        def meth_fanin0(writer):
            with writer.gen_try_block():
                writer.gen_return_pyobject('PyAigHandle', 'val.fanin0()')
            writer.gen_catch_invalid_argument()
        self.add_method('fanin0',
                        func_body=meth_fanin0,
                        doc_str='return fanin0 handle')

        def meth_fanin1(writer):
            with writer.gen_try_block():
                writer.gen_return_pyobject('PyAigHandle', 'val.fanin1()')
            writer.gen_catch_invalid_argument()
        self.add_method('fanin1',
                        func_body=meth_fanin1,
                        doc_str='return fanin1 handle')

        def meth_ex_fanin_list(writer):
            with writer.gen_try_block():
                writer.gen_return_pyobject('PyList<AigHandle, PyAigHandle>',
                                           'val.ex_fanin_list()')
            writer.gen_catch_invalid_argument()
        self.add_method('ex_fanin_list',
                        func_body=meth_ex_fanin_list,
                        doc_str='return EXTENDED fanin list')

        def meth_gen_dot(writer):
            writer.gen_vardecl(typename='std::ofstream',
                               varname='ofs(filename)')
            with writer.gen_if_block('!ofs'):
                writer.gen_vardecl(typename='std::ostringstream',
                                   varname='buf')
                writer.write_line('buf << "Could not create file \'" << filename << "\'";')
                writer.gen_value_error('buf.str().c_str()')
            writer.write_line('val.gen_dot(ofs, option);')
            writer.gen_return_py_none()
        self.add_method('gen_dot',
                        func_body=meth_gen_dot,
                        arg_list=[StringArg(name='filename',
                                            cvarname='filename'),
                                  OptArg(),
                                  KwdArg(),
                                  JsonValueArg(name='option',
                                               cvarname='option')],
                        doc_str='generate DOT file')

        def meth_gen_dot2(writer):
            writer.gen_vardecl(typename='std::ofstream',
                               varname='ofs(filename)')
            with writer.gen_if_block('!ofs'):
                writer.gen_vardecl(typename='std::ostringstream',
                                   varname='buf')
                writer.write_line('buf << "Could not create file \'" << filename << "\'";')
                writer.gen_value_error('buf.str().c_str()')
            writer.gen_vardecl(typename='std::vector<AigHandle>',
                               varname='aig_list')
            with writer.gen_if_block('!PyList<AigHandle, PyAigHandle>::FromPyObject(list_obj, aig_list)'):
                writer.gen_type_error('"argument 2 should be a sequence of \'AigHandle\'"')
            writer.write_line('AigHandle::gen_dot(ofs, aig_list, option);')
            writer.gen_return_py_none()
        self.add_static_method('gen_dot2',
                               func_body=meth_gen_dot2,
                               arg_list=[StringArg(name='filename',
                                                   cvarname='filename'),
                                         RawObjArg(name='aig_list',
                                                   cvarname='list_obj'),
                                         OptArg(),
                                         KwdArg(),
                                         JsonValueArg(name='option',
                                                      cvarname='option')],
                               doc_str='generate DOT file for AigHandles')

        def get_index(writer):
            writer.gen_return_py_long('val.index()')
        self.add_getter('get_index',
                        func_body=get_index)
        self.add_attr('index',
                      getter_name='get_index')

        self.add_richcompare('cmp_default')

        def nb_multiply(writer):
            with writer.gen_if_block('PyAigHandle::Check(self)'):
                writer.gen_autoref_assign('val1', 'PyAigHandle::_get_ref(self)')
                with writer.gen_if_block('PyBool_Check(other)'):
                    writer.gen_auto_assign('inv', 'PyBool::Get(other)')
                    writer.gen_return_pyobject('PyAigHandle', 'val1 * inv')
            writer.gen_return_py_notimplemented()
            
        self.add_nb_invert()
        self.add_nb_multiply()
        self.add_nb_and()
        self.add_nb_or()
        self.add_nb_xor()
        self.add_nb_inplace_and()
        self.add_nb_inplace_or()
        self.add_nb_inplace_xor()

        def hash_func(writer):
            writer.gen_return('val.hash()')
        self.add_hash(hash_func)
        
        def conv_func(writer):
            self.gen_alloc_code(writer, varname='obj')
            self.gen_obj_conv(writer, objname='obj', varname='my_obj')
            writer.write_line('new (&my_obj->mVal) AigHandle(val);')
            writer.gen_return('obj')
        self.add_conv(conv_func)

        self.add_deconv('default')
            
            
if __name__ == '__main__':

    gen = AigHandleGen()

    gen.make_header()

    gen.make_source()
