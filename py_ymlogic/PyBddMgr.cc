
/// @file PyBddMgr.cc
/// @brief Python BddMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/PyBddMgr.h"
#include "ym/PyBdd.h"
#include "ym/PyLiteral.h"
#include "ym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
struct BddMgrObject
{
  PyObject_HEAD
  BddMgr* mVal;
};

// Python 用のタイプ定義
PyTypeObject BddMgrType = {
  PyVarObject_HEAD_INIT(nullptr, 0)
};

// 生成関数
PyObject*
BddMgr_new(
  PyTypeObject* type,
  PyObject* args,
  PyObject* kwds
)
{
  // 引数を受け取らないことをチェックする．
  static const char* kwlist[] = {
    nullptr
  };
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "",
				    const_cast<char**>(kwlist)) ) {
    return nullptr;
  }

  auto self = type->tp_alloc(type, 0);
  auto bddmgr_obj = reinterpret_cast<BddMgrObject*>(self);
  bddmgr_obj->mVal = new BddMgr{};
  return self;
}

// 終了関数
void
BddMgr_dealloc(
  PyObject* self
)
{
  auto bddmgr_obj = reinterpret_cast<BddMgrObject*>(self);
  delete bddmgr_obj->mVal;
  Py_TYPE(self)->tp_free(self);
}

PyObject*
BddMgr_copy(
  PyObject* self,
  PyObject* args
)
{
  PyObject* bdd_obj = nullptr;
  if ( !PyArg_ParseTuple(args, "O!", PyBdd::_typeobject(), !bdd_obj) ) {
    return nullptr;
  }
  auto src_bdd = PyBdd::_get(bdd_obj);
  auto& bddmgr = PyBddMgr::_get(self);
  auto ans_bdd = bddmgr.copy(src_bdd);
  return PyBdd::ToPyObject(ans_bdd);
}

PyObject*
BddMgr_zero(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& bddmgr = PyBddMgr::_get(self);
  auto ans_bdd = bddmgr.zero();
  return PyBdd::ToPyObject(ans_bdd);
}

PyObject*
BddMgr_one(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& bddmgr = PyBddMgr::_get(self);
  auto ans_bdd = bddmgr.one();
  return PyBdd::ToPyObject(ans_bdd);
}

PyObject*
BddMgr_literal(
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
  PyObject* obj1 = nullptr;
  int inv = 0;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O!$p",
				    const_cast<char**>(kwlist),
				    &obj1, &inv) ) {
    return nullptr;
  }
  Literal lit;
  if ( PyLong_Check(obj1) ) {
    SizeType varid = PyLong_AsLong(obj1);
    lit = Literal{varid, static_cast<bool>(inv)};
  }
  else if ( PyLiteral::_check(obj1) ) {
    if ( inv ) {
      PyErr_SetString(PyExc_TypeError, "'inv' keyword is not allowed with 'Literal' type");
      return nullptr;
    }
    lit = PyLiteral::_get(obj1);
  }
  else {
    PyErr_SetString(PyExc_TypeError, "argument 1 must be an int or Literal");
    return nullptr;
  }
  auto& bddmgr = PyBddMgr::_get(self);
  auto ans_bdd = bddmgr.literal(lit);
  return PyBdd::ToPyObject(ans_bdd);
}

PyObject*
BddMgr_posi_literal(
  PyObject* self,
  PyObject* args
)
{
  SizeType varid = 0;
  if ( !PyArg_ParseTuple(args, "k", &varid) ) {
    return nullptr;
  }
  auto& bddmgr = PyBddMgr::_get(self);
  auto ans_bdd = bddmgr.posi_literal(varid);
  return PyBdd::ToPyObject(ans_bdd);
}

PyObject*
BddMgr_nega_literal(
  PyObject* self,
  PyObject* args
)
{
  SizeType varid = 0;
  if ( !PyArg_ParseTuple(args, "k", &varid) ) {
    return nullptr;
  }
  auto& bddmgr = PyBddMgr::_get(self);
  auto ans_bdd = bddmgr.nega_literal(varid);
  return PyBdd::ToPyObject(ans_bdd);
}

PyObject*
BddMgr_from_truth(
  PyObject* self,
  PyObject* args
)
{
  const char* str = nullptr;
  if ( PyArg_ParseTuple(args, "s", &str) ) {
    return nullptr;
  }
  auto& bddmgr = PyBddMgr::_get(self);
  try {
    auto ans_bdd = bddmgr.from_truth(str);
    return PyBdd::ToPyObject(ans_bdd);
  }
  catch ( std::invalid_argument ) {
    PyErr_SetString(PyExc_ValueError, "invalid string");
    return nullptr;
  }
}

