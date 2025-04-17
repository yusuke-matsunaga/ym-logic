
/// @file PyTvFunc.cc
/// @brief PyTvFunc の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyTvFunc.h"
#include "pym/PyLiteral.h"
#include "pym/PyNpnMap.h"
#include "pym/PyPrimType.h"
#include "pym/PySopCover.h"
#include "pym/PyInt.h"
#include "pym/PyLong.h"
#include "pym/PyString.h"
#include "pym/PyModule.h"
#include "ym/Tv2Sop.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
// この構造体は同じサイズのヒープから作られるので
// mVal のコンストラクタは起動されないことに注意．
// そのためあとでコンストラクタを明示的に起動する必要がある．
// またメモリを開放するときにも明示的にデストラクタを起動する必要がある．
struct TvFunc_Object
{
  PyObject_HEAD
  TvFunc mVal;
};

// Python 用のタイプ定義
PyTypeObject TvFunc_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PyTvFunc::init() 中で初期化する．
};

// 終了関数
void
dealloc_func(
  PyObject* self
)
{
  auto obj = reinterpret_cast<TvFunc_Object*>(self);
  obj->mVal.~TvFunc();
  Py_TYPE(self)->tp_free(self);
}

// repr 関数
PyObject*
repr_func(
  PyObject* self
)
{
  auto& val = PyTvFunc::_get_ref(self);
  try {
    auto str_val = val.str();
    return PyString::ToPyObject(str_val);
  }
  catch ( std::invalid_argument err ) {
    std::ostringstream buf;
    buf << "invalid argument" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
nb_invert(
  PyObject* self
)
{
  auto& val = PyTvFunc::_get_ref(self);
  try {
    return PyTvFunc::ToPyObject(~val);
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
    if ( PyTvFunc::Check(self) ) {
      auto& val1 = PyTvFunc::_get_ref(self);
      if ( PyTvFunc::Check(other) ) {
        auto& val2 = PyTvFunc::_get_ref(other);
        return PyTvFunc::ToPyObject(val1 & val2);
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
    if ( PyTvFunc::Check(self) ) {
      auto& val1 = PyTvFunc::_get_ref(self);
      if ( PyTvFunc::Check(other) ) {
        auto& val2 = PyTvFunc::_get_ref(other);
        return PyTvFunc::ToPyObject(val1 ^ val2);
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
    if ( PyTvFunc::Check(self) ) {
      auto& val1 = PyTvFunc::_get_ref(self);
      if ( PyTvFunc::Check(other) ) {
        auto& val2 = PyTvFunc::_get_ref(other);
        return PyTvFunc::ToPyObject(val1 | val2);
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
    if ( PyTvFunc::Check(self) ) {
      auto& val1 = PyTvFunc::_get_ref(self);
      if ( PyTvFunc::Check(other) ) {
        auto& val2 = PyTvFunc::_get_ref(other);
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
    if ( PyTvFunc::Check(self) ) {
      auto& val1 = PyTvFunc::_get_ref(self);
      if ( PyTvFunc::Check(other) ) {
        auto& val2 = PyTvFunc::_get_ref(other);
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
    if ( PyTvFunc::Check(self) ) {
      auto& val1 = PyTvFunc::_get_ref(self);
      if ( PyTvFunc::Check(other) ) {
        auto& val2 = PyTvFunc::_get_ref(other);
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
  auto& val = PyTvFunc::_get_ref(self);
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
  auto& val = PyTvFunc::_get_ref(self);
  try {
    if ( PyTvFunc::Check(other) ) {
      auto& val2 = PyTvFunc::_get_ref(other);
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

// make TvFunc object from string
PyObject*
from_string(
  PyObject* Py_UNUSED(self),
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "str",
    nullptr
  };
  const char* s_tmp = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "s",
                                    const_cast<char**>(kwlist),
                                    &s_tmp) ) {
    return nullptr;
  }
  std::string s = nullptr;
  if ( s_tmp != nullptr ) {
    s = std::string(s_tmp);
  }
  try {
    return PyTvFunc::ToPyObject(TvFunc(s));
  }
  catch ( std::invalid_argument err ) {
    std::ostringstream buf;
    buf << "invalid argument" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// make invalid TvFunc object
PyObject*
invalid(
  PyObject* Py_UNUSED(self),
  PyObject* Py_UNUSED(args)
)
{
  return PyTvFunc::ToPyObject(TvFunc::invalid());
}

// make ZERO TvFunc object
PyObject*
zero(
  PyObject* Py_UNUSED(self),
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "input_num",
    nullptr
  };
  int ni = 0;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "i",
                                    const_cast<char**>(kwlist),
                                    &ni) ) {
    return nullptr;
  }
  return PyTvFunc::ToPyObject(TvFunc::zero(ni));
}

// make ONE TvFunc object
PyObject*
one(
  PyObject* Py_UNUSED(self),
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "input_num",
    nullptr
  };
  int ni = 0;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "i",
                                    const_cast<char**>(kwlist),
                                    &ni) ) {
    return nullptr;
  }
  return PyTvFunc::ToPyObject(TvFunc::zero(ni));
}

// make literal type TvFunc object
PyObject*
literal(
  PyObject* Py_UNUSED(self),
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "input_num",
    "lit",
    "inv",
    nullptr
  };
  int ni;
  PyObject* lit_obj = nullptr;
  int inv_tmp = -1;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "iO|p",
                                    const_cast<char**>(kwlist),
                                    &ni,
                                    &lit_obj,
                                    &inv_tmp) ) {
    return nullptr;
  }
  bool inv = false;
  if ( inv_tmp != -1 ) {
    inv = static_cast<bool>(inv_tmp);
  }
  Literal lit;
  if ( PyLiteral::FromPyObject(lit_obj, lit) ) {
    if ( lit.varid() >= ni ) {
      PyErr_SetString(PyExc_ValueError, "'lit.varid()' is out of range");
      return nullptr;
    }
    if ( inv ) {
      lit = ~lit;
    }
    return PyTvFunc::ToPyObject(TvFunc::literal(ni, lit));
  }
  PyErr_SetString(PyExc_ValueError, "argument 2 must be an integer or a Literal object");
  return nullptr;
}

// make positive literal type TvFunc object
PyObject*
posi_literal(
  PyObject* Py_UNUSED(self),
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "input_num",
    "var",
    nullptr
  };
  int ni;
  int var;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "ii",
                                    const_cast<char**>(kwlist),
                                    &ni,
                                    &var) ) {
    return nullptr;
  }
  if ( var >= ni ) {
    PyErr_SetString(PyExc_ValueError, "'var' is out of range");
    return nullptr;
  }
  return PyTvFunc::ToPyObject(TvFunc::posi_literal(ni, var));
}

// make a cofactor
PyObject*
cofactor(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "lit",
    "inv",
    nullptr
  };
  PyObject* lit_obj = nullptr;
  int inv_tmp = -1;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O|$p",
                                    const_cast<char**>(kwlist),
                                    &lit_obj,
                                    &inv_tmp) ) {
    return nullptr;
  }
  bool inv = false;
  if ( inv_tmp != -1 ) {
    inv = static_cast<bool>(inv_tmp);
  }
  auto& val = PyTvFunc::_get_ref(self);
  Literal lit;
  if ( PyLiteral::FromPyObject(lit_obj, lit) ) {
    if ( lit.varid() >= val.input_num() ) {
      PyErr_SetString(PyExc_ValueError, "'lit.varid()' is out of range");
      return nullptr;
    }
    if ( inv ) {
      lit = ~lit;
    }
    return PyTvFunc::ToPyObject(val.cofactor(lit));
  }
  PyErr_SetString(PyExc_ValueError, "argument 1 must be an integer or a Literal object");
  return nullptr;
}

// make a cofactor
PyObject*
cofactor_int(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "lit",
    "inv",
    nullptr
  };
  PyObject* lit_obj = nullptr;
  int inv_tmp = -1;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O|$p",
                                    const_cast<char**>(kwlist),
                                    &lit_obj,
                                    &inv_tmp) ) {
    return nullptr;
  }
  bool inv = false;
  if ( inv_tmp != -1 ) {
    inv = static_cast<bool>(inv_tmp);
  }
  auto& val = PyTvFunc::_get_ref(self);
  Literal lit;
  if ( PyLiteral::FromPyObject(lit_obj, lit) ) {
    if ( lit.varid() >= val.input_num() ) {
      PyErr_SetString(PyExc_ValueError, "'lit.varid()' is out of range");
      return nullptr;
    }
    if ( inv ) {
      lit = ~lit;
    }
    val.cofactor_int(lit);
    Py_XINCREF(self);
    return self;
  }
  PyErr_SetString(PyExc_ValueError, "argument 1 must be an integer or a Literal object");
  return nullptr;
}

