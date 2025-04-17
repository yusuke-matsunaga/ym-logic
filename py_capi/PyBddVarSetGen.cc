
/// @file PyBddVarSet.cc
/// @brief PyBddVarSet の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyBddVarSet.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
// この構造体は同じサイズのヒープから作られるので
// mVal のコンストラクタは起動されないことに注意．
// そのためあとでコンストラクタを明示的に起動する必要がある．
// またメモリを開放するときにも明示的にデストラクタを起動する必要がある．
struct PyBddVarSet_Object
{
  PyObject_HEAD
  BddVarSet mVal;
};

// Python 用のタイプ定義
PyTypeObject PyBddVarSet_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PyBddVarSet::init() 中で初期化する．
};

// 終了関数
void
dealloc_func(
  PyObject* self
)
{
  auto obj = reinterpret_cast<PyBddVarSet_Object*>(self);
  obj->mVal.~BddVarSet();
  Py_TYPE(self)->tp_free(self);
}

PyObject*
nb_add(
  PyObject* self,
  PyObject* other
)
{
  try {
    if ( PyBddVarSet::Check(self) ) {
      auto& val1 = PyBddVarSet::_get_ref(self);
      if ( PyBddVarSet::Check(other) ) {
        auto& val2 = PyBddVarSet::_get_ref(other);
        return PyBddVarSet::ToPyObject(val1 + val2);
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
nb_subtract(
  PyObject* self,
  PyObject* other
)
{
  try {
    if ( PyBddVarSet::Check(self) ) {
      auto& val1 = PyBddVarSet::_get_ref(self);
      if ( PyBddVarSet::Check(other) ) {
        auto& val2 = PyBddVarSet::_get_ref(other);
        return PyBddVarSet::ToPyObject(val1 - val2);
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
    if ( PyBddVarSet::Check(self) ) {
      auto& val1 = PyBddVarSet::_get_ref(self);
      if ( PyBddVarSet::Check(other) ) {
        auto& val2 = PyBddVarSet::_get_ref(other);
        return PyBddVarSet::ToPyObject(val1 & val2);
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
nb_inplace_add(
  PyObject* self,
  PyObject* other
)
{
  try {
    if ( PyBddVarSet::Check(self) ) {
      auto& val1 = PyBddVarSet::_get_ref(self);
      if ( PyBddVarSet::Check(other) ) {
        auto& val2 = PyBddVarSet::_get_ref(other);
        val1 += val2;
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
nb_inplace_subtract(
  PyObject* self,
  PyObject* other
)
{
  try {
    if ( PyBddVarSet::Check(self) ) {
      auto& val1 = PyBddVarSet::_get_ref(self);
      if ( PyBddVarSet::Check(other) ) {
        auto& val2 = PyBddVarSet::_get_ref(other);
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
    if ( PyBddVarSet::Check(self) ) {
      auto& val1 = PyBddVarSet::_get_ref(self);
      if ( PyBddVarSet::Check(other) ) {
        auto& val2 = PyBddVarSet::_get_ref(other);
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

// Numberオブジェクト構造体
PyNumberMethods number = {
  .nb_add = nb_add,
  .nb_subtract = nb_subtract,
  .nb_and = nb_and,
  .nb_inplace_add = nb_inplace_add,
  .nb_inplace_subtract = nb_inplace_subtract,
  .nb_inplace_and = nb_inplace_and
};

// richcompare 関数
PyObject*
richcompare_func(
  PyObject* self,
  PyObject* other,
  int op
)
{
  auto& val = PyBddVarSet::_get_ref(self);
  try {
    if ( PyBddVarSet::Check(other) ) {
      auto& val2 = PyBddVarSet::_get_ref(other);
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

// return size
PyObject*
size(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyBddVarSet::_get_ref(self);
  return PyLong::ToPyObject(val.size());
}

// convert to list of 'BddVar's
PyObject*
to_varlist(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyBddVarSet::_get_ref(self);
  return PyList<BddVar, PyBddVar>::ToPyObject(val.to_varlist());
}

// return top variable
PyObject*
top_var(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyBddVarSet::_get_ref(self);
  return PyBddVar::ToPyObject(val.top_var());
}

// メソッド定義
PyMethodDef methods[] = {
  {"size",
   size,
   METH_NOARGS,
   PyDoc_STR("return size")},
  {"to_varlist",
   to_varlist,
   METH_NOARGS,
   PyDoc_STR("convert to list of 'BddVar's")},
  {"top_var",
   top_var,
   METH_NOARGS,
   PyDoc_STR("return top variable")},
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
    "mgr",
    "varset",
    nullptr
  };
  PyObject* mgr_obj = nullptr;
  PyObject* list_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "|O!O",
                                    const_cast<char**>(kwlist),
                                    PyBddMgr::_typeobject(), &mgr_obj,
                                    &list_obj) ) {
    return nullptr;
  }
  try {
    auto self = type->tp_alloc(type, 0);
    auto my_obj = reinterpret_cast<PyBddVarSet_Object*>(self);
    if ( mgr_obj == nullptr ) {
      new (&my_obj->mVal) BddVarSet();
    }
    else {
      auto& mgr = PyBddMgr::_get_ref(mgr_obj);
      std::vector<Bddvar> var_set;
      if ( !PyList<BddVar, PyBddVar>::FromPyObject(list_obj, var_set) ) {
        PyErr_SetString(PyExc_TypeError, "argument 2 should be a sequence of 'BddVar'");
        return nullptr;
      }
      new (&my_obj->mVal) BddVarSet(mgr, var_set);
    }
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


// @brief PyBddVarSet オブジェクトを使用可能にする．
bool
PyBddVarSet::init(
  PyObject* m
)
{
  PyBddVarSet_Type.tp_name = "PyBddVarSet";
  PyBddVarSet_Type.tp_basicsize = sizeof(PyBddVarSet_Object);
  PyBddVarSet_Type.tp_itemsize = 0;
  PyBddVarSet_Type.tp_dealloc = dealloc_func;
  PyBddVarSet_Type.tp_as_number = &number;
  PyBddVarSet_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  PyBddVarSet_Type.tp_doc = PyDoc_STR("Python extended object for BddVarSet");
  PyBddVarSet_Type.tp_richcompare = richcompare_func;
  PyBddVarSet_Type.tp_methods = methods;
  PyBddVarSet_Type.tp_new = new_func;
  if ( !PyModule::reg_type(m, "PyBddVarSet", &PyBddVarSet_Type) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// BddVarSet を PyObject に変換する．
PyObject*
PyBddVarSet::Conv::operator()(
  const ElemType& val ///< [in] 元の値
)
{
  auto type = PyBddVarSet::_typeobject();
  auto obj = type->tp_alloc(type, 0);
  auto my_obj = reinterpret_cast<PyBddVarSet_Object*>(obj);
  new (&my_obj->mVal) BddVarSet(val);
  return obj;
}

// PyObject を BddVarSet に変換する．
bool
PyBddVarSet::Deconv::operator()(
  PyObject* obj, ///< [in] Python のオブジェクト
  ElemType& val  ///< [out] 結果を格納する変数
)
{
  if ( PyBddVarSet::Check(obj) ) {
    val = PyBddVarSet::_get_ref(obj);
    return true;
  }
  return false;
}

// @brief PyObject が BddVarSet タイプか調べる．
bool
PyBddVarSet::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, &PyBddVarSet_Type);
}

// @brief PyObject から BddVarSet を取り出す．
BddVarSet&
PyBddVarSet::_get_ref(
  PyObject* obj
)
{
  auto my_obj = reinterpret_cast<PyBddVarSet_Object*>(obj);
  return my_obj->mVal;
}

// @brief BddVarSet を表すオブジェクトの型定義を返す．
PyTypeObject*
PyBddVarSet::_typeobject()
{
  return &PyBddVarSet_Type;
}

END_NAMESPACE_YM