PyObject*
BddMgr_enable_gc(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& bddmgr = PyBddMgr::_get(self);
  bddmgr.enable_gc();
  Py_RETURN_NONE;
}

PyObject*
BddMgr_disable_gc(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& bddmgr = PyBddMgr::_get(self);
  bddmgr.disable_gc();
  Py_RETURN_NONE;
}

// メソッド定義
PyMethodDef BddMgr_methods[] = {
  {"copy", BddMgr_copy, METH_VARARGS,
   PyDoc_STR("copy BDD")},
  {"zero", BddMgr_copy, METH_NOARGS,
   PyDoc_STR("generate Zero")},
  {"one", BddMgr_copy, METH_NOARGS,
   PyDoc_STR("generate One")},
  {"literal", reinterpret_cast<PyCFunction>(BddMgr_literal),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("generate literal")},
  {"posi_literal", BddMgr_posi_literal, METH_NOARGS,
   PyDoc_STR("generate positive literal")},
  {"nega_literal", BddMgr_nega_literal, METH_NOARGS,
   PyDoc_STR("generate positive literal")},
  {"from_truth", BddMgr_from_truth, METH_VARARGS,
   PyDoc_STR("generate BDD from truth-table string")},
  {"enable_gc", BddMgr_enable_gc, METH_NOARGS,
   PyDoc_STR("enable GC")},
  {"disable_gc", BddMgr_disable_gc, METH_NOARGS,
   PyDoc_STR("disable GC")},
  {nullptr, nullptr, 0, nullptr}
};

PyObject*
BddMgr_node_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& bddmgr = PyBddMgr::_get(self);
  auto val = bddmgr.node_num();
  return PyLong_FromLong(val);
}

PyObject*
BddMgr_gc_limit(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& bddmgr = PyBddMgr::_get(self);
  auto val = bddmgr.gc_limit();
  return PyLong_FromLong(val);
}

int
BddMgr_set_gc_limit(
  PyObject* self,
  PyObject* val_obj,
  void* Py_UNUSED(closure)
)
{
  if ( !PyLong_Check(val_obj) ) {
    PyErr_SetString(PyExc_TypeError, "int value is expected");
    return -1;
  }
  auto& bddmgr = PyBddMgr::_get(self);
  SizeType val = PyLong_AsLong(val_obj);
  bddmgr.set_gc_limit(val);
  return 0;
}

// get/set 関数定義
PyGetSetDef BddMgr_getset[] = {
  {"node_num", BddMgr_node_num, nullptr, PyDoc_STR("total node number"), nullptr},
  {"gc_limit", BddMgr_gc_limit, BddMgr_set_gc_limit, PyDoc_STR("GC limit"), nullptr},
  {nullptr, nullptr, nullptr, nullptr, nullptr},
};

END_NONAMESPACE


// @brief 'BddMgr' オブジェクトを使用可能にする．
bool
PyBddMgr::init(
  PyObject* m
)
{
  BddMgrType.tp_name = "BddMgr";
  BddMgrType.tp_basicsize = sizeof(BddMgrObject);
  BddMgrType.tp_itemsize = 0;
  BddMgrType.tp_dealloc = BddMgr_dealloc;
  BddMgrType.tp_flags = Py_TPFLAGS_DEFAULT;
  BddMgrType.tp_doc = PyDoc_STR("BddMgr object");
  BddMgrType.tp_methods = BddMgr_methods;
  BddMgrType.tp_getset = BddMgr_getset;
  BddMgrType.tp_new = BddMgr_new;

  // 型オブジェクトの登録
  if ( !PyModule::reg_type(m, "BddMgr", &BddMgrType) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// @brief PyObject が BddMgr タイプか調べる．
bool
PyBddMgr::_check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, _typeobject());
}

// @brief BddMgr を表す PyObject から BddMgr を取り出す．
BddMgr&
PyBddMgr::_get(
  PyObject* obj
)
{
  auto bddmgr_obj = reinterpret_cast<BddMgrObject*>(obj);
  return *bddmgr_obj->mVal;
}

// @brief BddMgr を表すオブジェクトの型定義を返す．
PyTypeObject*
PyBddMgr::_typeobject()
{
  return &BddMgrType;
}

END_NAMESPACE_YM
