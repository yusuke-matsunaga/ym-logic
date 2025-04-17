
/// @file PyBdd.cc
/// @brief PyBdd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyBdd.h"
#include "pym/PyBddMgr.h"
#include "pym/PyBddVar.h"
#include "pym/PyBddLit.h"
#include "pym/PyBddCompMap.h"
#include "pym/PyBddVarMap.h"
#include "pym/PyBddVarSet.h"
#include "pym/PyLiteral.h"
#include "pym/PyNpnMap.h"
#include "pym/PyInt.h"
#include "pym/PyLong.h"
#include "pym/PyBool.h"
#include "pym/PyString.h"
#include "pym/PyModule.h"
#include "ym/BddVar.h"
#include "ym/BddLit.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
// この構造体は同じサイズのヒープから作られるので
// mVal のコンストラクタは起動されないことに注意．
// そのためあとでコンストラクタを明示的に起動する必要がある．
// またメモリを開放するときにも明示的にデストラクタを起動する必要がある．
struct Bdd_Object
{
  PyObject_HEAD
  Bdd mVal;
};

// Python 用のタイプ定義
PyTypeObject Bdd_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PyBdd::init() 中で初期化する．
};

// 終了関数
void
dealloc_func(
  PyObject* self
)
{
  auto obj = reinterpret_cast<Bdd_Object*>(self);
  obj->mVal.~Bdd();
  Py_TYPE(self)->tp_free(self);
}