// transform
PyObject*
xform(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "map",
    nullptr
  };
  PyObject* npnmap_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O!",
                                    const_cast<char**>(kwlist),
                                    PyNpnMap::_typeobject(), &npnmap_obj) ) {
    return nullptr;
  }
  NpnMap npnmap;
  if ( npnmap_obj != nullptr ) {
    if ( !PyNpnMap::FromPyObject(npnmap_obj, npnmap) ) {
      PyErr_SetString(PyExc_TypeError, "could not convert to NpnMap");
      return nullptr;
    }
  }
  auto& val = PyTvFunc::_get_ref(self);
  return PyTvFunc::ToPyObject(val.xform(npnmap));
}

// shrink unused inputs
PyObject*
shrink_map(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyTvFunc::_get_ref(self);
  return PyNpnMap::ToPyObject(val.shrink_map());
}

// get cannonical map
PyObject*
npn_cannonical_map(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyTvFunc::_get_ref(self);
  return PyNpnMap::ToPyObject(val.npn_cannonical_map());
}

// get list of all cannonical maps
PyObject*
npn_cannonical_all_map(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyTvFunc::_get_ref(self);
  return PyList<NpnMap, PyNpnMap>::ToPyObject(val.npn_cannonical_all_map());
}

// True if valid
PyObject*
is_valid(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyTvFunc::_get_ref(self);
  return PyBool_FromLong(val.is_valid());
}

