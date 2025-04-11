
/// @file PyExpr.cc
/// @brief PyExpr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyExpr.h"
#include "pym/PyTvFunc.h"
#include "pym/PyLiteral.h"
#include "pym/PyPrimType.h"
#include "pym/PyLong.h"
#include "pym/PyUlong.h"
#include "pym/PyString.h"
#include "ym/TvFunc.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
// この構造体は同じサイズのヒープから作られるので
// mVal のコンストラクタは起動されないことに注意．
// そのためあとでコンストラクタを明示的に起動する必要がある．
// またメモリを開放するときにも明示的にデストラクタを起動する必要がある．
struct Expr_Object
{
  PyObject_HEAD
  Expr mVal;
};

// Python 用のタイプ定義
PyTypeObject Expr_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PyExpr::init() 中で初期化する．
};

// 終了関数
void
dealloc_func(
  PyObject* self
)
{
  auto obj = reinterpret_cast<Expr_Object*>(self);
  obj->mVal.~Expr();
  Py_TYPE(self)->tp_free(self);
}

// repr 関数
PyObject*
repr_func(
  PyObject* self
)
{
  auto& val = PyExpr::_get_ref(self);
  auto str_val = val.rep_string();
  return PyString::ToPyObject(str_val);
}

PyObject*
nb_invert(
  PyObject* self
)
{
  auto& val = PyExpr::_get_ref(self);
  return PyExpr::ToPyObject(~val);
}

PyObject*
nb_and(
  PyObject* self,
  PyObject* other
)
{
  if ( PyExpr::Check(self) ) {
    auto& val1 = PyExpr::_get_ref(self);
    if ( PyExpr::Check(other) ) {
      auto& val2 = PyExpr::_get_ref(other);
      return PyExpr::ToPyObject(val1 & val2);
    }
  }
  Py_RETURN_NOTIMPLEMENTED;
}

PyObject*
nb_xor(
  PyObject* self,
  PyObject* other
)
{
  if ( PyExpr::Check(self) ) {
    auto& val1 = PyExpr::_get_ref(self);
    if ( PyExpr::Check(other) ) {
      auto& val2 = PyExpr::_get_ref(other);
      return PyExpr::ToPyObject(val1 ^ val2);
    }
  }
  Py_RETURN_NOTIMPLEMENTED;
}

PyObject*
nb_or(
  PyObject* self,
  PyObject* other
)
{
  if ( PyExpr::Check(self) ) {
    auto& val1 = PyExpr::_get_ref(self);
    if ( PyExpr::Check(other) ) {
      auto& val2 = PyExpr::_get_ref(other);
      return PyExpr::ToPyObject(val1 | val2);
    }
  }
  Py_RETURN_NOTIMPLEMENTED;
}

PyObject*
nb_inplace_and(
  PyObject* self,
  PyObject* other
)
{
  if ( PyExpr::Check(self) ) {
    auto& val1 = PyExpr::_get_ref(self);
    if ( PyExpr::Check(other) ) {
      auto& val2 = PyExpr::_get_ref(other);
      val1 &= val2;
      Py_XINCREF(self);
      return self;
    }
  }
  Py_RETURN_NOTIMPLEMENTED;
}

PyObject*
nb_inplace_xor(
  PyObject* self,
  PyObject* other
)
{
  if ( PyExpr::Check(self) ) {
    auto& val1 = PyExpr::_get_ref(self);
    if ( PyExpr::Check(other) ) {
      auto& val2 = PyExpr::_get_ref(other);
      val1 ^= val2;
      Py_XINCREF(self);
      return self;
    }
  }
  Py_RETURN_NOTIMPLEMENTED;
}

