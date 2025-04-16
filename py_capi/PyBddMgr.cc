
/// @file PyBddMgr.cc
/// @brief PyBddMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyBddMgr.h"
#include "pym/PyBdd.h"
#include "pym/PyLong.h"
#include "pym/PyJsonValue.h"
#include "pym/PyList.h"
#include "ym/BddVar.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
// この構造体は同じサイズのヒープから作られるので
// mVal のコンストラクタは起動されないことに注意．
// そのためあとでコンストラクタを明示的に起動する必要がある．
// またメモリを開放するときにも明示的にデストラクタを起動する必要がある．
struct BddMgr_Object
{
  PyObject_HEAD
  BddMgr mVal;
};

// Python 用のタイプ定義
PyTypeObject BddMgr_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PyBddMgr::init() 中で初期化する．
};

// 終了関数
void
dealloc_func(
  PyObject* self
)
{
  auto obj = reinterpret_cast<BddMgr_Object*>(self);
  obj->mVal.~BddMgr();
  Py_TYPE(self)->tp_free(self);
}

// make variable BDD
PyObject*
variable(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "varid",
    nullptr
  };
  int varid;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "i",
                                    const_cast<char**>(kwlist),
                                    &varid) ) {
    return nullptr;
  }
  auto& val = PyBddMgr::_get_ref(self);
  return PyBdd::ToPyObject(val.variable(varid));
}

// make a copy of BDD
PyObject*
copy(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "src",
    nullptr
  };
  PyObject* src_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O!",
                                    const_cast<char**>(kwlist),
                                    PyBdd::_typeobject(), &src_obj) ) {
    return nullptr;
  }
  Bdd src;
  if ( src_obj != nullptr ) {
    if ( !PyBdd::FromPyObject(src_obj, src) ) {
      PyErr_SetString(PyExc_TypeError, "could not convert to Bdd");
      return nullptr;
    }
  }
  auto& val = PyBddMgr::_get_ref(self);
  return PyBdd::ToPyObject(val.copy(src));
}

// make ZERO BDD
PyObject*
zero(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyBddMgr::_get_ref(self);
  return PyBdd::ToPyObject(val.zero());
}

// make ONE BDD
PyObject*
one(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyBddMgr::_get_ref(self);
  return PyBdd::ToPyObject(val.one());
}

// make a BDD from truth table
PyObject*
from_truth(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "func_str",
    "var_list",
    nullptr
  };
  const char* func_str_tmp = nullptr;
  PyObject* list_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "s|$O",
                                    const_cast<char**>(kwlist),
                                    &func_str_tmp,
                                    &list_obj) ) {
    return nullptr;
  }
  std::string func_str;
  if ( func_str_tmp != nullptr ) {
    func_str = std::string(func_str_tmp);
  }
  auto& val = PyBddMgr::_get_ref(self);
  if ( list_obj == nullptr ) {
    try {
      return PyBdd::ToPyObject(val.from_truth(func_str));
    }
    catch ( std::invalid_argument err ) {
      std::ostringstream buf;
      buf << "invalid string" << ": " << err.what();
      PyErr_SetString(PyExc_ValueError, buf.str().c_str());
      return nullptr;
    }
  }
  else {
    std::vector<Bdd> tmp_list;
    if ( !PyList<Bdd, PyBdd>::FromPyObject(list_obj, tmp_list) ) {
      PyErr_SetString(PyExc_TypeError, "argument 2 should be a sequence of 'BddVar'");
      return nullptr;
    }
    std::vector<BddVar> var_list;
    if ( !BddVar::from_bdd_list(tmp_list, var_list) ) {
      PyErr_SetString(PyExc_TypeError, "argument 2 should be a sequence of 'BddVar'");
      return nullptr;
    }
    try {
      return PyBdd::ToPyObject(val.from_truth(func_str, var_list));
    }
    catch ( std::invalid_argument err ) {
      std::ostringstream buf;
      buf << "invalid string" << ": " << err.what();
      PyErr_SetString(PyExc_ValueError, buf.str().c_str());
      return nullptr;
    }
  }
}

// enable GC
PyObject*
enable_gc(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyBddMgr::_get_ref(self);
  val.enable_gc();
  Py_RETURN_NONE;
}