// True if invalid
PyObject*
is_invalid(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyTvFunc::_get_ref(self);
  return PyBool_FromLong(val.is_valid());
}

// True if ZERO
PyObject*
is_zero(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyTvFunc::_get_ref(self);
  return PyBool_FromLong(val.is_zero());
}

// True if ONE
PyObject*
is_one(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyTvFunc::_get_ref(self);
  return PyBool_FromLong(val.is_zero());
}

// return truth table value at the specified index
PyObject*
value(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "index",
    nullptr
  };
  int index;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "i",
                                    const_cast<char**>(kwlist),
                                    &index) ) {
    return nullptr;
  }
  auto& val = PyTvFunc::_get_ref(self);
  auto nexp = 1 << val.input_num();
  if ( index >= nexp ) {
    PyErr_SetString(PyExc_ValueError, "'index' is out of range");
    return nullptr;
  }
  return PyInt::ToPyObject(val.value(index));
}

// return the number of minterms of ZERO
PyObject*
count_zero(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyTvFunc::_get_ref(self);
  return PyLong::ToPyObject(val.count_zero());
}

// return the number of minterms of ONE
PyObject*
count_one(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyTvFunc::_get_ref(self);
  return PyLong::ToPyObject(val.count_one());
}

// return Walsh-0 coefficient
PyObject*
walsh_0(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyTvFunc::_get_ref(self);
  return PyLong::ToPyObject(val.walsh_0());
}

// return Walsh-1 coefficient
PyObject*
walsh_1(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "var",
    nullptr
  };
  int var;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "i",
                                    const_cast<char**>(kwlist),
                                    &var) ) {
    return nullptr;
  }
  auto& val = PyTvFunc::_get_ref(self);
  if ( var >= val.input_num() ) {
    PyErr_SetString(PyExc_ValueError, "'var' is out of range");
    return nullptr;
  }
  return PyLong::ToPyObject(val.walsh_1(var));
}

