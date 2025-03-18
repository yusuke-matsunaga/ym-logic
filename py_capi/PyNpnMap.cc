
/// @file PyNpnMap.cc
/// @brief Python NpnMap の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyNpnMap.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
struct NpnMapObject
{
  PyObject_HEAD
  NpnMap mVal;
};

// Python 用のタイプ定義
PyTypeObject NpnMapType = {
  PyVarObject_HEAD_INIT(nullptr, 0)
};

// 生成関数
PyObject*
NpnMap_new(
  PyTypeObject* type,
  PyObject* Py_UNUSED(args),
  PyObject* Py_UNUSED(kwds)
)
{
  auto self = type->tp_alloc(type, 0);
  // auto npnmap_obj = reinterpret_cast<NpnMapObject*>(self);
  // 必要なら npnmap_obj->mVal の初期化を行う．
  return self;
}

// 終了関数
void
NpnMap_dealloc(
  PyObject* self
)
{
  // auto npnmap_obj = reinterpret_cast<NpnMapObject*>(self);
  // 必要なら npnmap_obj->mVal の終了処理を行う．
  Py_TYPE(self)->tp_free(self);
}

// 初期化関数(__init__()相当)
int
NpnMap_init(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  // 名前付き引数はない．
  static const char* kwlist[] = {
    "",
    "",
    nullptr
  };
  SizeType ni1 = -1;
  SizeType ni2 = -1;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "|kk",
				    const_cast<char**>(kwlist),
				    &ni1, &ni2) ) {
    return -1;
  }
  auto npnmap_obj = reinterpret_cast<NpnMapObject*>(self);
  auto& npnmap = npnmap_obj->mVal;
  if ( ni1 == -1 ) {
    npnmap = NpnMap{};
  }
  else if ( ni2 == -1 ) {
    npnmap = NpnMap{ni1};
  }
  else {
    npnmap = NpnMap{ni1, ni2};
  }
  return 0;
}

// repr() 関数
PyObject*
NpnMap_repr(
  PyObject* self
)
{
  auto& val = PyNpnMap::_get_ref(self);
  // val から 文字列を作る．
  const char* tmp_str = nullptr;
  return Py_BuildValue("s", tmp_str);
}

PyObject*
NpnMap_identity(
  PyObject* Py_UNUSED(self),
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "",
    "oinv",
    nullptr
  };
  SizeType ni = -1;
  int inv = false;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "k|$p",
				    const_cast<char**>(kwlist),
				    &ni, &inv) ) {
    return nullptr;
  }
  return PyNpnMap::ToPyObject(NpnMap::identity(ni, static_cast<bool>(inv)));
}

PyObject*
NpnMap_clear(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& npnmap = PyNpnMap::_get_ref(self);
  npnmap.clear();
  Py_RETURN_NONE;
}

PyObject*
NpnMap_resize(
  PyObject* self,
  PyObject* args
)
{
  SizeType ni1 = -1;
  SizeType ni2 = -1;
  if ( !PyArg_ParseTuple(args, "k|k", &ni1, &ni2) ) {
    return nullptr;
  }
  auto& npnmap = PyNpnMap::_get_ref(self);
  if ( ni2 == -1 ) {
    npnmap.resize(ni1);
  }
  else {
    npnmap.resize(ni1, ni2);
  }
  Py_RETURN_NONE;
}

PyObject*
NpnMap_set_identity(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "",
    "oinv",
    nullptr
  };
  SizeType ni = -1;
  int oinv = false;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "k|$p",
				    const_cast<char**>(kwlist),
				    &ni, &oinv) ) {
    return nullptr;
  }
  auto& npnmap = PyNpnMap::_get_ref(self);
  npnmap.set_identity(ni, oinv);
  Py_RETURN_NONE;
}

PyObject*
NpnMap_set(
  PyObject* self,
  PyObject* args
)
{
  SizeType src_var = -1;
  SizeType dst_var = -1;
  int inv = false;
  if ( !PyArg_ParseTuple(args, "kkp", &src_var, &dst_var, &inv) ) {
    return nullptr;
  }
  auto& npnmap = PyNpnMap::_get_ref(self);
  npnmap.set(src_var, dst_var, static_cast<bool>(inv));
  Py_RETURN_NONE;
}

PyObject*
NpnMap_set_oinv(
  PyObject* self,
  PyObject* args
)
{
  int inv = false;
  if ( !PyArg_ParseTuple(args, "p", &inv) ) {
    return nullptr;
  }
  auto& npnmap = PyNpnMap::_get_ref(self);
  npnmap.set_oinv(static_cast<bool>(inv));
  Py_RETURN_NONE;
}

PyObject*
NpnMap_imap(
  PyObject* self,
  PyObject* args
)
{
  SizeType src_var = -1;
  if ( !PyArg_ParseTuple(args, "k", &src_var) ) {
    return nullptr;
  }
  auto& npnmap = PyNpnMap::_get_ref(self);
  auto vmap = npnmap.imap(src_var);
  auto dst_var = vmap.var();
  auto inv = vmap.inv();
  return Py_BuildValue("kp", dst_var, inv);
}

