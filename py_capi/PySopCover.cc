
/// @file PySopCover.cc
/// @brief PySopCover の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PySopCover.h"
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
struct SopCover_Object
{
  PyObject_HEAD
  SopCover mVal;
};

// Python 用のタイプ定義
PyTypeObject SopCover_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PySopCover::init() 中で初期化する．
};

// 終了関数
void
dealloc_func(
  PyObject* self
)
{
  auto obj = reinterpret_cast<SopCover_Object*>(self);
  obj->mVal.~SopCover();
  Py_TYPE(self)->tp_free(self);
}

PyObject*
nb_subtract(
  PyObject* self,
  PyObject* other
)
{
  try {
    if ( PySopCover::Check(self) ) {
      auto& val1 = PySopCover::_get_ref(self);
      if ( PySopCover::Check(other) ) {
        auto& val2 = PySopCover::_get_ref(other);
        return PySopCover::ToPyObject(val1 - val2);
      }
      if ( PySopCube::Check(other) ) {
        auto& val2 = PySopCube::_get_ref(other);
        return PySopCover::ToPyObject(val1 - val2);
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
nb_and(
  PyObject* self,
  PyObject* other
)
{
  try {
    if ( PySopCover::Check(self) ) {
      auto& val1 = PySopCover::_get_ref(self);
      if ( PySopCover::Check(other) ) {
        auto& val2 = PySopCover::_get_ref(other);
        return PySopCover::ToPyObject(val1 & val2);
      }
      if ( PySopCube::Check(other) ) {
        auto& val2 = PySopCube::_get_ref(other);
        return PySopCover::ToPyObject(val1 & val2);
      }
    }
    if ( PySopCover::Check(other) ) {
      auto& val2 = PySopCover::_get_ref(other);
      if ( PySopCube::Check(self) ) {
        auto& val1 = PySopCube::_get_ref(self);
        return PySopCover::ToPyObject(val1 & val2);
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
    if ( PySopCover::Check(self) ) {
      auto& val1 = PySopCover::_get_ref(self);
      if ( PySopCover::Check(other) ) {
        auto& val2 = PySopCover::_get_ref(other);
        return PySopCover::ToPyObject(val1 | val2);
      }
      if ( PySopCube::Check(other) ) {
        auto& val2 = PySopCube::_get_ref(other);
        return PySopCover::ToPyObject(val1 | val2);
      }
    }
    if ( PySopCover::Check(other) ) {
      auto& val2 = PySopCover::_get_ref(other);
      if ( PySopCube::Check(self) ) {
        auto& val1 = PySopCube::_get_ref(self);
        return PySopCover::ToPyObject(val1 | val2);
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
nb_inplace_subtract(
  PyObject* self,
  PyObject* other
)
{
  try {
    if ( PySopCover::Check(self) ) {
      auto& val1 = PySopCover::_get_ref(self);
      if ( PySopCover::Check(other) ) {
        auto& val2 = PySopCover::_get_ref(other);
        val1 -= val2;
        Py_XINCREF(self);
        return self;
      }
      if ( PySopCube::Check(other) ) {
        auto& val2 = PySopCube::_get_ref(other);
        val1 -= val2;
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
nb_inplace_and(
  PyObject* self,
  PyObject* other
)
{
  try {
    if ( PySopCover::Check(self) ) {
      auto& val1 = PySopCover::_get_ref(self);
      if ( PySopCover::Check(other) ) {
        auto& val2 = PySopCover::_get_ref(other);
        val1 &= val2;
        Py_XINCREF(self);
        return self;
      }
      if ( PySopCube::Check(other) ) {
        auto& val2 = PySopCube::_get_ref(other);
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
nb_inplace_or(
  PyObject* self,
  PyObject* other
)
{
  try {
    if ( PySopCover::Check(self) ) {
      auto& val1 = PySopCover::_get_ref(self);
      if ( PySopCover::Check(other) ) {
        auto& val2 = PySopCover::_get_ref(other);
        val1 |= val2;
        Py_XINCREF(self);
        return self;
      }
      if ( PySopCube::Check(other) ) {
        auto& val2 = PySopCube::_get_ref(other);
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

PyObject*
nb_true_divide(
  PyObject* self,
  PyObject* other
)
{
  try {
    if ( PySopCover::Check(self) ) {
      auto& val1 = PySopCover::_get_ref(self);
      if ( PySopCover::Check(other) ) {
        auto& val2 = PySopCover::_get_ref(other);
        return PySopCover::ToPyObject(val1.algdiv(val2));
      }
      if ( PySopCube::Check(other) ) {
        auto& val2 = PySopCube::_get_ref(other);
        return PySopCover::ToPyObject(val1.algdiv(val2));
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
    if ( PySopCover::Check(self) ) {
      auto& val1 = PySopCover::_get_ref(self);
      if ( PySopCover::Check(other) ) {
        auto& val2 = PySopCover::_get_ref(other);
        val1.algdiv_int(val2);
        Py_XINCREF(self);
        return self;
      }
      if ( PySopCube::Check(other) ) {
        auto& val2 = PySopCube::_get_ref(other);
        val1.algdiv_int(val2);
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
  .nb_subtract = nb_subtract,
  .nb_and = nb_and,
  .nb_or = nb_or,
  .nb_inplace_subtract = nb_inplace_subtract,
  .nb_inplace_and = nb_inplace_and,
  .nb_inplace_or = nb_inplace_or,
  .nb_floor_divide = nb_true_divide,
  .nb_inplace_true_divide = nb_inplace_true_divide
};

// hash 関数
Py_hash_t
hash_func(
  PyObject* self
)
{
  auto& val = PySopCover::_get_ref(self);
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
  auto& val = PySopCover::_get_ref(self);
  if ( PySopCover::Check(other) ) {
    auto& val2 = PySopCover::_get_ref(other);
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
  auto& val = PySopCover::_get_ref(self);
  return PySopCover::ToPyObject(val);
}

// return number of literals
PyObject*
literal_num(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "var",
    "inv",
    nullptr
  };
  PyObject* var_obj = nullptr;
  int inv_tmp = -1;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "|$Op",
                                    const_cast<char**>(kwlist),
                                    &var_obj,
                                    &inv_tmp) ) {
    return nullptr;
  }
  bool inv;
  if ( inv_tmp != -1 ) {
    inv = static_cast<bool>(inv_tmp);
  }
  auto& val = PySopCover::_get_ref(self);
  if ( var_obj == nullptr ) {
    if ( inv ) {
      PyErr_SetString(PyExc_TypeError, "'inv' shoud not be specified without 'var'");
      return nullptr;
    }
    return PyLong::ToPyObject(val.literal_num());
  }
  else if ( PyLiteral::Check(var_obj) ) {
    auto lit = PyLiteral::_get_ref(var_obj);
    if ( inv ) {
      lit = ~lit;
    }
    return PyLong::ToPyObject(val.literal_num(lit));
  }
  else {
    PyErr_SetString(PyExc_TypeError, "'var' must be an integer or a 'Literal'");
    return nullptr;
  }
}

// convert to list of list of literals
PyObject*
literal_list(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PySopCover::_get_ref(self);
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
    "cube_pos",
    "var_pos",
    nullptr
  };
  int cube_pos;
  int var_pos;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "ii",
                                    const_cast<char**>(kwlist),
                                    &cube_pos,
                                    &var_pos) ) {
    return nullptr;
  }
  auto& val = PySopCover::_get_ref(self);
  auto pat = val.get_pat(cube_pos, var_pos);
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

// return common cube
PyObject*
common_cube(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PySopCover::_get_ref(self);
  return PySopCube::ToPyObject(val.common_cube());
}

// convert to 'Expr'
PyObject*
expr(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PySopCover::_get_ref(self);
  return PyExpr::ToPyObject(val.expr());
}

// convert to 'TvFunc'
PyObject*
tvfunc(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PySopCover::_get_ref(self);
  return PyTvFunc::ToPyObject(val.tvfunc());
}

// メソッド定義
PyMethodDef methods[] = {
  {"copy",
   copy,
   METH_NOARGS,
   PyDoc_STR("make a copy")},
  {"literal_num",
   reinterpret_cast<PyCFunction>(literal_num),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return number of literals")},
  {"literal_list",
   literal_list,
   METH_NOARGS,
   PyDoc_STR("convert to list of list of literals")},
  {"get_pat",
   reinterpret_cast<PyCFunction>(get_pat),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("get pat")},
  {"common_cube",
   common_cube,
   METH_NOARGS,
   PyDoc_STR("return common cube")},
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
  auto& val = PySopCover::_get_ref(self);
  return PyLong::ToPyObject(val.variable_num());
}

PyObject*
get_cube_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PySopCover::_get_ref(self);
  return PyLong::ToPyObject(val.cube_num());
}

// getter/setter定義
PyGetSetDef getsets[] = {
  {"variable_num", get_variable_num, nullptr, PyDoc_STR(""), nullptr},
  {"cube_num", get_cube_num, nullptr, PyDoc_STR(""), nullptr},
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
    "cube_list",
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
  std::vector<SopCube> cube_list;
  if ( list_obj != nullptr ) {
    if ( !PyList<SopCube, PySopCube>::FromPyObject(list_obj, cube_list) ) {
      PyErr_SetString(PyExc_TypeError, "argument 2 should be a sequence of 'SopCube'");
      return nullptr;
    }
  }
  auto self = type->tp_alloc(type, 0);
  auto my_obj = reinterpret_cast<SopCover_Object*>(self);
  new (&my_obj->mVal) SopCover(ni, cube_list);
  return self;
}

END_NONAMESPACE


// @brief SopCover オブジェクトを使用可能にする．
bool
PySopCover::init(
  PyObject* m
)
{
  SopCover_Type.tp_name = "SopCover";
  SopCover_Type.tp_basicsize = sizeof(SopCover_Object);
  SopCover_Type.tp_itemsize = 0;
  SopCover_Type.tp_dealloc = dealloc_func;
  SopCover_Type.tp_as_number = &number;
  SopCover_Type.tp_hash = hash_func;
  SopCover_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  SopCover_Type.tp_doc = PyDoc_STR("Python extended object for SopCover");
  SopCover_Type.tp_richcompare = richcompare_func;
  SopCover_Type.tp_methods = methods;
  SopCover_Type.tp_getset = getsets;
  SopCover_Type.tp_new = new_func;
  if ( !PyModule::reg_type(m, "SopCover", &SopCover_Type) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// SopCover を PyObject に変換する．
PyObject*
PySopCover::Conv::operator()(
  const SopCover& val
)
{
  auto type = PySopCover::_typeobject();
  auto obj = type->tp_alloc(type, 0);
  auto my_obj = reinterpret_cast<SopCover_Object*>(obj);
  new (&my_obj->mVal) SopCover(val);
  return obj;
}

// PyObject を SopCover に変換する．
bool
PySopCover::Deconv::operator()(
  PyObject* obj,
  SopCover& val
)
{
  if ( PySopCover::Check(obj) ) {
    val = PySopCover::_get_ref(obj);
    return true;
  }
  return false;
}

// @brief PyObject が SopCover タイプか調べる．
bool
PySopCover::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, &SopCover_Type);
}

// @brief PyObject から SopCover を取り出す．
SopCover&
PySopCover::_get_ref(
  PyObject* obj
)
{
  auto my_obj = reinterpret_cast<SopCover_Object*>(obj);
  return my_obj->mVal;
}

// @brief SopCover を表すオブジェクトの型定義を返す．
PyTypeObject*
PySopCover::_typeobject()
{
  return &SopCover_Type;
}

END_NAMESPACE_YM