// return Walsh-2 coefficient
PyObject*
walsh_2(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "var1",
    "var2",
    nullptr
  };
  int var1;
  int var2;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "ii",
                                    const_cast<char**>(kwlist),
                                    &var1,
                                    &var2) ) {
    return nullptr;
  }
  auto& val = PyTvFunc::_get_ref(self);
  if ( var1 >= val.input_num() ) {
    PyErr_SetString(PyExc_ValueError, "'var1' is out of range");
    return nullptr;
  }
  if ( var2 >= val.input_num() ) {
    PyErr_SetString(PyExc_ValueError, "'var2' is out of range");
    return nullptr;
  }
  return PyLong::ToPyObject(val.walsh_2(var1, var2));
}

// check if the specified variable is a support
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
  int var;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "i",
                                    const_cast<char**>(kwlist),
                                    &var) ) {
    return nullptr;
  }
  auto& val = PyTvFunc::_get_ref(self);
  if ( var >= val.input_num() ) {
    PyErr_SetString(PyExc_ValueError, "'var' is out of range");
    return nullptr;
  }
  return PyBool_FromLong(val.check_sup(var));
}

// check unateness on the specified variable
PyObject*
check_unate(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "var",
    nullptr
  };
  int var;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "i",
                                    const_cast<char**>(kwlist),
                                    &var) ) {
    return nullptr;
  }
  auto& val = PyTvFunc::_get_ref(self);
  if ( var >= val.input_num() ) {
    PyErr_SetString(PyExc_ValueError, "'var' is out of range");
    return nullptr;
  }
  return PyBool_FromLong(val.check_unate(var));
}

// check synmmetry
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
  int var1;
  int var2;
  int inv_tmp = -1;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "ii|$p",
                                    const_cast<char**>(kwlist),
                                    &var1,
                                    &var2,
                                    &inv_tmp) ) {
    return nullptr;
  }
  bool inv = false;
  if ( inv_tmp != -1 ) {
    inv = static_cast<bool>(inv_tmp);
  }
  auto& val = PyTvFunc::_get_ref(self);
  if ( var1 >= val.input_num() ) {
    PyErr_SetString(PyExc_ValueError, "'var1' is out of range");
    return nullptr;
  }
  if ( var2 >= val.input_num() ) {
    PyErr_SetString(PyExc_ValueError, "'var2' is out of range");
    return nullptr;
  }
  return PyBool_FromLong(val.check_sym(var1, var2, inv));
}

// analyze the function type
PyObject*
analyze(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyTvFunc::_get_ref(self);
  return PyPrimType::ToPyObject(val.analyze());
}

// return all primes
PyObject*
all_primes(
  PyObject* Py_UNUSED(self),
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "f",
    "dc",
    nullptr
  };
  PyObject* f_obj = nullptr;
  PyObject* dc_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O!|O!",
                                    const_cast<char**>(kwlist),
                                    PyTvFunc::_typeobject(), &f_obj,
                                    PyTvFunc::_typeobject(), &dc_obj) ) {
    return nullptr;
  }
  auto& f = PyTvFunc::_get_ref(f_obj);
  if ( dc_obj == nullptr ) {
    auto cube_list = Tv2Sop::all_primes(f);
    return PySopCover::ToPyObject(SopCover(f.input_num(), cube_list));
  }
  else {
    auto& dc = PyTvFunc::_get_ref(dc_obj);
    auto cube_list = Tv2Sop::all_primes(f, dc);
    return PySopCover::ToPyObject(SopCover(f.input_num(), cube_list));
  }
}

