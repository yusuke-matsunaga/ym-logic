
/// @file PyAigMgr.cc
/// @brief PyAigMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyAigMgr.h"
#include "pym/PyAigHandle.h"
#include "pym/PyExpr.h"
#include "pym/PyList.h"
#include "pym/PyLong.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
// この構造体は同じサイズのヒープから作られるので
// mVal のコンストラクタは起動されないことに注意．
// そのためあとでコンストラクタを明示的に起動する必要がある．
// またメモリを開放するときにも明示的にデストラクタを起動する必要がある．
struct AigMgr_Object
{
  PyObject_HEAD
  AigMgr mVal;
};

// Python 用のタイプ定義
PyTypeObject AigMgr_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PyAigMgr::init() 中で初期化する．
};

// 終了関数
void
dealloc_func(
  PyObject* self
)
{
  auto obj = reinterpret_cast<AigMgr_Object*>(self);
  obj->mVal.~AigMgr();
  Py_TYPE(self)->tp_free(self);
}

// return specified INPUT
PyObject*
input(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "input_id",
    nullptr
  };
  int input_id;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "i",
                                    const_cast<char**>(kwlist),
                                    &input_id) ) {
    return nullptr;
  }
  auto& val = PyAigMgr::_get_ref(self);
  try {
    return PyAigHandle::ToPyObject(val.input(input_id));
  }
  catch ( std::invalid_argument err ) {
    std::ostringstream buf;
    buf << "invalid argument" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// make ZERO
PyObject*
zero(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyAigMgr::_get_ref(self);
  return PyAigHandle::ToPyObject(val.make_zero());
}

// make ONE
PyObject*
one(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyAigMgr::_get_ref(self);
  return PyAigHandle::ToPyObject(val.make_one());
}

// make INPUT
PyObject*
make_input(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyAigMgr::_get_ref(self);
  return PyAigHandle::ToPyObject(val.make_input());
}

// AND op
PyObject*
and_op(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "fanin_list",
    nullptr
  };
  PyObject* list_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O",
                                    const_cast<char**>(kwlist),
                                    &list_obj) ) {
    return nullptr;
  }
  auto& val = PyAigMgr::_get_ref(self);
  std::vector<AigHandle> fanin_list;
  if ( !PyList<AigHandle, PyAigHandle>::FromPyObject(list_obj, fanin_list) ) {
    PyErr_SetString(PyExc_TypeError, "argument 1 should be a sequence of 'AigHandle'");
    return nullptr;
  }
  return PyAigHandle::ToPyObject(val.and_op(fanin_list));
}

// OR op
PyObject*
or_op(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "fanin_list",
    nullptr
  };
  PyObject* list_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O",
                                    const_cast<char**>(kwlist),
                                    &list_obj) ) {
    return nullptr;
  }
  auto& val = PyAigMgr::_get_ref(self);
  std::vector<AigHandle> fanin_list;
  if ( !PyList<AigHandle, PyAigHandle>::FromPyObject(list_obj, fanin_list) ) {
    PyErr_SetString(PyExc_TypeError, "argument 1 should be a sequence of 'AigHandle'");
    return nullptr;
  }
  return PyAigHandle::ToPyObject(val.or_op(fanin_list));
}

// XOR op
PyObject*
xor_op(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "fanin_list",
    nullptr
  };
  PyObject* list_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O",
                                    const_cast<char**>(kwlist),
                                    &list_obj) ) {
    return nullptr;
  }
  auto& val = PyAigMgr::_get_ref(self);
  std::vector<AigHandle> fanin_list;
  if ( !PyList<AigHandle, PyAigHandle>::FromPyObject(list_obj, fanin_list) ) {
    PyErr_SetString(PyExc_TypeError, "argument 1 should be a sequence of 'AigHandle'");
    return nullptr;
  }
  return PyAigHandle::ToPyObject(val.and_op(fanin_list));
}

