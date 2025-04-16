
/// @file PySopCube.cc
/// @brief PySopCube の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PySopCube.h"
#include "pym/PyLiteral.h"
#include "pym/PyExpr.h"
#include "pym/PyTvFunc.h"
#include "pym/PyLong.h"
#include "pym/PyString.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
// この構造体は同じサイズのヒープから作られるので
// mVal のコンストラクタは起動されないことに注意．
// そのためあとでコンストラクタを明示的に起動する必要がある．
// またメモリを開放するときにも明示的にデストラクタを起動する必要がある．
struct SopCube_Object
{
  PyObject_HEAD
  SopCube mVal;
};

// Python 用のタイプ定義
PyTypeObject SopCube_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PySopCube::init() 中で初期化する．
};

// 終了関数
void
dealloc_func(
  PyObject* self
)
{
  auto obj = reinterpret_cast<SopCube_Object*>(self);
  obj->mVal.~SopCube();
  Py_TYPE(self)->tp_free(self);
}

PyObject*
nb_and(
  PyObject* self,
  PyObject* other
)
{
  try {
    if ( PySopCube::Check(self) ) {
      auto& val1 = PySopCube::_get_ref(self);
      if ( PySopCube::Check(other) ) {
        auto& val2 = PySopCube::_get_ref(other);
        return PySopCube::ToPyObject(val1 & val2);
      }
      if ( PyLiteral::Check(other) ) {
        auto& val2 = PyLiteral::_get_ref(other);
        return PySopCube::ToPyObject(val1 & val2);
      }
    }
    if ( PySopCube::Check(other) ) {
      auto& val2 = PySopCube::_get_ref(other);
      if ( PyLiteral::Check(self) ) {
        auto& val1 = PyLiteral::_get_ref(self);
        return PySopCube::ToPyObject(val1 & val2);
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
    if ( PySopCube::Check(self) ) {
      auto& val1 = PySopCube::_get_ref(self);
      if ( PySopCube::Check(other) ) {
        auto& val2 = PySopCube::_get_ref(other);
        val1 &= val2;
        Py_XINCREF(self);
        return self;
      }
      if ( PyLiteral::Check(other) ) {
        auto& val2 = PyLiteral::_get_ref(other);
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
nb_true_divide(
  PyObject* self,
  PyObject* other
)
{
  try {
    if ( PySopCube::Check(self) ) {
      auto& val1 = PySopCube::_get_ref(self);
      if ( PySopCube::Check(other) ) {
        auto& val2 = PySopCube::_get_ref(other);
        return PySopCube::ToPyObject(val1 / val2);
      }
      if ( PyLiteral::Check(other) ) {
        auto& val2 = PyLiteral::_get_ref(other);
        return PySopCube::ToPyObject(val1 / val2);
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
nb_inplace_true_divide(
  PyObject* self,
  PyObject* other
)
{
  try {
    if ( PySopCube::Check(self) ) {
      auto& val1 = PySopCube::_get_ref(self);
      if ( PySopCube::Check(other) ) {
        auto& val2 = PySopCube::_get_ref(other);
        val1 /= val2;
        Py_XINCREF(self);
        return self;
      }
      if ( PyLiteral::Check(other) ) {
        auto& val2 = PyLiteral::_get_ref(other);
        val1 /= val2;
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
  .nb_and = nb_and,
  .nb_inplace_and = nb_inplace_and,
  .nb_floor_divide = nb_true_divide,
  .nb_inplace_true_divide = nb_inplace_true_divide
};

// hash 関数
Py_hash_t
hash_func(
  PyObject* self
)
{
  auto& val = PySopCube::_get_ref(self);
  return val.hash();
}

// richcompare 関数
PyObject*
richcompare_func(
  PyObject* self,
  PyObject* other,
  int op
)
{
  auto& val = PySopCube::_get_ref(self);
  if ( PySopCube::Check(other) ) {
    auto& val2 = PySopCube::_get_ref(other);
    try {
      Py_RETURN_RICHCOMPARE(val, val2, op);
    }
    catch ( std::invalid_argument err ) {
      std::ostringstream buf;
      buf << "invalid argument" << ": " << err.what();
      PyErr_SetString(PyExc_ValueError, buf.str().c_str());
      return nullptr;
    }
  }
  Py_RETURN_NOTIMPLEMENTED;
}

// make a copy
PyObject*
copy(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PySopCube::_get_ref(self);
  return PySopCube::ToPyObject(val);
}

// True if valid
PyObject*
is_valid(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PySopCube::_get_ref(self);
  return PyBool_FromLong(val.is_valid());
}

// True if invalid
PyObject*
is_invalid(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PySopCube::_get_ref(self);
  return PyBool_FromLong(val.is_invalid());
}

// True if tautology
PyObject*
is_tautology(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PySopCube::_get_ref(self);
  return PyBool_FromLong(val.is_tautology());
}

// get pat
PyObject*
get_pat(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "pos",
    nullptr
  };
  int pos;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "i",
                                    const_cast<char**>(kwlist),
                                    &pos) ) {
    return nullptr;
  }
  auto& val = PySopCube::_get_ref(self);
  auto pat = val.get_pat(pos);
  const char* ans = nullptr;
  if ( pat == SopPat::_X ) {
    ans = "-";
  }
  else if ( pat == SopPat::_0 ) {
    ans = "0";
  }
  else if ( pat == SopPat::_1 ) {
    ans = "1";
  }
  return PyString::ToPyObject(ans);
}

// check if the specified literal is present
PyObject*
check_literal(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "literal",
    nullptr
  };
  PyObject* lit_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O!",
                                    const_cast<char**>(kwlist),
                                    PyLiteral::_typeobject(), &lit_obj) ) {
    return nullptr;
  }
  Literal lit;
  if ( lit_obj != nullptr ) {
    if ( !PyLiteral::FromPyObject(lit_obj, lit) ) {
      PyErr_SetString(PyExc_TypeError, "could not convert to Literal");
      return nullptr;
    }
  }
  auto& val = PySopCube::_get_ref(self);
  return PyBool_FromLong(val.check_literal(lit));
}

// return a list of literals
PyObject*
literal_list(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PySopCube::_get_ref(self);
  return PyList<Literal, PyLiteral>::ToPyObject(val.literal_list());
}

// check containment
PyObject*
check_containment(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "cube",
    nullptr
  };
  PyObject* obj1 = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O!",
                                    const_cast<char**>(kwlist),
                                    PySopCube::_typeobject(), &obj1) ) {
    return nullptr;
  }
  auto& val = PySopCube::_get_ref(self);
  auto& val1 = PySopCube::_get_ref(obj1);
  if ( val.variable_num() != val1.variable_num() ) {
    PyErr_SetString(PyExc_ValueError, "'variable_num()' mismatch");
    return nullptr;
  }
  return PyBool_FromLong(val.check_containment(val1));
}

// convert to 'Expr'
PyObject*
expr(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PySopCube::_get_ref(self);
  return PyExpr::ToPyObject(val.expr());
}

// convert to 'TvFunc'
PyObject*
tvfunc(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PySopCube::_get_ref(self);
  return PyTvFunc::ToPyObject(val.tvfunc());
}

// メソッド定義
PyMethodDef methods[] = {
  {"copy",
   copy,
   METH_NOARGS,
   PyDoc_STR("make a copy")},
  {"is_valid",
   is_valid,
   METH_NOARGS,
   PyDoc_STR("True if valid")},
  {"is_invalid",
   is_invalid,
   METH_NOARGS,
   PyDoc_STR("True if invalid")},
  {"is_tautology",
   is_tautology,
   METH_NOARGS,
   PyDoc_STR("True if tautology")},
  {"get_pat",
   reinterpret_cast<PyCFunction>(get_pat),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("get pat")},
  {"check_literal",
   reinterpret_cast<PyCFunction>(check_literal),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("check if the specified literal is present")},
  {"literal_list",
   literal_list,
   METH_NOARGS,
   PyDoc_STR("return a list of literals")},
  {"check_containment",
   reinterpret_cast<PyCFunction>(check_containment),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("check containment")},
  {"expr",
   expr,
   METH_NOARGS,
   PyDoc_STR("convert to 'Expr'")},
  {"tvfunc",
   tvfunc,
   METH_NOARGS,
   PyDoc_STR("convert to 'TvFunc'")},
  // end-marker
  {nullptr, nullptr, 0, nullptr}
};

PyObject*
get_variable_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PySopCube::_get_ref(self);
  return PyLong::ToPyObject(val.variable_num());
}

PyObject*
get_literal_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PySopCube::_get_ref(self);
  return PyLong::ToPyObject(val.literal_num());
}

// getter/setter定義
PyGetSetDef getsets[] = {
  {"variable_num", get_variable_num, nullptr, PyDoc_STR(""), nullptr},
  {"literal_num", get_literal_num, nullptr, PyDoc_STR(""), nullptr},
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
    "input_num",
    "literal_list",
    nullptr
  };
  int ni;
  PyObject* list_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "i|$O",
                                    const_cast<char**>(kwlist),
                                    &ni,
                                    &list_obj) ) {
    return nullptr;
  }
  std::vector<Literal> lit_list;
  if ( list_obj != nullptr ) {
    if ( !PyList<Literal, PyLiteral>::FromPyObject(list_obj, lit_list) ) {
      PyErr_SetString(PyExc_TypeError, "argument 2 should be a sequence of 'Literal'");
      return nullptr;
    }
  }
  auto self = type->tp_alloc(type, 0);
  auto my_obj = reinterpret_cast<SopCube_Object*>(self);
  new (&my_obj->mVal) SopCube(ni, lit_list);
  return self;
}

