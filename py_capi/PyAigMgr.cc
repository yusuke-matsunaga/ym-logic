
/// @file PyAigMgr.cc
/// @brief Python AigMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyAigMgr.h"
#include "pym/PyAigHandle.h"
#include "pym/PyExpr.h"
#include "pym/PyJsonValue.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
struct AigMgrObject
{
  PyObject_HEAD
  AigMgr mMgr;
};

// Python 用のタイプ定義
PyTypeObject AigMgrType = {
  PyVarObject_HEAD_INIT(nullptr, 0)
};

// 生成関数
PyObject*
AigMgr_new(
  PyTypeObject* type,
  PyObject* args,
  PyObject* kwds
)
{
  // 引数を取らないことをチェックする．
  static const char* kw_list[] = {
    nullptr
  };
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "",
				    const_cast<char**>(kw_list)) ) {
    return nullptr;
  }

  auto self = type->tp_alloc(type, 0);
  auto aigmgr_obj = reinterpret_cast<AigMgrObject*>(self);
  new (&aigmgr_obj->mMgr) AigMgr;
  return self;
}

// 終了関数
void
AigMgr_dealloc(
  PyObject* self
)
{
  auto aigmgr_obj = reinterpret_cast<AigMgrObject*>(self);
  aigmgr_obj->mMgr.~AigMgr();
  Py_TYPE(self)->tp_free(self);
}

PyObject*
AigMgr_eval(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kw_list[] = {
    "input_vals",
    "output_list",
    nullptr
  };
  PyObject* iv_obj = nullptr;
  PyObject* ol_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "OO",
				    const_cast<char**>(kw_list),
				    &iv_obj, &ol_obj) ) {
    return nullptr;
  }

  static const char* emsg1 = "'input_vals' should be a sequence of int";
  if ( !PySequence_Check(iv_obj) ) {
    PyErr_SetString(PyExc_TypeError, emsg1);
    return nullptr;
  }
  auto n1 = PySequence_Size(iv_obj);
  vector<AigBitVect> input_vals(n1);
  for ( SizeType i = 0; i < n1; ++ i ) {
    auto obj = PySequence_GetItem(iv_obj, i);
    auto res = PyLong_Check(obj);
    if ( res ) {
      auto val = PyLong_AsLong(obj);
      input_vals[i] = val;
    }
    Py_DecRef(obj);
    if ( !res ) {
      PyErr_SetString(PyExc_TypeError, emsg1);
      return nullptr;
    }
  }

  vector<AigHandle> output_list;
  if ( !PyAigHandle::FromPyList(ol_obj, output_list) ) {
    PyErr_SetString(PyExc_TypeError,
		    "'output_list' should be a sequence of AigHandles");
    return nullptr;
  }

  auto& mgr = PyAigMgr::_get_ref(self);
  try {
    auto output_vals = mgr.eval(input_vals, output_list);
    auto n = output_vals.size();
    auto ans_obj = PyList_New(n);
    for ( SizeType i = 0; i < n; ++ i ) {
      auto val = output_vals[i];
      auto obj = PyLong_FromLong(val);
      PyList_SetItem(ans_obj, i, obj);
    }
    return ans_obj;
  }
  catch ( std::invalid_argument err ) {
    PyErr_SetString(PyExc_ValueError, err.what());
    return nullptr;
  }
}

PyObject*
AigMgr_input(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kw_list[] = {
    "input_id",
    nullptr
  };
  SizeType input_id = -1;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "k",
				    const_cast<char**>(kw_list),
				    &input_id) ) {
    return nullptr;
  }

  auto& mgr = PyAigMgr::_get_ref(self);
  try {
    auto ans = mgr.input(input_id);
    return PyAigHandle::ToPyObject(ans);
  }
  catch ( std::out_of_range err ) {
    PyErr_SetString(PyExc_ValueError, err.what());
    return nullptr;
  }
}

PyObject*
AigMgr_make_zero(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& mgr = PyAigMgr::_get_ref(self);
  auto ans = mgr.make_zero();
  return PyAigHandle::ToPyObject(ans);
}