// メソッド定義
PyMethodDef NpnMap_methods[] = {
  {"identity", reinterpret_cast<PyCFunction>(NpnMap_identity),
   METH_STATIC | METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("make the identity map")},
  {"clear", NpnMap_clear, METH_NOARGS,
   PyDoc_STR("clear")},
  {"resize", NpnMap_resize, METH_VARARGS,
   PyDoc_STR("resize")},
  {"set_identity", reinterpret_cast<PyCFunction>(NpnMap_set_identity),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("set to the identity map")},
  {"set", NpnMap_set, METH_VARARGS,
   PyDoc_STR("set input map")},
  {"set_ovin", NpnMap_set, METH_VARARGS,
   PyDoc_STR("set output invert")},
  {"imap", NpnMap_imap, METH_VARARGS,
   PyDoc_STR("get input map")},
  {nullptr, nullptr, 0, nullptr}
};

PyObject*
NpnMap_input_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& npnmap = PyNpnMap::_get_ref(self);
  auto val = npnmap.input_num();
  return PyLong_FromLong(val);
}

PyObject*
NpnMap_input_num2(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& npnmap = PyNpnMap::_get_ref(self);
  auto val = npnmap.input_num2();
  return PyLong_FromLong(val);
}

PyObject*
NpnMap_oinv(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& npnmap = PyNpnMap::_get_ref(self);
  auto val = npnmap.oinv();
  return PyBool_FromLong(val);
}

// get/set 関数定義
PyGetSetDef NpnMap_getset[] = {
  {"input_num", NpnMap_input_num, nullptr, PyDoc_STR("input number"), nullptr},
  {"input_num2", NpnMap_input_num2, nullptr, PyDoc_STR("mapped input number"), nullptr},
  {"oinv", NpnMap_oinv, nullptr, PyDoc_STR("output invert"), nullptr},
  {nullptr, nullptr, nullptr, nullptr, nullptr},
};

// 比較関数
PyObject*
NpnMap_richcmpfunc(
  PyObject* self,
  PyObject* other,
  int op
)
{
  if ( PyNpnMap::_check(self) &&
       PyNpnMap::_check(other) ) {
    auto& val1 = PyNpnMap::_get_ref(self);
    auto& val2 = PyNpnMap::_get_ref(other);
    if ( op == Py_EQ ) {
      return PyBool_FromLong(val1 == val2);
    }
    if ( op == Py_NE ) {
      return PyBool_FromLong(val1 != val2);
    }
  }
  Py_INCREF(Py_NotImplemented);
  return Py_NotImplemented;
}

// 否定演算(単項演算の例)
PyObject*
NpnMap_invert(
  PyObject* self
)
{
  if ( PyNpnMap::_check(self) ) {
    auto& val = PyNpnMap::_get_ref(self);
    return PyNpnMap::ToPyObject(~val);
  }
  Py_RETURN_NOTIMPLEMENTED;
}

// 乗算(合成演算)
PyObject*
NpnMap_mul(
  PyObject* self,
  PyObject* other
)
{
  if ( PyNpnMap::_check(self) &&
       PyNpnMap::_check(other) ) {
    auto& val1 = PyNpnMap::_get_ref(self);
    auto& val2 = PyNpnMap::_get_ref(other);
    return PyNpnMap::ToPyObject(val1 * val2);
  }
  Py_RETURN_NOTIMPLEMENTED;
}

// 数値演算メソッド定義
PyNumberMethods NpnMap_number = {
  .nb_multiply = NpnMap_mul,
  .nb_invert = NpnMap_invert,
  .nb_inplace_multiply = NpnMap_mul
};

END_NONAMESPACE


// @brief 'NpnMap' オブジェクトを使用可能にする．
bool
PyNpnMap::init(
  PyObject* m
)
{
  NpnMapType.tp_name = "NpnMap";
  NpnMapType.tp_basicsize = sizeof(NpnMapObject);
  NpnMapType.tp_itemsize = 0;
  NpnMapType.tp_dealloc = NpnMap_dealloc;
  NpnMapType.tp_flags = Py_TPFLAGS_DEFAULT;
  NpnMapType.tp_doc = PyDoc_STR("NpnMap object");
  NpnMapType.tp_richcompare = NpnMap_richcmpfunc;
  NpnMapType.tp_methods = NpnMap_methods;
  NpnMapType.tp_getset = NpnMap_getset;
  NpnMapType.tp_init = NpnMap_init;
  NpnMapType.tp_new = NpnMap_new;
  NpnMapType.tp_repr = NpnMap_repr;
  NpnMapType.tp_as_number = &NpnMap_number;

  // 型オブジェクトの登録
  if ( !PyModule::reg_type(m, "NpnMap", &NpnMapType) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// @brief NpnMap を PyObject に変換する．
PyObject*
PyNpnMapConv::operator()(
  const NpnMap& val
)
{
  auto obj = NpnMapType.tp_alloc(&NpnMapType, 0);
  auto npnmap_obj = reinterpret_cast<NpnMapObject*>(obj);
  npnmap_obj->mVal = val;
  return obj;
}

// @brief PyObject* から NpnMap を取り出す．
bool
PyNpnMapDeconv::operator()(
  PyObject* obj,
  NpnMap& val
)
{
  if ( PyNpnMap::_check(obj) ) {
    val = PyNpnMap::_get_ref(obj);
    return true;
  }
  return false;
}

// @brief PyObject が NpnMap タイプか調べる．
bool
PyNpnMap::_check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, _typeobject());
}

// @brief NpnMap を表す PyObject から NpnMap を取り出す．
NpnMap&
PyNpnMap::_get_ref(
  PyObject* obj
)
{
  auto npnmap_obj = reinterpret_cast<NpnMapObject*>(obj);
  return npnmap_obj->mVal;
}

// @brief NpnMap を表すオブジェクトの型定義を返す．
PyTypeObject*
PyNpnMap::_typeobject()
{
  return &NpnMapType;
}

END_NAMESPACE_YM