END_NONAMESPACE


// @brief SopCube オブジェクトを使用可能にする．
bool
PySopCube::init(
  PyObject* m
)
{
  SopCube_Type.tp_name = "SopCube";
  SopCube_Type.tp_basicsize = sizeof(SopCube_Object);
  SopCube_Type.tp_itemsize = 0;
  SopCube_Type.tp_dealloc = dealloc_func;
  SopCube_Type.tp_as_number = &number;
  SopCube_Type.tp_hash = hash_func;
  SopCube_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  SopCube_Type.tp_doc = PyDoc_STR("Python extended object for SopCube");
  SopCube_Type.tp_richcompare = richcompare_func;
  SopCube_Type.tp_methods = methods;
  SopCube_Type.tp_getset = getsets;
  SopCube_Type.tp_new = new_func;
  if ( !PyModule::reg_type(m, "SopCube", &SopCube_Type) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// SopCube を PyObject に変換する．
PyObject*
PySopCube::Conv::operator()(
  const SopCube& val
)
{
  auto type = PySopCube::_typeobject();
  auto obj = type->tp_alloc(type, 0);
  auto my_obj = reinterpret_cast<SopCube_Object*>(obj);
  new (&my_obj->mVal) SopCube(val);
  return obj;
}

// PyObject を SopCube に変換する．
bool
PySopCube::Deconv::operator()(
  PyObject* obj,
  SopCube& val
)
{
  if ( PySopCube::Check(obj) ) {
    val = PySopCube::_get_ref(obj);
    return true;
  }
  return false;
}

// @brief PyObject が SopCube タイプか調べる．
bool
PySopCube::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, &SopCube_Type);
}

// @brief PyObject から SopCube を取り出す．
SopCube&
PySopCube::_get_ref(
  PyObject* obj
)
{
  auto my_obj = reinterpret_cast<SopCube_Object*>(obj);
  return my_obj->mVal;
}

// @brief SopCube を表すオブジェクトの型定義を返す．
PyTypeObject*
PySopCube::_typeobject()
{
  return &SopCube_Type;
}

END_NAMESPACE_YM