PyObject*
nb_inplace_or(
  PyObject* self,
  PyObject* other
)
{
  if ( PyExpr::Check(self) ) {
    auto& val1 = PyExpr::_get_ref(self);
    if ( PyExpr::Check(other) ) {
      auto& val2 = PyExpr::_get_ref(other);
      val1 |= val2;
      Py_XINCREF(self);
      return self;
    }
  }
  Py_RETURN_NOTIMPLEMENTED;
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

// str 関数
PyObject*
str_func(
  PyObject* self
)
{
  auto& val = PyExpr::_get_ref(self);
  auto str_val = val.to_string();
  return PyString::ToPyObject(str_val);
}

// richcompare 関数
PyObject*
richcompare_func(
  PyObject* self,
  PyObject* other,
  int op
)
{
  auto& val = PyExpr::_get_ref(self);
  if ( PyExpr::Check(other) ) {
    auto& val2 = PyExpr::_get_ref(other);
    if ( op == Py_EQ ) {
      return PyBool_FromLong(val == val2);
    }
    if ( op == Py_NE ) {
      return PyBool_FromLong(val != val2);
    }
  }
  Py_RETURN_NOTIMPLEMENTED;
}

// make an invalid object
PyObject*
invalid(
  PyObject* Py_UNUSED(self),
  PyObject* Py_UNUSED(args)
)
{
  return PyExpr::ToPyObject(Expr::invalid());
}

// make a ZERO expression
PyObject*
zero(
  PyObject* Py_UNUSED(self),
  PyObject* Py_UNUSED(args)
)
{
  return PyExpr::ToPyObject(Expr::zero());
}

// make a ONE expression
PyObject*
one(
  PyObject* Py_UNUSED(self),
  PyObject* Py_UNUSED(args)
)
{
  return PyExpr::ToPyObject(Expr::one());
}

// make a literal object
PyObject*
literal(
  PyObject* Py_UNUSED(self),
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "",
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
  Literal lit = Literal::x();
  if ( lit_obj != nullptr ) {
    if ( !PyLiteral::FromPyObject(lit_obj, lit) ) {
      PyErr_SetString(PyExc_ValueError, "could not convert to Literal");
      return nullptr;
    }
  }
  bool inv = false;
  if ( inv_tmp != -1 ) {
    inv = static_cast<bool>(inv_tmp);
  }
  if ( inv ) {
    lit = ~lit;
  }
  return PyExpr::ToPyObject(Expr::literal(lit));
}

// make a positive literal object
PyObject*
posi_literal(
  PyObject* Py_UNUSED(self),
  PyObject* args
)
{
  unsigned long varid;
  if ( !PyArg_ParseTuple(args, "k",
                         &varid) ) {
    return nullptr;
  }
  return PyExpr::ToPyObject(Expr::posi_literal(varid));
}

// make a negative literal object
PyObject*
nega_literal(
  PyObject* Py_UNUSED(self),
  PyObject* args
)
{
  unsigned long varid;
  if ( !PyArg_ParseTuple(args, "k",
                         &varid) ) {
    return nullptr;
  }
  return PyExpr::ToPyObject(Expr::nega_literal(varid));
}

// AND op
PyObject*
and_op(
  PyObject* Py_UNUSED(self),
  PyObject* args
)
{
  PyObject* obj = nullptr;
  if ( !PyArg_ParseTuple(args, "O",
                         &obj) ) {
    return nullptr;
  }
  std::vector<Expr> expr_list;
  if ( !PyList<Expr, PyExpr>::FromPyObject(obj, expr_list) ) {
    PyErr_SetString(PyExc_TypeError, "argument 1 must be a list of 'Expr's");
    return nullptr;
  }
  return PyExpr::ToPyObject(Expr::and_op(expr_list));
}

// OR op
PyObject*
or_op(
  PyObject* Py_UNUSED(self),
  PyObject* args
)
{
  PyObject* obj = nullptr;
  if ( !PyArg_ParseTuple(args, "O",
                         &obj) ) {
    return nullptr;
  }
  std::vector<Expr> expr_list;
  if ( !PyList<Expr, PyExpr>::FromPyObject(obj, expr_list) ) {
    PyErr_SetString(PyExc_TypeError, "argument 1 must be a list of 'Expr's");
    return nullptr;
  }
  return PyExpr::ToPyObject(Expr::or_op(expr_list));
}

// XOR op
PyObject*
xor_op(
  PyObject* Py_UNUSED(self),
  PyObject* args
)
{
  PyObject* obj = nullptr;
  if ( !PyArg_ParseTuple(args, "O",
                         &obj) ) {
    return nullptr;
  }
  std::vector<Expr> expr_list;
  if ( !PyList<Expr, PyExpr>::FromPyObject(obj, expr_list) ) {
    PyErr_SetString(PyExc_TypeError, "argument 1 must be a list of 'Expr's");
    return nullptr;
  }
  return PyExpr::ToPyObject(Expr::xor_op(expr_list));
}

// make an Expr object from string
PyObject*
from_string(
  PyObject* Py_UNUSED(self),
  PyObject* args
)
{
  const char* expr_str_tmp = nullptr;
  if ( !PyArg_ParseTuple(args, "s",
                         &expr_str_tmp) ) {
    return nullptr;
  }
  std::string expr_str;
  if ( expr_str_tmp != nullptr ) {
    expr_str = std::string(expr_str_tmp);
  }
  try {
    return PyExpr::ToPyObject(Expr::from_string(expr_str));
  }
  catch ( std::invalid_argument err ) {
    std::ostringstream buf;
    buf << "syntax error in argument string" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// COMPOSE op
PyObject*
compose(
  PyObject* self,
  PyObject* args
)
{
  PyObject* dict_obj = nullptr;
  if ( !PyArg_ParseTuple(args, "O",
                         &dict_obj) ) {
    return nullptr;
  }
  auto& val = PyExpr::_get_ref(self);
  if ( !PyDict_Check(dict_obj) ) {
    PyErr_SetString(PyExc_TypeError, "argument 1 must be a dictonary object");
    return nullptr;
  }
  std::unordered_map<SizeType, Expr> comp_map;
  PyObject* key;
  PyObject* value;
  Py_ssize_t pos = 0;
  while ( PyDict_Next(dict_obj, &pos, &key, &value) ) {
    auto id = PyLong::Get(key);
    if ( id == -1 && PyErr_Occurred() ) {
      return nullptr;
    }
    if ( !PyExpr::Check(value) ) {
      PyErr_SetString(PyExc_TypeError, "dictionary value must be an 'Expr' type");
      return nullptr;
    }
    auto& expr1 = PyExpr::_get_ref(value);
    comp_map.emplace(static_cast<SizeType>(id), expr1);
  }
  return PyExpr::ToPyObject(val.compose(comp_map));
}

// remap variable assignment
PyObject*
remap_var(
  PyObject* self,
  PyObject* args
)
{
  PyObject* dict_obj = nullptr;
  if ( !PyArg_ParseTuple(args, "O",
                         &dict_obj) ) {
    return nullptr;
  }
  auto& val = PyExpr::_get_ref(self);
  if ( !PyDict_Check(dict_obj) ) {
    PyErr_SetString(PyExc_TypeError, "argument 1 must be a dictonary object");
    return nullptr;
  }
  std::unordered_map<SizeType, SizeType> comp_map;
  PyObject* key;
  PyObject* value;
  Py_ssize_t pos = 0;
  while ( PyDict_Next(dict_obj, &pos, &key, &value) ) {
    auto id = PyLong::Get(key);
    if ( id == -1 && PyErr_Occurred() ) {
      return nullptr;
    }
    auto val = PyLong::Get(value);
    if ( val == -1 && PyErr_Occurred() ) {
      return nullptr;
    }
    comp_map.emplace(static_cast<SizeType>(id), static_cast<SizeType>(val));
  }
  return PyExpr::ToPyObject(val.remap_var(comp_map));
}

// simplify
PyObject*
simplify(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyExpr::_get_ref(self);
  val.simplify();
  Py_RETURN_NONE;
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
    "",
    "mask",
    nullptr
  };
  PyObject* vect_obj = nullptr;
  unsigned long mask = ~0UL;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O|$k",
                                    const_cast<char**>(kwlist),
                                    &vect_obj,
                                    &mask) ) {
    return nullptr;
  }
  auto& val = PyExpr::_get_ref(self);
  std::vector<unsigned long> vals;
  if ( !PyList<unsigned long, PyUlong>::FromPyObject(vect_obj, vals) ) {
    PyErr_SetString(PyExc_TypeError, "argument 1 must be a vector of integer");
    return nullptr;
  }
  auto ans = val.eval(vals, static_cast<Expr::BitVectType>(mask));
  return PyLong::ToPyObject(ans);
}

