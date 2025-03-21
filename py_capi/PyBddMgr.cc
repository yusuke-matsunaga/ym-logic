
/// @file PyBddMgr.cc
/// @brief Python BddMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyBddMgr.h"
#include "pym/PyBdd.h"
#include "pym/PyJsonValue.h"
#include "pym/PyModule.h"
#include "ym/BddMgr.h"
#include "ym/BddVar.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
struct BddMgrObject
{
  PyObject_HEAD
  BddMgr mVal;
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
  static const char* kw_list[] = {
    nullptr
  };
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "",
				    const_cast<char**>(kw_list)) ) {
    return nullptr;
  }

  auto self = type->tp_alloc(type, 0);
  auto bddmgr_obj = reinterpret_cast<BddMgrObject*>(self);
  new (&bddmgr_obj->mVal) BddMgr{};
  return self;
}

// 終了関数
void
BddMgr_dealloc(
  PyObject* self
)
{
  auto bddmgr_obj = reinterpret_cast<BddMgrObject*>(self);
  bddmgr_obj->mVal.~BddMgr();
  Py_TYPE(self)->tp_free(self);
}

PyObject*
BddMgr_variable(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kw_list[] = {
    "varid",
    nullptr
  };
  SizeType varid = -1;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "k",
				    const_cast<char**>(kw_list),
				    &varid) ) {
    return nullptr;
  }
  auto& bddmgr = PyBddMgr::_get_ref(self);
  auto var = bddmgr.variable(varid);
  return PyBdd::ToPyObject(var);
}

PyObject*
BddMgr_copy(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kw_list[] = {
    "src",
    nullptr
  };
  PyObject* bdd_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O!",
				    const_cast<char**>(kw_list),
				    PyBdd::_typeobject(), !bdd_obj) ) {
    return nullptr;
  }
  auto& src_bdd = PyBdd::_get_ref(bdd_obj);
  auto& bddmgr = PyBddMgr::_get_ref(self);
  auto ans_bdd = bddmgr.copy(src_bdd);
  return PyBdd::ToPyObject(ans_bdd);
}

PyObject*
BddMgr_zero(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& bddmgr = PyBddMgr::_get_ref(self);
  auto ans_bdd = bddmgr.zero();
  return PyBdd::ToPyObject(ans_bdd);
}

PyObject*
BddMgr_one(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& bddmgr = PyBddMgr::_get_ref(self);
  auto ans_bdd = bddmgr.one();
  return PyBdd::ToPyObject(ans_bdd);
}

PyObject*
BddMgr_from_truth(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kw_list[] = {
    "func_str",
    "var_list",
    nullptr
  };
  PyObject* var_list_obj = nullptr;
  const char* str = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "s|O",
				    const_cast<char**>(kw_list),
				    &str,
				    &var_list_obj) ) {
    return nullptr;
  }
  auto& bddmgr = PyBddMgr::_get_ref(self);
  if ( var_list_obj == nullptr ) {
    try {
      auto ans_bdd = bddmgr.from_truth(str);
      return PyBdd::ToPyObject(ans_bdd);
    }
    catch ( std::invalid_argument ) {
      PyErr_SetString(PyExc_ValueError, "invalid string");
      return nullptr;
    }
  }
  else {
    static const char* err_msg = "var_list should be a sequence of BddVar";
    if ( !PySequence_Check(var_list_obj) ) {
      PyErr_SetString(PyExc_TypeError, err_msg);
      return nullptr;
    }
    auto n = PySequence_Size(var_list_obj);
    vector<BddVar> var_list(n);
    for ( SizeType i = 0; i < n; ++ i ) {
      auto var_obj = PySequence_GetItem(var_list_obj, i);
      if ( !PyBdd::Check(var_obj) ) {
	PyErr_SetString(PyExc_TypeError, err_msg);
	return nullptr;
      }
      auto& var = PyBdd::_get_ref(var_obj);
      if ( !var.is_variable() ) {
	PyErr_SetString(PyExc_TypeError, err_msg);
	return nullptr;
      }
      var_list[i] = BddVar::from_bdd(var);
    }
    try {
      auto ans_bdd = bddmgr.from_truth(str, var_list);
      return PyBdd::ToPyObject(ans_bdd);
    }
    catch ( std::invalid_argument ) {
      PyErr_SetString(PyExc_ValueError, "invalid string");
      return nullptr;
    }
  }
}

PyObject*
BddMgr_enable_gc(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& bddmgr = PyBddMgr::_get_ref(self);
  bddmgr.enable_gc();
  Py_RETURN_NONE;
}

PyObject*
BddMgr_disable_gc(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& bddmgr = PyBddMgr::_get_ref(self);
  bddmgr.disable_gc();
  Py_RETURN_NONE;
}

// メソッド定義
PyMethodDef BddMgr_methods[] = {
  {"variable", reinterpret_cast<PyCFunction>(BddMgr_variable),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return variable object")},
  {"copy", reinterpret_cast<PyCFunction>(BddMgr_copy),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("copy BDD")},
  {"zero", BddMgr_zero, METH_NOARGS,
   PyDoc_STR("generate Zero")},
  {"one", BddMgr_one, METH_NOARGS,
   PyDoc_STR("generate One")},
  {"from_truth", reinterpret_cast<PyCFunction>(BddMgr_from_truth),
   METH_VARARGS | METH_KEYWORDS,
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
  auto& bddmgr = PyBddMgr::_get_ref(self);
  auto val = bddmgr.node_num();
  return PyLong_FromLong(val);
}

PyObject*
BddMgr_gc_limit(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& bddmgr = PyBddMgr::_get_ref(self);
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
  auto& bddmgr = PyBddMgr::_get_ref(self);
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

// @brief BddMgr を PyObject* に変換する
PyObject*
PyBddMgr::Conv::operator()(
  const ElemType& val
)
{
  auto type = PyBddMgr::_typeobject();
  auto obj = type->tp_alloc(type, 0);
  auto bddmgr_obj = reinterpret_cast<BddMgrObject*>(obj);
  new (&bddmgr_obj->mVal) BddMgr(val);
  return obj;
}

// @brief PyObject* から BddMgr を取り出すファンクタクラス
bool
PyBddMgr::Deconv::operator()(
  PyObject* obj,
  ElemType& val
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
  return Py_IS_TYPE(obj, _typeobject());
}

// @brief BddMgr を表す PyObject から BddMgr を取り出す．
BddMgr&
PyBddMgr::_get_ref(
  PyObject* obj
)
{
  auto bddmgr_obj = reinterpret_cast<BddMgrObject*>(obj);
  return bddmgr_obj->mVal;
}

// @brief BddMgr を表すオブジェクトの型定義を返す．
PyTypeObject*
PyBddMgr::_typeobject()
{
  return &BddMgrType;
}

END_NAMESPACE_YM