PyObject*
AigMgr_make_one(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& mgr = PyAigMgr::_get_ref(self);
  auto ans = mgr.make_one();
  return PyAigHandle::ToPyObject(ans);
}

PyObject*
AigMgr_make_input(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& mgr = PyAigMgr::_get_ref(self);
  auto ans = mgr.make_input();
  return PyAigHandle::ToPyObject(ans);
}

PyObject*
AigMgr_and_op(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kw_list[] = {
    "fanin_list",
    nullptr
  };
  PyObject* list_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O",
				    const_cast<char**>(kw_list),
				    &list_obj) ) {
    return nullptr;
  }
  vector<AigHandle> fanin_list;
  if ( !PyAigHandle::FromPyList(list_obj, fanin_list) ) {
    PyErr_SetString(PyExc_TypeError,
		    "'fanin_list' should be a sequcence of 'AigHandle'");
    return nullptr;
  }
  auto& mgr = PyAigMgr::_get_ref(self);
  auto ans = mgr.and_op(fanin_list);
  return PyAigHandle::ToPyObject(ans);
}

PyObject*
AigMgr_or_op(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kw_list[] = {
    "fanin_list",
    nullptr
  };
  PyObject* list_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O",
				    const_cast<char**>(kw_list),
				    &list_obj) ) {
    return nullptr;
  }
  vector<AigHandle> fanin_list;
  if ( !PyAigHandle::FromPyList(list_obj, fanin_list) ) {
    PyErr_SetString(PyExc_TypeError,
		    "'fanin_list' should be a sequcence of 'AigHandle'");
    return nullptr;
  }
  auto& mgr = PyAigMgr::_get_ref(self);
  auto ans = mgr.or_op(fanin_list);
  return PyAigHandle::ToPyObject(ans);
}

PyObject*
AigMgr_xor_op(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kw_list[] = {
    "fanin_list",
    nullptr
  };
  PyObject* list_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O",
				    const_cast<char**>(kw_list),
				    &list_obj) ) {
    return nullptr;
  }
  vector<AigHandle> fanin_list;
  if ( !PyAigHandle::FromPyList(list_obj, fanin_list) ) {
    PyErr_SetString(PyExc_TypeError,
		    "'fanin_list' should be a sequcence of 'AigHandle'");
    return nullptr;
  }
  auto& mgr = PyAigMgr::_get_ref(self);
  auto ans = mgr.xor_op(fanin_list);
  return PyAigHandle::ToPyObject(ans);
}

PyObject*
AigMgr_from_expr(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kw_list[] = {
    "expr",
    nullptr
  };
  PyObject* expr_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O!",
				    const_cast<char**>(kw_list),
				    PyExpr::_typeobject(), &expr_obj) ) {
    return nullptr;
  }
  auto expr = PyExpr::_get_ref(expr_obj);
  auto& mgr = PyAigMgr::_get_ref(self);
  auto ans = mgr.from_expr(expr);
  return PyAigHandle::ToPyObject(ans);
}

PyObject*
AigMgr_from_expr_list(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kw_list[] = {
    "expr_list",
    nullptr
  };
  PyObject* list_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O",
				    const_cast<char**>(kw_list),
				    &list_obj) ) {
    return nullptr;
  }
  static const char* emsg = "'expr_list' should be a sequence of Exprs";
  if ( !PySequence_Check(list_obj) ) {
    PyErr_SetString(PyExc_TypeError, emsg);
    return nullptr;
  }
  auto n = PySequence_Size(list_obj);
  vector<Expr> expr_list(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    auto obj = PySequence_GetItem(list_obj, i);
    auto res = PyExpr::_check(obj);
    if ( res ) {
      expr_list[i] = PyExpr::_get_ref(obj);
    }
    Py_DecRef(obj);
    if ( !res ) {
      PyErr_SetString(PyExc_TypeError, emsg);
      return nullptr;
    }
  }
  auto& mgr = PyAigMgr::_get_ref(self);
  auto ans = mgr.from_expr_list(expr_list);
  return PyAigHandle::ToPyList(ans);
}