// convert to TvFunc
PyObject*
tvfunc(
  PyObject* self,
  PyObject* args
)
{
  unsigned long ni = 0;
  if ( !PyArg_ParseTuple(args, "|k",
                         &ni) ) {
    return nullptr;
  }
  auto& val = PyExpr::_get_ref(self);
  return PyTvFunc::ToPyObject(val.tvfunc(ni));
}

// check if valid
PyObject*
is_valid(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyExpr::_get_ref(self);
  return PyBool_FromLong(val.is_valid());
}

// check if invalid
PyObject*
is_invalid(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyExpr::_get_ref(self);
  return PyBool_FromLong(val.is_invalid());
}

// check if zero
PyObject*
is_zero(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyExpr::_get_ref(self);
  return PyBool_FromLong(val.is_zero());
}

// check if one
PyObject*
is_one(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyExpr::_get_ref(self);
  return PyBool_FromLong(val.is_one());
}

// check if constant
PyObject*
is_constant(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyExpr::_get_ref(self);
  return PyBool_FromLong(val.is_constant());
}

// check if positive literal
PyObject*
is_posi_literal(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyExpr::_get_ref(self);
  return PyBool_FromLong(val.is_posi_literal());
}

// check if negative Literal
PyObject*
is_nega_literal(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyExpr::_get_ref(self);
  return PyBool_FromLong(val.is_nega_literal());
}