// make AIG from EXPR
PyObject*
from_expr(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "expr",
    nullptr
  };
  PyObject* expr_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O!",
                                    const_cast<char**>(kwlist),
                                    PyExpr::_typeobject(), &expr_obj) ) {
    return nullptr;
  }
  Expr expr;
  if ( expr_obj != nullptr ) {
    if ( !PyExpr::FromPyObject(expr_obj, expr) ) {
      PyErr_SetString(PyExc_TypeError, "could not convert to Expr");
      return nullptr;
    }
  }
  auto& val = PyAigMgr::_get_ref(self);
  return PyAigHandle::ToPyObject(val.from_expr(expr));
}

// メソッド定義
PyMethodDef methods[] = {
  {"input",
   reinterpret_cast<PyCFunction>(input),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return specified INPUT")},
  {"zero",
   zero,
   METH_NOARGS,
   PyDoc_STR("make ZERO")},
  {"one",
   one,
   METH_NOARGS,
   PyDoc_STR("make ONE")},
  {"make_input",
   make_input,
   METH_NOARGS,
   PyDoc_STR("make INPUT")},
  {"and_op",
   reinterpret_cast<PyCFunction>(and_op),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("AND op")},
  {"or_op",
   reinterpret_cast<PyCFunction>(or_op),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("OR op")},
  {"xor_op",
   reinterpret_cast<PyCFunction>(xor_op),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("XOR op")},
  {"from_expr",
   reinterpret_cast<PyCFunction>(from_expr),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("make AIG from EXPR")},
  // end-marker
  {nullptr, nullptr, 0, nullptr}
};

PyObject*
get_node_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyAigMgr::_get_ref(self);
  return PyLong::ToPyObject(val.node_num());
}

PyObject*
get_input_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyAigMgr::_get_ref(self);
  return PyLong::ToPyObject(val.input_num());
}

PyObject*
get_and_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyAigMgr::_get_ref(self);
  return PyLong::ToPyObject(val.and_num());
}

// getter/setter定義
PyGetSetDef getsets[] = {
  {"node_num", get_node_num, nullptr, PyDoc_STR(""), nullptr},
  {"input_num", get_input_num, nullptr, PyDoc_STR(""), nullptr},
  {"and_num", get_and_num, nullptr, PyDoc_STR(""), nullptr},
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


// @brief AigMgr オブジェクトを使用可能にする．
bool
PyAigMgr::init(
  PyObject* m
)
{
  AigMgr_Type.tp_name = "AigMgr";
  AigMgr_Type.tp_basicsize = sizeof(AigMgr_Object);
  AigMgr_Type.tp_itemsize = 0;
  AigMgr_Type.tp_dealloc = dealloc_func;
  AigMgr_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  AigMgr_Type.tp_doc = PyDoc_STR("Python extended object for AigMgr");
  AigMgr_Type.tp_methods = methods;
  AigMgr_Type.tp_getset = getsets;
  AigMgr_Type.tp_new = new_func;
  if ( !PyModule::reg_type(m, "AigMgr", &AigMgr_Type) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// AigMgr を PyObject に変換する．
PyObject*
PyAigMgr::Conv::operator()(
  const ElemType& val ///< [in] 元の値
)
{
  auto type = PyAigMgr::_typeobject();
  auto obj = type->tp_alloc(type, 0);
  auto my_obj = reinterpret_cast<AigMgr_Object*>(obj);
  new (&my_obj->mVal) AigMgr(val);
  return obj;
}

// PyObject を AigMgr に変換する．
bool
PyAigMgr::Deconv::operator()(
  PyObject* obj, ///< [in] Python のオブジェクト
  ElemType& val  ///< [out] 結果を格納する変数
)
{
  if ( PyAigMgr::Check(obj) ) {
    val = PyAigMgr::_get_ref(obj);
    return true;
  }
  return false;
}

// @brief PyObject が AigMgr タイプか調べる．
bool
PyAigMgr::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, &AigMgr_Type);
}

// @brief PyObject から AigMgr を取り出す．
AigMgr&
PyAigMgr::_get_ref(
  PyObject* obj
)
{
  auto my_obj = reinterpret_cast<AigMgr_Object*>(obj);
  return my_obj->mVal;
}

// @brief AigMgr を表すオブジェクトの型定義を返す．
PyTypeObject*
PyAigMgr::_typeobject()
{
  return &AigMgr_Type;
}

END_NAMESPACE_YM
