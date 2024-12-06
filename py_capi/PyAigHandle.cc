
/// @file PyAigHandle.cc
/// @brief Python AigHandle の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyAigHandle.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
struct AigHandleObject
{
  PyObject_HEAD
  AigHandle mHandle;
};

// Python 用のタイプ定義
PyTypeObject AigHandleType = {
  PyVarObject_HEAD_INIT(nullptr, 0)
};

// 生成関数
PyObject*
AigHandle_new(
  PyTypeObject* type,
  PyObject* args,
  PyObject* kwds
)
{
  PyErr_SetString(PyExc_TypeError, "instantiation of 'AigHandle' is disabled");
  return nullptr;
}

// 終了関数
void
AigHandle_dealloc(
  PyObject* self
)
{
  auto aighandle_obj = reinterpret_cast<AigHandleObject*>(self);
  aighandle_obj->mHandle.~AigHandle();
  Py_TYPE(self)->tp_free(self);
}

PyObject*
AigHandle_inv(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto h = PyAigHandle::Get(self);
  auto ans = h.inv();
  return PyBool_FromLong(ans);
}

PyObject*
AigHandle_is_zero(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto h = PyAigHandle::Get(self);
  auto ans = h.is_zero();
  return PyBool_FromLong(ans);
}

PyObject*
AigHandle_is_one(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto h = PyAigHandle::Get(self);
  auto ans = h.is_one();
  return PyBool_FromLong(ans);
}

PyObject*
AigHandle_is_const(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto h = PyAigHandle::Get(self);
  auto ans = h.is_const();
  return PyBool_FromLong(ans);
}

PyObject*
AigHandle_is_input(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto h = PyAigHandle::Get(self);
  auto ans = h.is_input();
  return PyBool_FromLong(ans);
}

PyObject*
AigHandle_input_id(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto h = PyAigHandle::Get(self);
  auto ans = h.input_id();
  return PyLong_FromLong(ans);
}

PyObject*
AigHandle_is_and(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto h = PyAigHandle::Get(self);
  auto ans = h.is_and();
  return PyBool_FromLong(ans);
}

PyObject*
AigHandle_fanin(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kw_list[] = {
    "pos",
    nullptr
  };
  SizeType pos = 0;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "k",
				    const_cast<char**>(kw_list),
				    &pos) ) {
    return nullptr;
  }
  auto h = PyAigHandle::Get(self);
  try {
    auto ans = h.fanin(pos);
    return PyAigHandle::ToPyObject(ans);
  }
  catch ( std::invalid_argument err ) {
    PyErr_SetString(PyExc_ValueError, err.what());
    return nullptr;
  }
}

PyObject*
AigHandle_fanin0(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto h = PyAigHandle::Get(self);
  try {
    auto ans = h.fanin0();
    return PyAigHandle::ToPyObject(ans);
  }
  catch ( std::invalid_argument err ) {
    PyErr_SetString(PyExc_ValueError, err.what());
    return nullptr;
  }
}

PyObject*
AigHandle_fanin1(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto h = PyAigHandle::Get(self);
  try {
    auto ans = h.fanin1();
    return PyAigHandle::ToPyObject(ans);
  }
  catch ( std::invalid_argument err ) {
    PyErr_SetString(PyExc_ValueError, err.what());
    return nullptr;
  }
}

PyObject*
AigHandle_ex_fanin_list(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto h = PyAigHandle::Get(self);
  try {
    auto fanin_list = h.ex_fanin_list();
    auto n = fanin_list.size();
    auto ans_obj = PyList_New(n);
    for ( SizeType i = 0; i < n; ++ i ) {
      auto h = fanin_list[i];
      auto obj = PyAigHandle::ToPyObject(h);
      PyList_SetItem(ans_obj, i, obj);
    }
    return ans_obj;
  }
  catch ( std::invalid_argument err ) {
    PyErr_SetString(PyExc_ValueError, err.what());
    return nullptr;
  }
}