// return irredundant SOP
PyObject*
isop(
  PyObject* Py_UNUSED(self),
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "f",
    "dc",
    nullptr
  };
  PyObject* f_obj = nullptr;
  PyObject* dc_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O!|O!",
                                    const_cast<char**>(kwlist),
                                    PyTvFunc::_typeobject(), &f_obj,
                                    PyTvFunc::_typeobject(), &dc_obj) ) {
    return nullptr;
  }
  auto& f = PyTvFunc::_get_ref(f_obj);
  if ( dc_obj == nullptr ) {
    auto cube_list = Tv2Sop::isop(f);
    return PySopCover::ToPyObject(SopCover(f.input_num(), cube_list));
  }
  else {
    auto& dc = PyTvFunc::_get_ref(dc_obj);
    auto cube_list = Tv2Sop::isop(f, dc);
    return PySopCover::ToPyObject(SopCover(f.input_num(), cube_list));
  }
}

// メソッド定義
PyMethodDef methods[] = {
  {"from_string",
   reinterpret_cast<PyCFunction>(from_string),
   METH_VARARGS | METH_KEYWORDS | METH_STATIC,
   PyDoc_STR("make TvFunc object from string")},
  {"invalid",
   invalid,
   METH_NOARGS | METH_STATIC,
   PyDoc_STR("make invalid TvFunc object")},
  {"zero",
   reinterpret_cast<PyCFunction>(zero),
   METH_VARARGS | METH_KEYWORDS | METH_STATIC,
   PyDoc_STR("make ZERO TvFunc object")},
  {"one",
   reinterpret_cast<PyCFunction>(one),
   METH_VARARGS | METH_KEYWORDS | METH_STATIC,
   PyDoc_STR("make ONE TvFunc object")},
  {"literal",
   reinterpret_cast<PyCFunction>(literal),
   METH_VARARGS | METH_KEYWORDS | METH_STATIC,
   PyDoc_STR("make literal type TvFunc object")},
  {"posi_literal",
   reinterpret_cast<PyCFunction>(posi_literal),
   METH_VARARGS | METH_KEYWORDS | METH_STATIC,
   PyDoc_STR("make positive literal type TvFunc object")},
  {"cofactor",
   reinterpret_cast<PyCFunction>(cofactor),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("make a cofactor")},
  {"cofactor_int",
   reinterpret_cast<PyCFunction>(cofactor_int),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("make a cofactor")},
  {"xform",
   reinterpret_cast<PyCFunction>(xform),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("transform")},
  {"shrink_map",
   shrink_map,
   METH_NOARGS,
   PyDoc_STR("shrink unused inputs")},
  {"npn_cannonical_map",
   npn_cannonical_map,
   METH_NOARGS,
   PyDoc_STR("get cannonical map")},
  {"npn_cannonical_all_map",
   npn_cannonical_all_map,
   METH_NOARGS,
   PyDoc_STR("get list of all cannonical maps")},
  {"is_valid",
   is_valid,
   METH_NOARGS,
   PyDoc_STR("True if valid")},
  {"is_invalid",
   is_invalid,
   METH_NOARGS,
   PyDoc_STR("True if invalid")},
  {"is_zero",
   is_zero,
   METH_NOARGS,
   PyDoc_STR("True if ZERO")},
  {"is_one",
   is_one,
   METH_NOARGS,
   PyDoc_STR("True if ONE")},
  {"value",
   reinterpret_cast<PyCFunction>(value),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return truth table value at the specified index")},
  {"count_zero",
   count_zero,
   METH_NOARGS,
   PyDoc_STR("return the number of minterms of ZERO")},
  {"count_one",
   count_one,
   METH_NOARGS,
   PyDoc_STR("return the number of minterms of ONE")},
  {"walsh_0",
   walsh_0,
   METH_NOARGS,
   PyDoc_STR("return Walsh-0 coefficient")},
  {"walsh_1",
   reinterpret_cast<PyCFunction>(walsh_1),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return Walsh-1 coefficient")},
  {"walsh_2",
   reinterpret_cast<PyCFunction>(walsh_2),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return Walsh-2 coefficient")},
  {"check_sup",
   reinterpret_cast<PyCFunction>(check_sup),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("check if the specified variable is a support")},
  {"check_unate",
   reinterpret_cast<PyCFunction>(check_unate),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("check unateness on the specified variable")},
  {"check_sym",
   reinterpret_cast<PyCFunction>(check_sym),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("check synmmetry")},
  {"analyze",
   analyze,
   METH_NOARGS,
   PyDoc_STR("analyze the function type")},
  {"all_primes",
   reinterpret_cast<PyCFunction>(all_primes),
   METH_VARARGS | METH_KEYWORDS | METH_STATIC,
   PyDoc_STR("return all primes")},
  {"isop",
   reinterpret_cast<PyCFunction>(isop),
   METH_VARARGS | METH_KEYWORDS | METH_STATIC,
   PyDoc_STR("return irredundant SOP")},
  // end-marker
  {nullptr, nullptr, 0, nullptr}
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
    "input_num",
    "values",
    nullptr
  };
  int ni = -1;
  PyObject* vect_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "|i$O",
                                    const_cast<char**>(kwlist),
                                    &ni,
                                    &vect_obj) ) {
    return nullptr;
  }
  try {
    TvFunc func;
    std::vector<int> vect;
    if ( ni == -1 ) {
      func = TvFunc::invalid();
    }
    else if ( vect_obj == nullptr ) {
      func = TvFunc::zero(ni);
    }
    else if ( PyList<int, PyInt>::FromPyObject(vect_obj, vect) ) {
      auto n = vect.size();
      if ( n != (1 << ni) ) {
        PyErr_SetString(PyExc_ValueError, "invalid vector size");
        return nullptr;
      }
      func = TvFunc(ni, vect);
    }
    else {
      PyErr_SetString(PyExc_ValueError, "argument 2 must be a sequence of ints");
      return nullptr;
    }
    return PyTvFunc::ToPyObject(std::move(func));
  }
  catch ( std::invalid_argument err ) {
    std::ostringstream buf;
    buf << "invalid argument" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

END_NONAMESPACE


// @brief TvFunc オブジェクトを使用可能にする．
bool
PyTvFunc::init(
  PyObject* m
)
{
  TvFunc_Type.tp_name = "TvFunc";
  TvFunc_Type.tp_basicsize = sizeof(TvFunc_Object);
  TvFunc_Type.tp_itemsize = 0;
  TvFunc_Type.tp_dealloc = dealloc_func;
  TvFunc_Type.tp_repr = repr_func;
  TvFunc_Type.tp_as_number = &number;
  TvFunc_Type.tp_hash = hash_func;
  TvFunc_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  TvFunc_Type.tp_doc = PyDoc_STR("Python extended object for TvFunc");
  TvFunc_Type.tp_richcompare = richcompare_func;
  TvFunc_Type.tp_methods = methods;
  TvFunc_Type.tp_new = new_func;
  if ( !PyModule::reg_type(m, "TvFunc", &TvFunc_Type) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// TvFunc を PyObject に変換する．
PyObject*
PyTvFunc::Conv::operator()(
  const ElemType& val ///< [in] 元の値
)
{
  auto type = PyTvFunc::_typeobject();
  auto obj = type->tp_alloc(type, 0);
  auto my_obj = reinterpret_cast<TvFunc_Object*>(obj);
  new (&my_obj->mVal) TvFunc(std::move(val));
  return obj;
}

// PyObject を TvFunc に変換する．
bool
PyTvFunc::Deconv::operator()(
  PyObject* obj, ///< [in] Python のオブジェクト
  ElemType& val  ///< [out] 結果を格納する変数
)
{
  if ( PyTvFunc::Check(obj) ) {
    val = PyTvFunc::_get_ref(obj);
    return true;
  }
  return false;
}

// @brief PyObject が TvFunc タイプか調べる．
bool
PyTvFunc::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, &TvFunc_Type);
}

// @brief PyObject から TvFunc を取り出す．
TvFunc&
PyTvFunc::_get_ref(
  PyObject* obj
)
{
  auto my_obj = reinterpret_cast<TvFunc_Object*>(obj);
  return my_obj->mVal;
}

// @brief TvFunc を表すオブジェクトの型定義を返す．
PyTypeObject*
PyTvFunc::_typeobject()
{
  return &TvFunc_Type;
}

END_NAMESPACE_YM