// disable GC
PyObject*
disable_gc(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyBddMgr::_get_ref(self);
  val.enable_gc();
  Py_RETURN_NONE;
}

// メソッド定義
PyMethodDef methods[] = {
  {"variable",
   reinterpret_cast<PyCFunction>(variable),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("make variable BDD")},
  {"copy",
   reinterpret_cast<PyCFunction>(copy),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("make a copy of BDD")},
  {"zero",
   zero,
   METH_NOARGS,
   PyDoc_STR("make ZERO BDD")},
  {"one",
   one,
   METH_NOARGS,
   PyDoc_STR("make ONE BDD")},
  {"from_truth",
   reinterpret_cast<PyCFunction>(from_truth),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("make a BDD from truth table")},
  {"enable_gc",
   enable_gc,
   METH_NOARGS,
   PyDoc_STR("enable GC")},
  {"disable_gc",
   disable_gc,
   METH_NOARGS,
   PyDoc_STR("disable GC")},
  // end-marker
  {nullptr, nullptr, 0, nullptr}
};

PyObject*
get_node_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyBddMgr::_get_ref(self);
  return PyLong::ToPyObject(val.node_num());
}

PyObject*
get_gc_limit(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyBddMgr::_get_ref(self);
  return PyLong::ToPyObject(val.gc_limit());
}

int
set_gc_limit(
  PyObject* self,
  PyObject* obj,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyBddMgr::_get_ref(self);
  if ( !PyLong::Check(obj) ) {
    PyErr_SetString(PyExc_TypeError, "integer type is expected");
    return -1;
  }
  auto gc_limit = PyLong::Get(obj);
  val.set_gc_limit(gc_limit);
  return 0;
}

// getter/setter定義
PyGetSetDef getsets[] = {
  {"node_num", get_node_num, nullptr, PyDoc_STR(""), nullptr},
  {"gc_limit", get_gc_limit, set_gc_limit, PyDoc_STR(""), nullptr},
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
    auto my_obj = reinterpret_cast<BddMgr_Object*>(self);
    new (&my_obj->mVal) BddMgr();
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


// @brief BddMgr オブジェクトを使用可能にする．
bool
PyBddMgr::init(
  PyObject* m
)
{
  BddMgr_Type.tp_name = "BddMgr";
  BddMgr_Type.tp_basicsize = sizeof(BddMgr_Object);
  BddMgr_Type.tp_itemsize = 0;
  BddMgr_Type.tp_dealloc = dealloc_func;
  BddMgr_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  BddMgr_Type.tp_doc = PyDoc_STR("Python extended object for BddMgr");
  BddMgr_Type.tp_methods = methods;
  BddMgr_Type.tp_getset = getsets;
  BddMgr_Type.tp_new = new_func;
  if ( !PyModule::reg_type(m, "BddMgr", &BddMgr_Type) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// BddMgr を PyObject に変換する．
PyObject*
PyBddMgr::Conv::operator()(
  const BddMgr& val
)
{
  auto type = PyBddMgr::_typeobject();
  auto obj = type->tp_alloc(type, 0);
  auto my_obj = reinterpret_cast<BddMgr_Object*>(obj);
  new (&my_obj->mVal) BddMgr(val);
  return obj;
}

// PyObject を BddMgr に変換する．
bool
PyBddMgr::Deconv::operator()(
  PyObject* obj,
  BddMgr& val
)
{
  if ( PyBddMgr::Check(obj) ) {
    val = PyBddMgr::_get_ref(obj);
    return true;
  }
  return false;
}

// @brief PyObject が BddMgr タイプか調べる．
bool
PyBddMgr::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, &BddMgr_Type);
}

// @brief PyObject から BddMgr を取り出す．
BddMgr&
PyBddMgr::_get_ref(
  PyObject* obj
)
{
  auto my_obj = reinterpret_cast<BddMgr_Object*>(obj);
  return my_obj->mVal;
}

// @brief BddMgr を表すオブジェクトの型定義を返す．
PyTypeObject*
PyBddMgr::_typeobject()
{
  return &BddMgr_Type;
}

END_NAMESPACE_YM
