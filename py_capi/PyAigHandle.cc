
/// @file PyAigHandle.cc
/// @brief Python AigHandle の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyAigHandle.h"
#include "pym/PyJsonValue.h"
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
  auto h = PyAigHandle::_get_ref(self);
  auto ans = h.inv();
  return PyBool_FromLong(ans);
}

PyObject*
AigHandle_is_zero(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto h = PyAigHandle::_get_ref(self);
  auto ans = h.is_zero();
  return PyBool_FromLong(ans);
}

PyObject*
AigHandle_is_one(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto h = PyAigHandle::_get_ref(self);
  auto ans = h.is_one();
  return PyBool_FromLong(ans);
}

PyObject*
AigHandle_is_const(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto h = PyAigHandle::_get_ref(self);
  auto ans = h.is_const();
  return PyBool_FromLong(ans);
}

PyObject*
AigHandle_is_input(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto h = PyAigHandle::_get_ref(self);
  auto ans = h.is_input();
  return PyBool_FromLong(ans);
}

PyObject*
AigHandle_input_id(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto h = PyAigHandle::_get_ref(self);
  auto ans = h.input_id();
  return PyLong_FromLong(ans);
}

PyObject*
AigHandle_is_and(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto h = PyAigHandle::_get_ref(self);
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
  auto h = PyAigHandle::_get_ref(self);
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
  auto h = PyAigHandle::_get_ref(self);
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
  auto h = PyAigHandle::_get_ref(self);
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
  auto h = PyAigHandle::_get_ref(self);
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

PyObject*
AigHandle_gen_dot(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kw_list[] = {
    "filename",
    "option",
    nullptr
  };
  const char* filename = nullptr;
  PyObject* option_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "s|$O",
				    const_cast<char**>(kw_list),
				    &filename,
				    &option_obj) ) {
    return nullptr;
  }

  ofstream ofs{filename};
  if ( !ofs ) {
    ostringstream buf;
    buf << "Could not create file '" << filename << "'";
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
  JsonValue option;
  if ( option_obj != nullptr ) {
    PyJsonValue::Deconv json_dec;
    if ( !json_dec(option_obj, option) ) {
      PyErr_SetString(PyExc_TypeError, "'option' should be a JsonValue type");
      return nullptr;
    }
  }
  auto aig = PyAigHandle::_get_ref(self);
  aig.gen_dot(ofs, option);
  Py_RETURN_NONE;
}

PyObject*
AigHandle_gen_dot2(
  PyObject* Py_UNUSED(self),
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kw_list[] = {
    "filename",
    "aig_list",
    "option",
    nullptr
  };
  const char* filename = nullptr;
  PyObject* aig_list_obj = nullptr;
  PyObject* option_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "sO|$O",
				    const_cast<char**>(kw_list),
				    &filename,
				    &aig_list_obj,
				    &option_obj) ) {
    return nullptr;
  }
  const char* emsg = "'aig_list' shuld be a list of AigHandle";
  if ( !PySequence_Check(aig_list_obj) ) {
    PyErr_SetString(PyExc_TypeError, emsg);
    return nullptr;
  }
  auto n = PySequence_Size(aig_list_obj);
  vector<AigHandle> aig_list(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    auto aig_obj = PySequence_GetItem(aig_list_obj, i);
    if ( !PyAigHandle::Check(aig_obj) ) {
      PyErr_SetString(PyExc_TypeError, emsg);
      return nullptr;
    }
    auto aig = PyAigHandle::_get_ref(aig_obj);
    aig_list[i] = aig;
    Py_DECREF(aig_obj);
  }

  ofstream ofs{filename};
  if ( !ofs ) {
    ostringstream buf;
    buf << "Could not create file '" << filename << "'";
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
  JsonValue option;
  if ( option_obj != nullptr ) {
    PyJsonValue::Deconv json_dec;
    if ( !json_dec(option_obj, option) ) {
      PyErr_SetString(PyExc_TypeError, "'option' should be a JsonValue type");
      return nullptr;
    }
  }
  AigHandle::gen_dot(ofs, aig_list, option);
  Py_RETURN_NONE;
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
  {"gen_dot", reinterpret_cast<PyCFunction>(AigHandle_gen_dot),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("gen DOT file")},
  {"gen_dot2", reinterpret_cast<PyCFunction>(AigHandle_gen_dot2),
   METH_VARARGS | METH_KEYWORDS | METH_STATIC,
   PyDoc_STR("gen DOT file")},
  {nullptr, nullptr, 0, nullptr}
};

PyObject*
AigHandle_index(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto h = PyAigHandle::_get_ref(self);
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
    auto val1 = PyAigHandle::_get_ref(self);
    auto val2 = PyAigHandle::_get_ref(other);
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
    auto h = PyAigHandle::_get_ref(self);
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
  if ( PyAigHandle::Check(self) &&
       PyBool_Check(other) ) {
    auto h = PyAigHandle::_get_ref(self);
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
  if ( PyAigHandle::Check(self) &&
       PyAigHandle::Check(other) ) {
    auto h1 = PyAigHandle::_get_ref(self);
    auto h2 = PyAigHandle::_get_ref(other);
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
  if ( PyAigHandle::Check(self) &&
       PyAigHandle::Check(other) ) {
    auto h1 = PyAigHandle::_get_ref(self);
    auto h2 = PyAigHandle::_get_ref(other);
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
  if ( PyAigHandle::Check(self) &&
       PyAigHandle::Check(other) ) {
    auto h1 = PyAigHandle::_get_ref(self);
    auto h2 = PyAigHandle::_get_ref(other);
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
  auto h = PyAigHandle::_get_ref(self);
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
PyAigHandle::Conv::operator()(
  const AigHandle& val
)
{
  auto obj = AigHandleType.tp_alloc(&AigHandleType, 0);
  auto aighandle_obj = reinterpret_cast<AigHandleObject*>(obj);
  new (&aighandle_obj->mHandle) AigHandle{val};
  return obj;
}

// @brief PyObject* から AigHandle を取り出す．
bool
PyAigHandle::Deconv::operator()(
  PyObject* obj,
  AigHandle& val
)
{
  if ( PyAigHandle::Check(obj) ) {
    val = PyAigHandle::_get_ref(obj);
    return true;
  }
  return false;
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
AigHandle&
PyAigHandle::_get_ref(
  PyObject* obj
)
{
  auto aighandle_obj = reinterpret_cast<AigHandleObject*>(obj);
  return aighandle_obj->mHandle;
}

// @brief AigHandle を表すオブジェクトの型定義を返す．
PyTypeObject*
PyAigHandle::_typeobject()
{
  return &AigHandleType;
}

END_NAMESPACE_YM