PyObject*
nb_invert(
  PyObject* self
)
{
  auto& val = PyBdd::_get_ref(self);
  try {
    return PyBdd::ToPyObject(~val);
  }
  catch ( std::invalid_argument err ) {
    std::ostringstream buf;
    buf << "invalid argument" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
nb_and(
  PyObject* self,
  PyObject* other
)
{
  try {
    if ( PyBdd::Check(self) ) {
      auto& val1 = PyBdd::_get_ref(self);
      if ( PyBdd::Check(other) ) {
        auto& val2 = PyBdd::_get_ref(other);
        return PyBdd::ToPyObject(val1 & val2);
      }
    }
    Py_RETURN_NOTIMPLEMENTED;
  }
  catch ( std::invalid_argument err ) {
    std::ostringstream buf;
    buf << "invalid argument" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
nb_xor(
  PyObject* self,
  PyObject* other
)
{
  try {
    if ( PyBdd::Check(self) ) {
      auto& val1 = PyBdd::_get_ref(self);
      if ( PyBdd::Check(other) ) {
        auto& val2 = PyBdd::_get_ref(other);
        return PyBdd::ToPyObject(val1 ^ val2);
      }
    }
    Py_RETURN_NOTIMPLEMENTED;
  }
  catch ( std::invalid_argument err ) {
    std::ostringstream buf;
    buf << "invalid argument" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
nb_or(
  PyObject* self,
  PyObject* other
)
{
  try {
    if ( PyBdd::Check(self) ) {
      auto& val1 = PyBdd::_get_ref(self);
      if ( PyBdd::Check(other) ) {
        auto& val2 = PyBdd::_get_ref(other);
        return PyBdd::ToPyObject(val1 | val2);
      }
    }
    Py_RETURN_NOTIMPLEMENTED;
  }
  catch ( std::invalid_argument err ) {
    std::ostringstream buf;
    buf << "invalid argument" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
nb_inplace_and(
  PyObject* self,
  PyObject* other
)
{
  try {
    if ( PyBdd::Check(self) ) {
      auto& val1 = PyBdd::_get_ref(self);
      if ( PyBdd::Check(other) ) {
        auto& val2 = PyBdd::_get_ref(other);
        val1 &= val2;
        Py_XINCREF(self);
        return self;
      }
    }
    Py_RETURN_NOTIMPLEMENTED;
  }
  catch ( std::invalid_argument err ) {
    std::ostringstream buf;
    buf << "invalid argument" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
nb_inplace_xor(
  PyObject* self,
  PyObject* other
)
{
  try {
    if ( PyBdd::Check(self) ) {
      auto& val1 = PyBdd::_get_ref(self);
      if ( PyBdd::Check(other) ) {
        auto& val2 = PyBdd::_get_ref(other);
        val1 ^= val2;
        Py_XINCREF(self);
        return self;
      }
    }
    Py_RETURN_NOTIMPLEMENTED;
  }
  catch ( std::invalid_argument err ) {
    std::ostringstream buf;
    buf << "invalid argument" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
nb_inplace_or(
  PyObject* self,
  PyObject* other
)
{
  try {
    if ( PyBdd::Check(self) ) {
      auto& val1 = PyBdd::_get_ref(self);
      if ( PyBdd::Check(other) ) {
        auto& val2 = PyBdd::_get_ref(other);
        val1 |= val2;
        Py_XINCREF(self);
        return self;
      }
    }
    Py_RETURN_NOTIMPLEMENTED;
  }
  catch ( std::invalid_argument err ) {
    std::ostringstream buf;
    buf << "invalid argument" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// Numberオブジェクト構造体
PyNumberMethods number = {
  .nb_invert = nb_invert,
  .nb_and = nb_and,
  .nb_xor = nb_xor,
  .nb_or = nb_or,
  .nb_inplace_and = nb_inplace_and,
  .nb_inplace_xor = nb_inplace_xor,
  .nb_inplace_or = nb_inplace_or
};

// hash 関数
Py_hash_t
hash_func(
  PyObject* self
)
{
  auto& val = PyBdd::_get_ref(self);
  try {
    return val.hash();
  }
  catch ( std::invalid_argument err ) {
    std::ostringstream buf;
    buf << "invalid argument" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return 0;
  }
}

// richcompare 関数
PyObject*
richcompare_func(
  PyObject* self,
  PyObject* other,
  int op
)
{
  auto& val = PyBdd::_get_ref(self);
  try {
    if ( PyBdd::Check(other) ) {
      auto& val2 = PyBdd::_get_ref(other);
      if ( op == Py_EQ ) {
        return PyBool_FromLong(val == val2);
      }
      if ( op == Py_NE ) {
        return PyBool_FromLong(val != val2);
      }
    }
    Py_RETURN_NOTIMPLEMENTED;
  }
  catch ( std::invalid_argument err ) {
    std::ostringstream buf;
    buf << "invalid argument" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// make invalid Bdd object
PyObject*
invalid(
  PyObject* Py_UNUSED(self),
  PyObject* Py_UNUSED(args)
)
{
  return PyBdd::ToPyObject(Bdd::invalid());
}

// make cofactor
PyObject*
cofactor(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "",
    "inv",
    nullptr
  };
  PyObject* val2_obj = nullptr;
  int inv_tmp = -1;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O!|$p",
                                    const_cast<char**>(kwlist),
                                    PyBdd::_typeobject(), &val2_obj,
                                    &inv_tmp) ) {
    return nullptr;
  }
  Bdd val2;
  if ( val2_obj != nullptr ) {
    if ( !PyBdd::FromPyObject(val2_obj, val2) ) {
      PyErr_SetString(PyExc_TypeError, "could not convert to Bdd");
      return nullptr;
    }
  }
  bool inv = false;
  if ( inv_tmp != -1 ) {
    inv = static_cast<bool>(inv_tmp);
  }
  auto& val = PyBdd::_get_ref(self);
  if ( inv ) {
    val2 = ~val2;
  }
  try {
    return PyBdd::ToPyObject(val.cofactor(val2));
  }
  catch ( std::invalid_argument err ) {
    std::ostringstream buf;
    buf << "argument 1 must be a cube-type BDD" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// ITE op
PyObject*
ite(
  PyObject* Py_UNUSED(self),
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "cond_bdd",
    "then_bdd",
    "else_bdd",
    nullptr
  };
  PyObject* val1_obj = nullptr;
  PyObject* val2_obj = nullptr;
  PyObject* val3_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O!O!O!",
                                    const_cast<char**>(kwlist),
                                    PyBdd::_typeobject(), &val1_obj,
                                    PyBdd::_typeobject(), &val2_obj,
                                    PyBdd::_typeobject(), &val3_obj) ) {
    return nullptr;
  }
  Bdd val1;
  if ( val1_obj != nullptr ) {
    if ( !PyBdd::FromPyObject(val1_obj, val1) ) {
      PyErr_SetString(PyExc_TypeError, "could not convert to Bdd");
      return nullptr;
    }
  }
  Bdd val2;
  if ( val2_obj != nullptr ) {
    if ( !PyBdd::FromPyObject(val2_obj, val2) ) {
      PyErr_SetString(PyExc_TypeError, "could not convert to Bdd");
      return nullptr;
    }
  }
  Bdd val3;
  if ( val3_obj != nullptr ) {
    if ( !PyBdd::FromPyObject(val3_obj, val3) ) {
      PyErr_SetString(PyExc_TypeError, "could not convert to Bdd");
      return nullptr;
    }
  }
  try {
    return PyBdd::ToPyObject(Bdd::ite(val1, val2, val3));
  }
  catch ( std::invalid_argument err ) {
    std::ostringstream buf;
    buf << "invalid argument" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// COMPOSE op
PyObject*
compose(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "var",
    "operand",
    nullptr
  };
  PyObject* var_obj = nullptr;
  PyObject* operand_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "OO!",
                                    const_cast<char**>(kwlist),
                                    &var_obj,
                                    PyBdd::_typeobject(), &operand_obj) ) {
    return nullptr;
  }
  BddVar var;
  if ( var_obj != nullptr ) {
    if ( !PyBddVar::FromPyObject(var_obj, var) ) {
      PyErr_SetString(PyExc_ValueError, "could not convert to BddVar");
      return nullptr;
    }
  }
  Bdd operand;
  if ( operand_obj != nullptr ) {
    if ( !PyBdd::FromPyObject(operand_obj, operand) ) {
      PyErr_SetString(PyExc_TypeError, "could not convert to Bdd");
      return nullptr;
    }
  }
  auto& val = PyBdd::_get_ref(self);
  try {
    return PyBdd::ToPyObject(val.compose(var, operand));
  }
  catch ( std::invalid_argument err ) {
    std::ostringstream buf;
    buf << "invalid argument" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// multiway COMPOSE op
PyObject*
multi_compose(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "map",
    nullptr
  };
  PyObject* dict_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O!",
                                    const_cast<char**>(kwlist),
                                    &PyDict_Type, &dict_obj) ) {
    return nullptr;
  }
  auto& val = PyBdd::_get_ref(self);
  std::unordered_map<BddVar, Bdd> compose_map;
  if ( !PyBddCompMap::FromPyObject(dict_obj, compose_map) ) {
    PyErr_SetString(PyExc_TypeError, "argument 1 should be a dictionary from 'BddVar' to 'Bdd'");
    return nullptr;
  }
  try {
    return PyBdd::ToPyObject(val.multi_compose(compose_map));
  }
  catch ( std::invalid_argument err ) {
    std::ostringstream buf;
    buf << "invalid argument" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// remap variable ordering
PyObject*
remap_vars(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "map",
    nullptr
  };
  PyObject* dict_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O!",
                                    const_cast<char**>(kwlist),
                                    &PyDict_Type, &dict_obj) ) {
    return nullptr;
  }
  auto& val = PyBdd::_get_ref(self);
  std::unordered_map<BddVar, BddLit> var_map;
  if ( !PyBddVarMap::FromPyObject(dict_obj, var_map) ) {
    PyErr_SetString(PyExc_TypeError, "argument 1 should be a dictionary from 'BddVar' to 'BddLit'");
    return nullptr;
  }
  try {
    return PyBdd::ToPyObject(val.remap_vars(var_map));
  }
  catch ( std::invalid_argument err ) {
    std::ostringstream buf;
    buf << "invalid argument" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// return True if valid
PyObject*
is_valid(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyBdd::_get_ref(self);
  return PyBool_FromLong(val.is_valid());
}

// return True if invalid
PyObject*
is_invalid(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyBdd::_get_ref(self);
  return PyBool_FromLong(val.is_invalid());
}

// return True if ZERO
PyObject*
is_zero(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyBdd::_get_ref(self);
  return PyBool_FromLong(val.is_zero());
}

// return True if ONE
PyObject*
is_one(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyBdd::_get_ref(self);
  return PyBool_FromLong(val.is_one());
}

// return True if CUBE
PyObject*
is_cube(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyBdd::_get_ref(self);
  return PyBool_FromLong(val.is_cube());
}

// return True if positive CUBE
PyObject*
is_posicube(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyBdd::_get_ref(self);
  return PyBool_FromLong(val.is_posicube());
}

// check support variable
PyObject*
check_sup(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "var",
    nullptr
  };
  PyObject* var_bdd_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O!",
                                    const_cast<char**>(kwlist),
                                    PyBdd::_typeobject(), &var_bdd_obj) ) {
    return nullptr;
  }
  Bdd var_bdd;
  if ( var_bdd_obj != nullptr ) {
    if ( !PyBdd::FromPyObject(var_bdd_obj, var_bdd) ) {
      PyErr_SetString(PyExc_TypeError, "could not convert to Bdd");
      return nullptr;
    }
  }
  auto& val = PyBdd::_get_ref(self);
  auto var = BddVar::from_bdd(var_bdd);
  if ( var.is_invalid() ) {
    PyErr_SetString(PyExc_TypeError, "argument 1 must be a variable");
    return nullptr;
  }
  try {
    return PyBool_FromLong(val.check_sup(var));
  }
  catch ( std::invalid_argument err ) {
    std::ostringstream buf;
    buf << "invalid argument" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// check symmmetry
PyObject*
check_sym(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "var1",
    "var2",
    "inv",
    nullptr
  };
  PyObject* var1_bdd_obj = nullptr;
  PyObject* var2_bdd_obj = nullptr;
  int inv_tmp = -1;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O!O!|$p",
                                    const_cast<char**>(kwlist),
                                    PyBdd::_typeobject(), &var1_bdd_obj,
                                    PyBdd::_typeobject(), &var2_bdd_obj,
                                    &inv_tmp) ) {
    return nullptr;
  }
  Bdd var1_bdd;
  if ( var1_bdd_obj != nullptr ) {
    if ( !PyBdd::FromPyObject(var1_bdd_obj, var1_bdd) ) {
      PyErr_SetString(PyExc_TypeError, "could not convert to Bdd");
      return nullptr;
    }
  }
  Bdd var2_bdd;
  if ( var2_bdd_obj != nullptr ) {
    if ( !PyBdd::FromPyObject(var2_bdd_obj, var2_bdd) ) {
      PyErr_SetString(PyExc_TypeError, "could not convert to Bdd");
      return nullptr;
    }
  }
  bool inv = false;
  if ( inv_tmp != -1 ) {
    inv = static_cast<bool>(inv_tmp);
  }
  auto& val = PyBdd::_get_ref(self);
  auto var1 = BddVar::from_bdd(var1_bdd);
  if ( var1.is_invalid() ) {
    PyErr_SetString(PyExc_TypeError, "argument 1 must be a variable");
    return nullptr;
  }
  auto var2 = BddVar::from_bdd(var2_bdd);
  if ( var2.is_invalid() ) {
    PyErr_SetString(PyExc_TypeError, "argument 2 must be a variable");
    return nullptr;
  }
  try {
    return PyBool_FromLong(val.check_sym(var1, var2, inv));
  }
  catch ( std::invalid_argument err ) {
    std::ostringstream buf;
    buf << "invalid argument" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// get support variable
PyObject*
get_support(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyBdd::_get_ref(self);
  try {
    return PyBddVarSet::ToPyObject(val.get_support());
  }
  catch ( std::invalid_argument err ) {
    std::ostringstream buf;
    buf << "invalid argument" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// get ONE path
PyObject*
get_onepath(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyBdd::_get_ref(self);
  try {
    return PyBdd::ToPyObject(val.get_onepath());
  }
  catch ( std::invalid_argument err ) {
    std::ostringstream buf;
    buf << "invalid argument" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// get ZERO path
PyObject*
get_zeropath(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyBdd::_get_ref(self);
  try {
    return PyBdd::ToPyObject(val.get_onepath());
  }
  catch ( std::invalid_argument err ) {
    std::ostringstream buf;
    buf << "invalid argument" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// decompose root node
PyObject*
root_decomp(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyBdd::_get_ref(self);
  try {
    Bdd f0;
    Bdd f1;
    auto var = val.root_decomp(f0, f1);
    auto var_obj = PyBdd::ToPyObject(var);
    auto f0_obj = PyBdd::ToPyObject(f0);
    auto f1_obj = PyBdd::ToPyObject(f1);
    return Py_BuildValue("OOO", var_obj, f0_obj, f1_obj);
  }
  catch ( std::invalid_argument err ) {
    std::ostringstream buf;
    buf << "invalid argument" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// return root variable
PyObject*
root_var(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyBdd::_get_ref(self);
  try {
    return PyBdd::ToPyObject(val.root_var());
  }
  catch ( std::invalid_argument err ) {
    std::ostringstream buf;
    buf << "invalid argument" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// return negative cofactor of root variable
PyObject*
root_cofactor0(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyBdd::_get_ref(self);
  try {
    return PyBdd::ToPyObject(val.root_cofactor0());
  }
  catch ( std::invalid_argument err ) {
    std::ostringstream buf;
    buf << "invalid argument" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// return positive cofactor of root variable
PyObject*
root_cofactor1(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyBdd::_get_ref(self);
  try {
    return PyBdd::ToPyObject(val.root_cofactor1());
  }
  catch ( std::invalid_argument err ) {
    std::ostringstream buf;
    buf << "invalid argument" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// evaluate input vector
PyObject*
eval(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "input_vector",
    nullptr
  };
  PyObject* vector_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O",
                                    const_cast<char**>(kwlist),
                                    &vector_obj) ) {
    return nullptr;
  }
  auto& val = PyBdd::_get_ref(self);
  std::vector<bool> input_vector;
  if ( !PyList<bool, PyBool>::FromPyObject(vector_obj, input_vector) ) {
    PyErr_SetString(PyExc_TypeError, "artument 1 should be a sequence of bool");
    return nullptr;
  }
  return PyBool_FromLong(val.eval(input_vector));
}

// get literal list
PyObject*
to_litlist(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyBdd::_get_ref(self);
  try {
    return PyList<BddLit, PyBddLit>::ToPyObject(val.to_litlist());
  }
  catch ( std::invalid_argument err ) {
    std::ostringstream buf;
    buf << "invalid argument" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// convert to truth table
PyObject*
to_truth(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "var_list",
    nullptr
  };
  PyObject* list_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O",
                                    const_cast<char**>(kwlist),
                                    &list_obj) ) {
    return nullptr;
  }
  auto& val = PyBdd::_get_ref(self);
  std::vector<Bdd> tmp_list;
  if ( !PyList<Bdd, PyBdd>::FromPyObject(list_obj, tmp_list) ) {
    PyErr_SetString(PyExc_TypeError, "argument 1 should be a sequence of 'BddVar'");
    return nullptr;
  }
  std::vector<BddVar> var_list;
  if ( !BddVar::from_bdd_list(tmp_list, var_list) ) {
    PyErr_SetString(PyExc_TypeError, "argument 1 should be a sequence of 'BddVar'");
    return nullptr;
  }
  try {
    return PyString::ToPyObject(val.to_truth(var_list));
  }
  catch ( std::invalid_argument err ) {
    std::ostringstream buf;
    buf << "invalid argument" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// メソッド定義
PyMethodDef methods[] = {
  {"invalid",
   invalid,
   METH_NOARGS | METH_STATIC,
   PyDoc_STR("make invalid Bdd object")},
  {"cofactor",
   reinterpret_cast<PyCFunction>(cofactor),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("make cofactor")},
  {"ite",
   reinterpret_cast<PyCFunction>(ite),
   METH_VARARGS | METH_KEYWORDS | METH_STATIC,
   PyDoc_STR("ITE op")},
  {"compose",
   reinterpret_cast<PyCFunction>(compose),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("COMPOSE op")},
  {"multi_compose",
   reinterpret_cast<PyCFunction>(multi_compose),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("multiway COMPOSE op")},
  {"remap_vars",
   reinterpret_cast<PyCFunction>(remap_vars),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("remap variable ordering")},
  {"is_valid",
   is_valid,
   METH_NOARGS,
   PyDoc_STR("return True if valid")},
  {"is_invalid",
   is_invalid,
   METH_NOARGS,
   PyDoc_STR("return True if invalid")},
  {"is_zero",
   is_zero,
   METH_NOARGS,
   PyDoc_STR("return True if ZERO")},
  {"is_one",
   is_one,
   METH_NOARGS,
   PyDoc_STR("return True if ONE")},
  {"is_cube",
   is_cube,
   METH_NOARGS,
   PyDoc_STR("return True if CUBE")},
  {"is_posicube",
   is_posicube,
   METH_NOARGS,
   PyDoc_STR("return True if positive CUBE")},
  {"check_sup",
   reinterpret_cast<PyCFunction>(check_sup),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("check support variable")},
  {"check_sym",
   reinterpret_cast<PyCFunction>(check_sym),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("check symmmetry")},
  {"get_support",
   get_support,
   METH_NOARGS,
   PyDoc_STR("get support variable")},
  {"get_onepath",
   get_onepath,
   METH_NOARGS,
   PyDoc_STR("get ONE path")},
  {"get_zeropath",
   get_zeropath,
   METH_NOARGS,
   PyDoc_STR("get ZERO path")},
  {"root_decomp",
   root_decomp,
   METH_NOARGS,
   PyDoc_STR("decompose root node")},
  {"root_var",
   root_var,
   METH_NOARGS,
   PyDoc_STR("return root variable")},
  {"root_cofactor0",
   root_cofactor0,
   METH_NOARGS,
   PyDoc_STR("return negative cofactor of root variable")},
  {"root_cofactor1",
   root_cofactor1,
   METH_NOARGS,
   PyDoc_STR("return positive cofactor of root variable")},
  {"eval",
   reinterpret_cast<PyCFunction>(eval),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("evaluate input vector")},
  {"to_litlist",
   to_litlist,
   METH_NOARGS,
   PyDoc_STR("get literal list")},
  {"to_truth",
   reinterpret_cast<PyCFunction>(to_truth),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("convert to truth table")},
  // end-marker
  {nullptr, nullptr, 0, nullptr}
};

PyObject*
get_size(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyBdd::_get_ref(self);
  return PyLong::ToPyObject(val.size());
}

PyObject*
get_mgr(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyBdd::_get_ref(self);
  return PyBddMgr::ToPyObject(val.mgr());
}

// getter/setter定義
PyGetSetDef getsets[] = {
  {"size", get_size, nullptr, PyDoc_STR(""), nullptr},
  {"mgr", get_mgr, nullptr, PyDoc_STR(""), nullptr},
  // end-marker
  {nullptr, nullptr, nullptr, nullptr}
};

// new 関数
PyObject*
new_func(
  PyTypeObject* type,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    nullptr
  };
  // 余分な引数を取らないことを確認しておく．
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "", const_cast<char**>(kwlist)) ) {
    return nullptr;
  }
  try {
    auto self = type->tp_alloc(type, 0);
    auto my_obj = reinterpret_cast<Bdd_Object*>(self);
    new (&my_obj->mVal) Bdd();
    return self;
  }
  catch ( std::invalid_argument err ) {
    std::ostringstream buf;
    buf << "invalid argument" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

END_NONAMESPACE


// @brief Bdd オブジェクトを使用可能にする．
bool
PyBdd::init(
  PyObject* m
)
{
  Bdd_Type.tp_name = "Bdd";
  Bdd_Type.tp_basicsize = sizeof(Bdd_Object);
  Bdd_Type.tp_itemsize = 0;
  Bdd_Type.tp_dealloc = dealloc_func;
  Bdd_Type.tp_as_number = &number;
  Bdd_Type.tp_hash = hash_func;
  Bdd_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  Bdd_Type.tp_doc = PyDoc_STR("Python extended object for Bdd");
  Bdd_Type.tp_richcompare = richcompare_func;
  Bdd_Type.tp_methods = methods;
  Bdd_Type.tp_getset = getsets;
  Bdd_Type.tp_new = new_func;
  if ( !PyModule::reg_type(m, "Bdd", &Bdd_Type) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// Bdd を PyObject に変換する．
PyObject*
PyBdd::Conv::operator()(
  const ElemType& val ///< [in] 元の値
)
{
  auto type = PyBdd::_typeobject();
  auto obj = type->tp_alloc(type, 0);
  auto my_obj = reinterpret_cast<Bdd_Object*>(obj);
  new (&my_obj->mVal) Bdd(val);
  return obj;
}

// PyObject を Bdd に変換する．
bool
PyBdd::Deconv::operator()(
  PyObject* obj, ///< [in] Python のオブジェクト
  ElemType& val  ///< [out] 結果を格納する変数
)
{
  if ( PyBdd::Check(obj) ) {
    val = PyBdd::_get_ref(obj);
    return true;
  }
  return false;
}

// @brief PyObject が Bdd タイプか調べる．
bool
PyBdd::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, &Bdd_Type);
}

// @brief PyObject から Bdd を取り出す．
Bdd&
PyBdd::_get_ref(
  PyObject* obj
)
{
  auto my_obj = reinterpret_cast<Bdd_Object*>(obj);
  return my_obj->mVal;
}

// @brief Bdd を表すオブジェクトの型定義を返す．
PyTypeObject*
PyBdd::_typeobject()
{
  return &Bdd_Type;
}

END_NAMESPACE_YM