// check if Literal type
PyObject*
is_literal(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyExpr::_get_ref(self);
  return PyBool_FromLong(val.is_literal());
}

// check if AND type
PyObject*
is_and(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyExpr::_get_ref(self);
  return PyBool_FromLong(val.is_and());
}

// check if OR type
PyObject*
is_or(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyExpr::_get_ref(self);
  return PyBool_FromLong(val.is_or());
}

// check if XOR type
PyObject*
is_xor(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyExpr::_get_ref(self);
  return PyBool_FromLong(val.is_xor());
}

// check if OP type
PyObject*
is_op(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyExpr::_get_ref(self);
  return PyBool_FromLong(val.is_op());
}

// check if simple type
PyObject*
is_simple(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyExpr::_get_ref(self);
  return PyBool_FromLong(val.is_simple());
}

// check if simple AND type
PyObject*
is_simple_and(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyExpr::_get_ref(self);
  return PyBool_FromLong(val.is_simple_and());
}

// check if simple OR type
PyObject*
is_simple_or(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyExpr::_get_ref(self);
  return PyBool_FromLong(val.is_simple_or());
}

// check if simple XOR type
PyObject*
is_simple_xor(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyExpr::_get_ref(self);
  return PyBool_FromLong(val.is_simple_xor());
}

// check if SOP type
PyObject*
is_sop(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyExpr::_get_ref(self);
  return PyBool_FromLong(val.is_sop());
}

// analyze type of root node
PyObject*
analyze(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyExpr::_get_ref(self);
  return PyPrimType::ToPyObject(val.analyze());
}

// get varid
PyObject*
get_varid(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyExpr::_get_ref(self);
  return PyLong::ToPyObject(val.varid());
}

// get literal
PyObject*
get_literal(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyExpr::_get_ref(self);
  return PyLiteral::ToPyObject(val.literal());
}

// get operand list
PyObject*
get_operand_list(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyExpr::_get_ref(self);
  return PyList<Expr, PyExpr>::ToPyObject(val.operand_list());
}

// get literal num
PyObject*
literal_num(
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
  PyObject* lit_obj = nullptr;
  int inv_tmp = -1;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "|O$p",
                                    const_cast<char**>(kwlist),
                                    &lit_obj,
                                    &inv_tmp) ) {
    return nullptr;
  }
  bool inv = false;
  if ( inv_tmp != -1 ) {
    inv = static_cast<bool>(inv_tmp);
  }
  auto& val = PyExpr::_get_ref(self);
  Literal lit;
  SizeType ans;
  if ( lit_obj == nullptr ) {
    if ( inv ) {
      PyErr_SetString(PyExc_ValueError, "'inv' should not be specified without 'var'");
      return nullptr;
    }
    ans = val.literal_num();
  }
  else if ( PyLiteral::FromPyObject(lit_obj, lit) ) {
    if ( inv ) {
      lit = ~lit;
    }
    ans = val.literal_num(lit);
  }
  else {
    PyErr_SetString(PyExc_TypeError, "argument 1 must be an integer or a literal");
    return nullptr;
  }
  return PyLong::ToPyObject(ans);
}

// get literal num in SOP form
PyObject*
sop_literal_num(
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
  PyObject* lit_obj = nullptr;
  int inv_tmp = -1;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "|O$p",
                                    const_cast<char**>(kwlist),
                                    &lit_obj,
                                    &inv_tmp) ) {
    return nullptr;
  }
  bool inv = false;
  if ( inv_tmp != -1 ) {
    inv = static_cast<bool>(inv_tmp);
  }
  auto& val = PyExpr::_get_ref(self);
  Literal lit;
  SizeType ans;
  if ( lit_obj == nullptr ) {
    if ( inv ) {
      PyErr_SetString(PyExc_ValueError, "'inv' should not be specified without 'var'");
      return nullptr;
    }
    ans = val.sop_literal_num();
  }
  else if ( PyLiteral::FromPyObject(lit_obj, lit) ) {
    if ( inv ) {
      lit = ~lit;
    }
    ans = val.sop_literal_num(lit);
  }
  else {
    PyErr_SetString(PyExc_TypeError, "argument 1 must be an int or a literal");
    return nullptr;
  }
  return PyLong::ToPyObject(ans);
}