// メソッド定義
PyMethodDef AigMgr_methods[] = {
  {"eval", reinterpret_cast<PyCFunction>(AigMgr_eval),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("evaluate")},
  {"input", reinterpret_cast<PyCFunction>(AigMgr_input),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("returns an INPUT")},
  {"make_zero", AigMgr_make_zero,
   METH_NOARGS,
   PyDoc_STR("make ZERO")},
  {"make_one", AigMgr_make_one,
   METH_NOARGS,
   PyDoc_STR("make ONE")},
  {"make_input", AigMgr_make_input,
   METH_NOARGS,
   PyDoc_STR("make an INPU")},
  {"and_op", reinterpret_cast<PyCFunction>(AigMgr_and_op),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("make an AND")},
  {"or_op", reinterpret_cast<PyCFunction>(AigMgr_or_op),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("make an OR")},
  {"xor_op", reinterpret_cast<PyCFunction>(AigMgr_xor_op),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("make an XOR")},
  {"from_expr", reinterpret_cast<PyCFunction>(AigMgr_from_expr),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("convert from an Expr")},
  {"from_expr_list", reinterpret_cast<PyCFunction>(AigMgr_from_expr_list),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("convert from a sequcence of Exprs")},
  {nullptr, nullptr, 0, nullptr}
};

PyObject*
AigMgr_node_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& mgr = PyAigMgr::_get_ref(self);
  auto ans = mgr.node_num();
  return PyLong_FromLong(ans);
}

PyObject*
AigMgr_input_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& mgr = PyAigMgr::_get_ref(self);
  auto ans = mgr.input_num();
  return PyLong_FromLong(ans);
}

PyObject*
AigMgr_and_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& mgr = PyAigMgr::_get_ref(self);
  auto ans = mgr.and_num();
  return PyLong_FromLong(ans);
}

// get/set 関数定義
PyGetSetDef AigMgr_getset[] = {
  {"node_num", AigMgr_node_num, nullptr, PyDoc_STR("# of total nodes"), nullptr},
  {"input_num", AigMgr_node_num, nullptr, PyDoc_STR("# of inputs"), nullptr},
  {"and_num", AigMgr_node_num, nullptr, PyDoc_STR("# of AND nodes"), nullptr},
  {nullptr, nullptr, nullptr, nullptr, nullptr},
};

END_NONAMESPACE


// @brief 'AigMgr' オブジェクトを使用可能にする．
bool
PyAigMgr::init(
  PyObject* m
)
{
  AigMgrType.tp_name = "AigMgr";
  AigMgrType.tp_basicsize = sizeof(AigMgrObject);
  AigMgrType.tp_itemsize = 0;
  AigMgrType.tp_dealloc = AigMgr_dealloc;
  AigMgrType.tp_flags = Py_TPFLAGS_DEFAULT;
  AigMgrType.tp_doc = PyDoc_STR("AigMgr object");
  AigMgrType.tp_methods = AigMgr_methods;
  AigMgrType.tp_getset = AigMgr_getset;
  AigMgrType.tp_new = AigMgr_new;

  // 型オブジェクトの登録
  if ( !PyModule::reg_type(m, "AigMgr", &AigMgrType) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// @brief PyObject が AigMgr タイプか調べる．
bool
PyAigMgr::_check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, _typeobject());
}

// @brief AigMgr を表す PyObject から AigMgr を取り出す．
AigMgr&
PyAigMgr::_get_ref(
  PyObject* obj
)
{
  auto aigmgr_obj = reinterpret_cast<AigMgrObject*>(obj);
  return aigmgr_obj->mMgr;
}

// @brief AigMgr を表すオブジェクトの型定義を返す．
PyTypeObject*
PyAigMgr::_typeobject()
{
  return &AigMgrType;
}

END_NAMESPACE_YM
