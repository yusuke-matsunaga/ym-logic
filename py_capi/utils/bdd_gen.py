#! /usr/bin/env python3

""" PyBdd を生成するスクリプト

:file: tvfunc_gen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import PyObjGen, OptArg, KwdArg
from mk_py_capi import RawObjArg, TypedRawObjArg
from mk_py_capi import IntArg, UintArg, LongArg, UlongArg
from mk_py_capi import BoolArg, StringArg, TypedObjConvArg


class BddArg(TypedObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='Bdd',
                         cvardefault=None,
                         pyclassname='PyBdd')


class BddGen(PyObjGen):

    def __init__(self):
        super().__init__(classname='Bdd',
                         pyname='Bdd',
                         namespace='YM',
                         header_include_files=['ym/Bdd.h'],
                         source_include_files=['pym/PyBdd.h',
                                               'pym/PyLiteral.h',
                                               'pym/PyNpnMap.h',
                                               'pym/PyInt.h',
                                               'pym/PyLong.h',
                                               'pym/PyString.h',
                                               'pym/PyModule.h',
                                               'ym/BddVar.h',
                                               'ym/BddLit.h'])

        def new_body(writer):
            writer.gen_auto_assign('self', 'type->tp_alloc(type, 0)')
            self.gen_obj_conv(writer, objname='self', varname='my_obj')
            writer.write_line('new (&my_obj->mVal) Bdd();')
            writer.gen_return_self()
        self.add_new(new_body)
        
        self.add_dealloc('default')

        """
        def repr_func(writer):
            writer.gen_auto_assign('str_val', 'val.str()')
        self.add_repr(func_body=repr_func)
        """

        def meth_invalid(writer):
            writer.gen_return_pyobject('PyBdd', 'Bdd::invalid()')
        self.add_static_method('invalid',
                               func_body=meth_invalid,
                               doc_str='make invalid Bdd object')

        def meth_cofactor(writer):
            with writer.gen_if_block('inv'):
                writer.gen_assign('val2', '~val2')
            with writer.gen_try_block():
                writer.gen_return_pyobject('PyBdd',
                                           'val.cofactor(val2)')
            writer.gen_catch_invalid_argument('"argument 1 must be a cube-type BDD"')
        self.add_method('cofactor',
                        func_body=meth_cofactor,
                        arg_list=[BddArg(cvarname='val2'),
                                  OptArg(),
                                  KwdArg(),
                                  BoolArg(name='inv',
                                          cvarname='inv',
                                          cvardefault='false')],
                        doc_str='make cofactor')

        def meth_ite(writer):
            with writer.gen_try_block():
                writer.gen_return_pyobject('PyBdd',
                                           'Bdd::ite(val1, val2, val3)')
            writer.gen_catch_invalid_argument()
        self.add_static_method('ite',
                               func_body=meth_ite,
                               arg_list=[BddArg(name='cond_bdd',
                                                cvarname='val1'),
                                         BddArg(name='then_bdd',
                                                cvarname='val2'),
                                         BddArg(name='else_bdd',
                                                cvarname='val3')],
                               doc_str='ITE op')

        def meth_compose(writer):
            writer.gen_auto_assign('var', 'BddVar::from_bdd(var_bdd)')
            with writer.gen_if_block('var.is_invalid()'):
                writer.gen_type_error('"argument 1 must be a variable"')
            with writer.gen_try_block():
                writer.gen_return_pyobject('PyBdd',
                                           'val.compose(var, operand)')
            writer.gen_catch_invalid_argument()
        self.add_method('compose',
                        func_body=meth_compose,
                        arg_list=[BddArg(name='var',
                                         cvarname='var_bdd'),
                                  BddArg(name='operand',
                                         cvarname='operand')],
                        doc_str='COMPOSE op')

        def meth_multi_compose(writer):
            pass
        self.add_method('multi_compose',
                        func_body=meth_multi_compose,
                        arg_list=[TypedRawObjArg(name='map',
                                                 cvarname='dict_obj',
                                                 pytypename='&PyDict_Type')],
                        doc_str='multiway COMPOSE op')

        def meth_remap_vars(writer):
            pass
        self.add_method('remap_vars',
                        func_body=meth_remap_vars,
                        arg_list=[TypedRawObjArg(name='map',
                                                 cvarname='dict_obj',
                                                 pytypename='&PyDict_Type')],
                        doc_str='remap variable ordering')

        def meth_is_valid(writer):
            writer.gen_return_py_bool('val.is_valid()')
        self.add_method('is_valid',
                        func_body=meth_is_valid,
                        doc_str='return True if valid')

        def meth_is_invalid(writer):
            writer.gen_return_py_bool('val.is_invalid()')
        self.add_method('is_invalid',
                        func_body=meth_is_invalid,
                        doc_str='return True if invalid')

        def meth_is_zero(writer):
            writer.gen_return_py_bool('val.is_zero()')
        self.add_method('is_zero',
                        func_body=meth_is_zero,
                        doc_str='return True if ZERO')

        def meth_is_one(writer):
            writer.gen_return_py_bool('val.is_one()')
        self.add_method('is_one',
                        func_body=meth_is_one,
                        doc_str='return True if ONE')

        def meth_is_cube(writer):
            writer.gen_return_py_bool('val.is_cube()')
        self.add_method('is_cube',
                        func_body=meth_is_cube,
                        doc_str='return True if CUBE')

        def meth_is_posicube(writer):
            writer.gen_return_py_bool('val.is_posicube()')
        self.add_method('is_posicube',
                        func_body=meth_is_posicube,
                        doc_str='return True if positive CUBE')

        def meth_check_sup(writer):
            writer.gen_auto_assign('var', 'BddVar::from_bdd(var_bdd)')
            with writer.gen_if_block('var.is_invalid()'):
                writer.gen_type_error('"argument 1 must be a variable"')
            with writer.gen_try_block():
                writer.gen_return_py_bool('val.check_sup(var)')
            writer.gen_catch_invalid_argument()
        self.add_method('check_sup',
                        func_body=meth_check_sup,
                        arg_list=[BddArg(name='var',
                                         cvarname='var_bdd')],
                        doc_str='check support variable')

        def meth_check_sym(writer):
            writer.gen_auto_assign('var1', 'BddVar::from_bdd(var1_bdd)')
            with writer.gen_if_block('var1.is_invalid()'):
                writer.gen_type_error('"argument 1 must be a variable"')
            writer.gen_auto_assign('var2', 'BddVar::from_bdd(var2_bdd)')
            with writer.gen_if_block('var2.is_invalid()'):
                writer.gen_type_error('"argument 2 must be a variable"')
            with writer.gen_try_block():
                writer.gen_return_py_bool('val.check_sym(var1, var2, inv)')
            writer.gen_catch_invalid_argument()
        self.add_method('check_sym',
                        func_body=meth_check_sym,
                        arg_list=[BddArg(name='var1',
                                         cvarname='var1_bdd'),
                                  BddArg(name='var2',
                                         cvarname='var2_bdd'),
                                  OptArg(),
                                  KwdArg(),
                                  BoolArg(name='inv',
                                          cvarname='inv',
                                          cvardefault='false')],
                        doc_str='check symmmetry')

        def meth_get_support(writer):
            with writer.gen_try_block():
                writer.gen_return_pyobject('PyBddVarSet',
                                           'val.get_support()')
            writer.gen_catch_invalid_argument()
        self.add_method('get_support',
                        func_body=meth_get_support,
                        doc_str='get support variable')

        def meth_get_onepath(writer):
            with writer.gen_try_block():
                writer.gen_return_pyobject('PyBdd',
                                           'val.get_onepath()')
            writer.gen_catch_invalid_argument()
        self.add_method('get_onepath',
                        func_body=meth_get_onepath,
                        doc_str='get ONE path')

        def meth_get_zeropath(writer):
            with writer.gen_try_block():
                writer.gen_return_pyobject('PyBdd',
                                           'val.get_zeropath()')
            writer.gen_catch_invalid_argument()
        self.add_method('get_zeropath',
                        func_body=meth_get_onepath,
                        doc_str='get ZERO path')

        def meth_root_decomp(writer):
            with writer.gen_try_block():
                writer.gen_vardecl(typename='Bdd',
                                   varname='f0')
                writer.gen_vardecl(typename='Bdd',
                                   varname='f1')
                writer.gen_auto_assign('var',
                                       'val.root_decomp(f0, f1)')
                writer.gen_auto_assign('var_obj', 'PyBdd::ToPyObject(var)')
                writer.gen_auto_assign('f0_obj', 'PyBdd::ToPyObject(f0)')
                writer.gen_auto_assign('f1_obj', 'PyBdd::ToPyObject(f1)')
                writer.gen_return_buildvalue('OOO', ['var_obj', 'f0_obj', 'f1_obj'])
            writer.gen_catch_invalid_argument()
        self.add_method('root_decomp',
                        func_body=meth_root_decomp,
                        doc_str='decompose root node')

        def meth_root_var(writer):
            with writer.gen_try_block():
                writer.gen_return_pyobject('PyBdd', 'val.root_var()')
            writer.gen_catch_invalid_argument()
        self.add_method('root_var',
                        func_body=meth_root_var,
                        doc_str='return root variable')

        def meth_root_cofactor0(writer):
            with writer.gen_try_block():
                writer.gen_return_pyobject('PyBdd', 'val.root_cofactor0()')
            writer.gen_catch_invalid_argument()
        self.add_method('root_cofactor0',
                        func_body=meth_root_cofactor0,
                        doc_str='return negative cofactor of root variable')

        def meth_root_cofactor1(writer):
            with writer.gen_try_block():
                writer.gen_return_pyobject('PyBdd', 'val.root_cofactor1()')
            writer.gen_catch_invalid_argument()
        self.add_method('root_cofactor1',
                        func_body=meth_root_cofactor1,
                        doc_str='return positive cofactor of root variable')

        def meth_eval(writer):
            pass
        self.add_method('eval',
                        func_body=meth_eval,
                        arg_list=[],
                        doc_str='evaluate input vector')

        def meth_to_litlist(writer):
            with writer.gen_try_block():
                writer.gen_return_pyobject('PyList<Bdd, PyBdd>',
                                           'val.to_litlist()')
            writer.gen_catch_invalid_argument()
        self.add_method('to_litlist',
                        func_body=meth_to_litlist,
                        doc_str='get literal list')

        def meth_to_truth(writer):
            pass

        def meth_gen_dot(writer):
            pass

        def meth_gen_dot2(writer):
            pass

        def get_size(writer):
            writer.gen_return_py_long('val.size()')
        self.add_getter('get_size',
                        func_body=get_size)
        self.add_attr('size',
                      getter_name='get_size')

        def get_mgr(writer):
            writer.gen_return_pyobject('PyBddMgr', 'val.mgr()')
        self.add_getter('get_mgr',
                        func_body=get_mgr)
        self.add_attr('mgr',
                      getter_name='get_mgr')
        
        def cmp_func(writer):
            with writer.gen_if_block('PyBdd::Check(other)'):
                self.gen_ref_conv(writer, objname='other', refname='val2')
                with writer.gen_if_block('op == Py_EQ'):
                    writer.gen_return_py_bool('val == val2')
                with writer.gen_if_block('op == Py_NE'):
                    writer.gen_return_py_bool('val != val2')
            writer.gen_return_py_notimplemented()
        self.add_richcompare(cmp_func)

        def nb_invert(writer):
            writer.gen_return_pyobject('PyBdd', '~val')

        def nb_and(writer):
            with writer.gen_if_block('PyBdd::Check(other)'):
                self.gen_ref_conv(writer, objname='other', refname='val2')
                with writer.gen_try_block():
                    writer.gen_return_pyobject('PyBdd', 'val & val2')
                writer.gen_catch_invalid_argument()
            writer.gen_return_py_notimplemented()

        def nb_or(writer):
            with writer.gen_if_block('PyBdd::Check(other)'):
                self.gen_ref_conv(writer, objname='other', refname='val2')
                with writer.gen_try_block():
                    writer.gen_return_pyobject('PyBdd', 'val | val2')
                writer.gen_catch_invalid_argument()
            writer.gen_return_py_notimplemented()

        def nb_xor(writer):
            with writer.gen_if_block('PyBdd::Check(other)'):
                self.gen_ref_conv(writer, objname='other', refname='val2')
                with writer.gen_try_block():
                    writer.gen_return_pyobject('PyBdd', 'val ^ val2')
                writer.gen_catch_invalid_argument()
            writer.gen_return_py_notimplemented()

        def nb_inplace_and(writer):
            with writer.gen_if_block('PyBdd::Check(other)'):
                self.gen_ref_conv(writer, objname='other', refname='val2')
                with writer.gen_try_block():
                    writer.write_line('val &= val2;')
                    writer.gen_return_self(incref=True)
                writer.gen_catch_invalid_argument()
            writer.gen_return_py_notimplemented()

        def nb_inplace_or(writer):
            with writer.gen_if_block('PyBdd::Check(other)'):
                self.gen_ref_conv(writer, objname='other', refname='val2')
                with writer.gen_try_block():
                    writer.write_line('val |= val2;')
                    writer.gen_return_self(incref=True)
                writer.gen_catch_invalid_argument()
            writer.gen_return_py_notimplemented()

        def nb_inplace_xor(writer):
            with writer.gen_if_block('PyBdd::Check(other)'):
                self.gen_ref_conv(writer, objname='other', refname='val2')
                with writer.gen_try_block():
                    writer.write_line('val ^= val2;')
                    writer.gen_return_self(incref=True)
                writer.gen_catch_invalid_argument()
            writer.gen_return_py_notimplemented()

        self.add_number(nb_invert=nb_invert,
                        nb_and=nb_and,
                        nb_or=nb_or,
                        nb_xor=nb_xor,
                        nb_inplace_and=nb_inplace_and,
                        nb_inplace_or=nb_inplace_or,
                        nb_inplace_xor=nb_inplace_xor)

        def hash_func(writer):
            writer.gen_return('val.hash()')
        self.add_hash(hash_func)
        
        def conv_func(writer):
            self.gen_alloc_code(writer, varname='obj')
            self.gen_obj_conv(writer, objname='obj', varname='my_obj')
            writer.write_line(f'new (&my_obj->mVal) {self.classname}(std::move(val));')
            writer.gen_return('obj')
        self.add_conv(conv_func)

        self.add_deconv('default')
            
            
if __name__ == '__main__':

    gen = BddGen()

    gen.make_header()

    gen.make_source()