// get cube num in SOP form
PyObject*
sop_cube_num(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyExpr::_get_ref(self);
  auto ans = val.sop_cube_num();
  return PyLong::ToPyObject(ans);
}

// メソッド定義
PyMethodDef methods[] = {
  {"invalid",
   invalid,
   METH_NOARGS | METH_STATIC,
   PyDoc_STR("make an invalid object")},
  {"zero",
   zero,
   METH_NOARGS | METH_STATIC,
   PyDoc_STR("make a ZERO expression")},
  {"one",
   one,
   METH_NOARGS | METH_STATIC,
   PyDoc_STR("make a ONE expression")},
  {"literal",
   reinterpret_cast<PyCFunction>(literal),
   METH_VARARGS | METH_KEYWORDS | METH_STATIC,
   PyDoc_STR("make a literal object")},
  {"posi_literal",
   posi_literal,
   METH_VARARGS | METH_STATIC,
   PyDoc_STR("make a positive literal object")},
  {"nega_literal",
   nega_literal,
   METH_VARARGS | METH_STATIC,
   PyDoc_STR("make a negative literal object")},
  {"and_op",
   and_op,
   METH_VARARGS | METH_STATIC,
   PyDoc_STR("AND op")},
  {"or_op",
   or_op,
   METH_VARARGS | METH_STATIC,
   PyDoc_STR("OR op")},
  {"xor_op",
   xor_op,
   METH_VARARGS | METH_STATIC,
   PyDoc_STR("XOR op")},
  {"from_string",
   from_string,
   METH_VARARGS | METH_STATIC,
   PyDoc_STR("make an Expr object from string")},
  {"compose",
   compose,
   METH_VARARGS,
   PyDoc_STR("COMPOSE op")},
  {"remap_var",
   remap_var,
   METH_VARARGS,
   PyDoc_STR("remap variable assignment")},
  {"simplify",
   simplify,
   METH_NOARGS,
   PyDoc_STR("simplify")},
  {"eval",
   reinterpret_cast<PyCFunction>(eval),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("evaluate input vector")},
  {"tvfunc",
   tvfunc,
   METH_VARARGS,
   PyDoc_STR("convert to TvFunc")},
  {"is_valid",
   is_valid,
   METH_NOARGS,
   PyDoc_STR("check if valid")},
  {"is_invalid",
   is_invalid,
   METH_NOARGS,
   PyDoc_STR("check if invalid")},
  {"is_zero",
   is_zero,
   METH_NOARGS,
   PyDoc_STR("check if zero")},
  {"is_one",
   is_one,
   METH_NOARGS,
   PyDoc_STR("check if one")},
  {"is_constant",
   is_constant,
   METH_NOARGS,
   PyDoc_STR("check if constant")},
  {"is_posi_literal",
   is_posi_literal,
   METH_NOARGS,
   PyDoc_STR("check if positive literal")},
  {"is_nega_literal",
   is_nega_literal,
   METH_NOARGS,
   PyDoc_STR("check if negative Literal")},
  {"is_literal",
   is_literal,
   METH_NOARGS,
   PyDoc_STR("check if Literal type")},
  {"is_and",
   is_and,
   METH_NOARGS,
   PyDoc_STR("check if AND type")},
  {"is_or",
   is_or,
   METH_NOARGS,
   PyDoc_STR("check if OR type")},
  {"is_xor",
   is_xor,
   METH_NOARGS,
   PyDoc_STR("check if XOR type")},
  {"is_op",
   is_op,
   METH_NOARGS,
   PyDoc_STR("check if OP type")},
  {"is_simple",
   is_simple,
   METH_NOARGS,
   PyDoc_STR("check if simple type")},
  {"is_simple_and",
   is_simple_and,
   METH_NOARGS,
   PyDoc_STR("check if simple AND type")},
  {"is_simple_or",
   is_simple_or,
   METH_NOARGS,
   PyDoc_STR("check if simple OR type")},
  {"is_simple_xor",
   is_simple_xor,
   METH_NOARGS,
   PyDoc_STR("check if simple XOR type")},
  {"is_sop",
   is_sop,
   METH_NOARGS,
   PyDoc_STR("check if SOP type")},
  {"analyze",
   analyze,
   METH_NOARGS,
   PyDoc_STR("analyze type of root node")},
  {"get_varid",
   get_varid,
   METH_NOARGS,
   PyDoc_STR("get varid")},
  {"get_literal",
   get_literal,
   METH_NOARGS,
   PyDoc_STR("get literal")},
  {"get_operand_list",
   get_operand_list,
   METH_NOARGS,
   PyDoc_STR("get operand list")},
  {"literal_num",
   reinterpret_cast<PyCFunction>(literal_num),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("get literal num")},
  {"sop_literal_num",
   reinterpret_cast<PyCFunction>(sop_literal_num),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("get literal num in SOP form")},
  {"sop_cube_num",
   sop_cube_num,
   METH_NOARGS,
   PyDoc_STR("get cube num in SOP form")},
  // end-marker
  {nullptr, nullptr, 0, nullptr}
};

