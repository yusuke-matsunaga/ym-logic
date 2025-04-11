#! /usr/bin/env python3

""" PyNpnMap を生成するスクリプト

:file: npnmap_gen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import PyObjGen
from mk_py_capi import IntArg, BoolArg, OptArg, KwdArg


class NpnMapGen(PyObjGen):

    def __init__(self):
        super().__init__(classname='NpnMap',
                         pyname='NpnMap',
                         namespace='YM',
                         header_include_files=['ym/NpnMap.h'],
                         source_include_files=['pym/PyNpnMap.h',
                                               'pym/PyInt.h'])

        self.add_new('default')
        
        self.add_dealloc('default')

        def init_func(writer):
            self.gen_ref_conv(writer,
                              objname='self',
                              refname='npnmap')
            with writer.gen_if_block('ni1 == -1'):
                writer.gen_assign('npnmap', 'NpnMap()')
            with writer.gen_elseif_block('ni2 == -1'):
                writer.gen_assign('npnmap', 'NpnMap(ni1)')
            with writer.gen_else_block():
                writer.gen_assign('npnmap', 'NpnMap(ni1, ni2)')
            writer.gen_return('0')
        self.add_init(func_body=init_func,
                      arg_list=[OptArg(),
                                IntArg(cvarname='ni1',
                                       cvardefault='-1'),
                                IntArg(cvarname='ni2',
                                       cvardefault='-1')])

        self.add_richcompare('eq_default')
        
        def meth_identity(writer):
            writer.gen_return_pyobject('PyNpnMap',
                                       'NpnMap::identity(ni, inv)')
        self.add_static_method('identity',
                               func_body=meth_identity,
                               arg_list=[IntArg(cvarname='ni',
                                                cvardefault='-1'),
                                         OptArg(),
                                         KwdArg(),
                                         BoolArg(name='inv',
                                                 cvarname='inv',
                                                 cvardefault='false')],
                               doc_str='make the identity map')

        def meth_clear(writer):
            writer.write_line('val.clear();')
            writer.gen_return_py_none()
        self.add_method('clear',
                        func_body=meth_clear,
                        doc_str='clear')

        def meth_resize(writer):
            with writer.gen_if_block('ni2 == -1'):
                writer.write_line('val.resize(ni1);')
            with writer.gen_else_block():
                writer.write_line('val.resize(ni1, ni2);')
            writer.gen_return_py_none()
        self.add_method('resize',
                        func_body=meth_resize,
                        arg_list=[IntArg(cvarname='ni1'),
                                  OptArg(),
                                  IntArg(cvarname='ni2',
                                         cvardefault='-1')],
                        doc_str='resize')

        def meth_set_identity(writer):
            writer.write_line('val.set_identity(ni, oinv);')
            writer.gen_return_py_none()
        self.add_method('set_identity',
                        func_body=meth_set_identity,
                        arg_list=[IntArg(cvarname='ni'),
                                  OptArg(),
                                  KwdArg(),
                                  BoolArg(name='oinv',
                                          cvarname='oinv',
                                          cvardefault='false')],
                                  doc_str='set itself to identity map')

        def meth_set_imap(writer):
            writer.write_line('val.set_imap(src_var, dst_var, inv);')
            writer.gen_return_py_none()
        self.add_method('set_imap',
                        func_body=meth_set_imap,
                        arg_list=[IntArg(name='src_var',
                                         cvarname='src_var'),
                                  IntArg(name='dst_var',
                                         cvarname='dst_var'),
                                  OptArg(),
                                  KwdArg(),
                                  BoolArg(name='inv',
                                          cvarname='inv',
                                          cvardefault='false')],
                        doc_str='set input mapping')

        def meth_set_oinv(writer):
            writer.write_line('val.set_oinv(inv);')
            writer.gen_return_py_none()
        self.add_method('set_oinv',
                        func_body=meth_set_oinv,
                        arg_list=[BoolArg(name='inv',
                                          cvarname='inv')],
                        doc_str='set output polatity')

        def meth_imap(writer):
            writer.gen_auto_assign('imap', 'val.imap(var)')
            writer.gen_auto_assign('dst_var', 'imap.var()')
            writer.gen_auto_assign('inv', 'imap.inv()')
            writer.gen_return_buildvalue('kp', ['dst_var', 'inv'])
        self.add_method('imap',
                        func_body=meth_imap,
                        arg_list=[IntArg(cvarname='var')],
                        doc_str='get input mapping')

        def get_input_num(writer):
            writer.gen_return_py_int('val.input_num()')
        self.add_getter('get_input_num',
                        func_body=get_input_num)
        self.add_attr('input_num',
                      getter_name='get_input_num',
                      doc_str='number of inputs')

        def get_input_num2(writer):
            writer.gen_return_py_int('val.input_num2()')
        self.add_getter('get_input_num2',
                        func_body=get_input_num2)
        self.add_attr('input_num2',
                      getter_name='get_input_num2',
                      doc_str='number of mapped inputs')

        def get_oinv(writer):
            writer.gen_return_py_bool('val.oinv()')
        self.add_getter('get_oinv',
                        func_body=get_oinv)
        self.add_attr('oinv',
                      getter_name='get_oinv',
                      doc_str='output polarity')

        def nb_invert(writer):
            writer.gen_return_pyobject('PyNpnMap', '~val')

        def nb_common(writer, body):
            with writer.gen_if_block('PyNpnMap::Check(self)'):
                writer.gen_autoref_assign('val1', 'PyNpnMap::_get_ref(self)')
                with writer.gen_if_block('PyNpnMap::Check(other)'):
                    writer.gen_autoref_assign('val2', 'PyNpnMap::_get_ref(other)')
                    body(writer)
            writer.gen_return_py_notimplemented()
            
        def nb_multiply(writer):
            def body(writer):
                writer.gen_return_pyobject('PyNpnMap', 'val1 * val2')
            nb_common(writer, body)

        def nb_inplace_multiply(writer):
            def body(writer):
                writer.write_line('val1 *= val2;')
                writer.gen_return_self(incref=True)
            nb_common(writer, body)

        self.add_number(nb_invert=nb_invert,
                        nb_multiply=nb_multiply,
                        nb_inplace_multiply=nb_inplace_multiply)
        
        self.add_conv('default')

        self.add_deconv('default')


if __name__ == '__main__':

    gen = NpnMapGen()

    gen.make_header()

    gen.make_source()
    
    