// メソッド定義
PyMethodDef AigHandle_methods[] = {
  {"inv", AigHandle_inv,
   METH_NOARGS,
   PyDoc_STR("check if inverted")},
  {"is_zero", AigHandle_is_zero,
   METH_NOARGS,
   PyDoc_STR("check if ZERO")},
  {"is_one", AigHandle_is_one,
   METH_NOARGS,
   PyDoc_STR("check if ONE")},
  {"is_const", AigHandle_is_const,
   METH_NOARGS,
   PyDoc_STR("check if constant")},
  {"is_input", AigHandle_is_input,
   METH_NOARGS,
   PyDoc_STR("check if INPUT")},
  {"input_id", AigHandle_input_id,
   METH_NOARGS,
   PyDoc_STR("returns input_id")},
  {"is_and", AigHandle_is_and,
   METH_NOARGS,
   PyDoc_STR("check if AND")},
  {"fanin", reinterpret_cast<PyCFunction>(AigHandle_fanin),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("returns a fanin's handle")},
  {"fanin0", AigHandle_fanin0,
   METH_NOARGS,
   PyDoc_STR("returns a fanin0's handle")},
  {"fanin1", AigHandle_fanin1,
   METH_NOARGS,
   PyDoc_STR("returns a fanin1's handle")},
  {"ex_fanin_list", AigHandle_ex_fanin_list,
   METH_NOARGS,
   PyDoc_STR("returns an expanded fanin list")},
  {nullptr, nullptr, 0, nullptr}
};

PyObject*
AigHandle_index(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto h = PyAigHandle::Get(self);
  auto ans = h.index();
  return PyLong_FromLong(ans);
}

// get/set 関数定義
PyGetSetDef AigHandle_getset[] = {
  {"index", AigHandle_index, nullptr, PyDoc_STR("index"), nullptr},
  {nullptr, nullptr, nullptr, nullptr, nullptr},
};

// 比較関数
PyObject*
AigHandle_richcmpfunc(
  PyObject* self,
  PyObject* other,
  int op
)
{
  if ( PyAigHandle::Check(self) &&
       PyAigHandle::Check(other) ) {
    auto val1 = PyAigHandle::Get(self);
    auto val2 = PyAigHandle::Get(other);
    Py_RETURN_RICHCOMPARE(val1, val2, op);
  }
  Py_RETURN_NOTIMPLEMENTED;
}

// 否定演算
PyObject*
AigHandle_invert(
  PyObject* self
)
{
  if ( PyAigHandle::Check(self) ) {
    auto h = PyAigHandle::Get(self);
    return PyAigHandle::ToPyObject(~h);
  }
  Py_RETURN_NOTIMPLEMENTED;
}

// 反転属性との乗算
PyObject*
AigHandle_mult(
  PyObject* self,
  PyObject* other
)
{
  if ( PyAigHandle::Check(self) && PyBool_Check(other) ) {
    auto h = PyAigHandle::Get(self);
    auto inv = (other == Py_True);
    return PyAigHandle::ToPyObject(h * inv);
  }
  Py_RETURN_NOTIMPLEMENTED;
}

// AND 演算
PyObject*
AigHandle_and(
  PyObject* self,
  PyObject* other
)
{
  if ( PyAigHandle::Check(self) && PyAigHandle::Check(other) ) {
    auto h1 = PyAigHandle::Get(self);
    auto h2 = PyAigHandle::Get(other);
    return PyAigHandle::ToPyObject(h1 & h2);
  }
  Py_RETURN_NOTIMPLEMENTED;
}

// OR 演算
PyObject*
AigHandle_or(
  PyObject* self,
  PyObject* other
)
{
  if ( PyAigHandle::Check(self) && PyAigHandle::Check(other) ) {
    auto h1 = PyAigHandle::Get(self);
    auto h2 = PyAigHandle::Get(other);
    return PyAigHandle::ToPyObject(h1 | h2);
  }
  Py_RETURN_NOTIMPLEMENTED;
}