PyObject*
input_size(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyExpr::_get_ref(self);
  auto ans = val.input_size();
  return PyLong::ToPyObject(ans);
}

// getter/setter定義
PyGetSetDef getsets[] = {
  {"input_size", input_size, nullptr, PyDoc_STR(""), nullptr},
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
    "",
    nullptr
  };
  const char* expr_str_tmp = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "|s",
                                    const_cast<char**>(kwlist),
                                    &expr_str_tmp) ) {
    return nullptr;
  }
  std::string expr_str;
  if ( expr_str_tmp != nullptr ) {
    expr_str = std::string(expr_str_tmp);
  }
  auto self = type->tp_alloc(type, 0);
  auto my_obj = reinterpret_cast<Expr_Object*>(self);
  Expr src;
  if ( expr_str != string{} ) {
    try {
      src = Expr::from_rep_string(expr_str);
    }
    catch ( std::invalid_argument err ) {
      std::ostringstream buf;
      buf << "invalid argument" << ": " << err.what();
      PyErr_SetString(PyExc_ValueError, buf.str().c_str());
      return nullptr;
    }
  }
  new (&my_obj->mVal) Expr(src);
  return self;
}

END_NONAMESPACE


// @brief Expr オブジェクトを使用可能にする．
bool
PyExpr::init(
  PyObject* m
)
{
  Expr_Type.tp_name = "Expr";
  Expr_Type.tp_basicsize = sizeof(Expr_Object);
  Expr_Type.tp_itemsize = 0;
  Expr_Type.tp_dealloc = dealloc_func;
  Expr_Type.tp_repr = repr_func;
  Expr_Type.tp_as_number = &number;
  Expr_Type.tp_str = str_func;
  Expr_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  Expr_Type.tp_doc = PyDoc_STR("Python extended object for Expr");
  Expr_Type.tp_richcompare = richcompare_func;
  Expr_Type.tp_methods = methods;
  Expr_Type.tp_getset = getsets;
  Expr_Type.tp_new = new_func;
  if ( !PyModule::reg_type(m, "Expr", &Expr_Type) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// Expr を PyObject に変換する．
PyObject*
PyExpr::Conv::operator()(
  const Expr& val
)
{
  auto type = PyExpr::_typeobject();
  auto obj = type->tp_alloc(type, 0);
  auto my_obj = reinterpret_cast<Expr_Object*>(obj);
  new (&my_obj->mVal) Expr(val);
  return obj;
}

// PyObject を Expr に変換する．
bool
PyExpr::Deconv::operator()(
  PyObject* obj,
  Expr& val
)
{
  if ( PyExpr::Check(obj) ) {
    val = PyExpr::_get_ref(obj);
    return true;
  }
  return false;
}

// @brief PyObject が Expr タイプか調べる．
bool
PyExpr::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, &Expr_Type);
}

// @brief PyObject から Expr を取り出す．
Expr&
PyExpr::_get_ref(
  PyObject* obj
)
{
  auto my_obj = reinterpret_cast<Expr_Object*>(obj);
  return my_obj->mVal;
}

// @brief Expr を表すオブジェクトの型定義を返す．
PyTypeObject*
PyExpr::_typeobject()
{
  return &Expr_Type;
}

END_NAMESPACE_YM