// XOR 演算
PyObject*
AigHandle_xor(
  PyObject* self,
  PyObject* other
)
{
  if ( PyAigHandle::Check(self) && PyAigHandle::Check(other) ) {
    auto h1 = PyAigHandle::Get(self);
    auto h2 = PyAigHandle::Get(other);
    return PyAigHandle::ToPyObject(h1 ^ h2);
  }
  Py_RETURN_NOTIMPLEMENTED;
}

// 数値演算メソッド定義
PyNumberMethods AigHandle_number = {
  .nb_multiply = AigHandle_mult,
  .nb_invert = AigHandle_invert,
  .nb_and = AigHandle_and,
  .nb_xor = AigHandle_xor,
  .nb_or = AigHandle_or,
  .nb_inplace_multiply = AigHandle_mult,
  .nb_inplace_and = AigHandle_and,
  .nb_inplace_xor = AigHandle_xor,
  .nb_inplace_or = AigHandle_or
};

// ハッシュ関数
Py_hash_t
AigHandle_hash(
  PyObject* self
)
{
  auto h = PyAigHandle::Get(self);
  return h.index();
}

END_NONAMESPACE


// @brief 'AigHandle' オブジェクトを使用可能にする．
bool
PyAigHandle::init(
  PyObject* m
)
{
  AigHandleType.tp_name = "AigHandle";
  AigHandleType.tp_basicsize = sizeof(AigHandleObject);
  AigHandleType.tp_itemsize = 0;
  AigHandleType.tp_dealloc = AigHandle_dealloc;
  AigHandleType.tp_flags = Py_TPFLAGS_DEFAULT;
  AigHandleType.tp_doc = PyDoc_STR("AigHandle object");
  AigHandleType.tp_richcompare = AigHandle_richcmpfunc;
  AigHandleType.tp_methods = AigHandle_methods;
  AigHandleType.tp_getset = AigHandle_getset;
  AigHandleType.tp_new = AigHandle_new;
  AigHandleType.tp_as_number = &AigHandle_number;
  AigHandleType.tp_hash = AigHandle_hash;

  // 型オブジェクトの登録
  if ( !PyModule::reg_type(m, "AigHandle", &AigHandleType) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// @brief AigHandle を PyObject に変換する．
PyObject*
PyAigHandle::ToPyObject(
  const AigHandle& val
)
{
  auto obj = AigHandleType.tp_alloc(&AigHandleType, 0);
  auto aighandle_obj = reinterpret_cast<AigHandleObject*>(obj);
  new (&aighandle_obj->mHandle) AigHandle{val};
  return obj;
}

// @brief AigHandle のリストを表す PyObject を作る．
PyObject*
PyAigHandle::ToPyList(
  const vector<AigHandle>& handle_list
)
{
  auto n = handle_list.size();
  auto ans_obj = PyList_New(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    auto obj = ToPyObject(handle_list[i]);
    PyList_SetItem(ans_obj, i, obj);
  }
  return ans_obj;
}

// @brief PyObject が AigHandle タイプか調べる．
bool
PyAigHandle::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, _typeobject());
}

// @brief AigHandle を表す PyObject から AigHandle を取り出す．
AigHandle
PyAigHandle::Get(
  PyObject* obj
)
{
  auto aighandle_obj = reinterpret_cast<AigHandleObject*>(obj);
  return aighandle_obj->mHandle;
}

// @brief AigHandle のリストを表す PyObject からの変換
bool
PyAigHandle::ConvFromPyList(
  PyObject* list_obj,
  vector<AigHandle>& handle_list
)
{
  if ( !PySequence_Check(list_obj) ) {
    return false;
  }
  auto n = PySequence_Size(list_obj);
  vector<AigHandle> fanin_list(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    auto obj = PySequence_GetItem(list_obj, i);
    auto res = Check(obj);
    if ( res ) {
      auto h = Get(obj);
      fanin_list[i] = h;
    }
    Py_DECREF(obj);
    if ( !res ) {
      return false;
    }
  }
  return true;
}

// @brief AigHandle を表すオブジェクトの型定義を返す．
PyTypeObject*
PyAigHandle::_typeobject()
{
  return &AigHandleType;
}

END_NAMESPACE_YM
