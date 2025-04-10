
/// @file PyNpnMap.cc
/// @brief PyNpnMap の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyNpnMap.h"
#include "pym/PyInt.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
// この構造体は同じサイズのヒープから作られるので
// mVal のコンストラクタは起動されないことに注意．
// そのためあとでコンストラクタを明示的に起動する必要がある．
// またメモリを開放するときにも明示的にデストラクタを起動する必要がある．
struct NpnMap_Object
{
  PyObject_HEAD
  NpnMap mVal;
};

// Python 用のタイプ定義
PyTypeObject NpnMap_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PyNpnMap::init() 中で初期化する．
};

// 終了関数
void
dealloc_func(
  PyObject* self
)
{
  Py_TYPE(self)->tp_free(self);
}

PyObject*
nb_multiply(
  PyObject* self,
  PyObject* other
)
{
  auto& val = PyNpnMap::_get_ref(self);
  if ( PyNpnMap::Check(other) ) {
    auto& val1 = PyNpnMap::_get_ref(self);
    auto& val2 = PyNpnMap::_get_ref(other);
    return PyNpnMap::ToPyObject(val1 * val2);
  }
  Py_RETURN_NOTIMPLEMENTED;
}

PyObject*
nb_invert(
  PyObject* self
)
{
  auto& val = PyNpnMap::_get_ref(self);
  return PyNpnMap::ToPyObject(~val);
}

PyObject*
nb_inplace_multiply(
  PyObject* self,
  PyObject* other
)
{
  auto& val = PyNpnMap::_get_ref(self);
  if ( PyNpnMap::Check(other) ) {
    auto& val1 = PyNpnMap::_get_ref(self);
    auto& val2 = PyNpnMap::_get_ref(other);
    val1 *= val2;
    Py_XINCREF(self);
    return self;
  }
  Py_RETURN_NOTIMPLEMENTED;
}

// Numberオブジェクト構造体
PyNumberMethods number = {
  .nb_multiply = nb_multiply,
  .nb_invert = nb_invert,
  .nb_inplace_multiply = nb_inplace_multiply
};

// make the identity map
PyObject*
identity(
  PyObject* Py_UNUSED(self),
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "",
    "inv",
    nullptr
  };
  int ni = -1;
  int inv_tmp = -1;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "i|$p",
                                    const_cast<char**>(kwlist),
                                    &ni,
                                    &inv_tmp) ) {
    return nullptr;
  }
  bool inv = false;
  if ( inv_tmp != -1 ) {
    inv = static_cast<bool>(inv_tmp);
  }
  return PyNpnMap::ToPyObject(NpnMap::identity(ni, inv));
}

// clear
PyObject*
clear(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyNpnMap::_get_ref(self);
  val.clear();
  Py_RETURN_NONE;
}

// resize
PyObject*
resize(
  PyObject* self,
  PyObject* args
)
{
  int ni1;
  int ni2 = -1;
  if ( !PyArg_ParseTuple(args, "i|i",
                         &ni1,
                         &ni2) ) {
    return nullptr;
  }
  auto& val = PyNpnMap::_get_ref(self);
  if ( ni2 == -1 ) {
    val.resize(ni1);
  }
  else {
    val.resize(ni1, ni2);
  }
  Py_RETURN_NONE;
}

// set itself to identity map
PyObject*
set_identity(
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
  int ni;
  int oinv_tmp = -1;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "i|$p",
                                    const_cast<char**>(kwlist),
                                    &ni,
                                    &oinv_tmp) ) {
    return nullptr;
  }
  bool oinv = false;
  if ( oinv_tmp != -1 ) {
    oinv = static_cast<bool>(oinv_tmp);
  }
  auto& val = PyNpnMap::_get_ref(self);
  val.set_identity(ni, oinv);
  Py_RETURN_NONE;
}

// set input mapping
PyObject*
set_imap(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "src_var",
    "dst_var",
    "inv",
    nullptr
  };
  int src_var;
  int dst_var;
  int inv_tmp = -1;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "ii|$p",
                                    const_cast<char**>(kwlist),
                                    &src_var,
                                    &dst_var,
                                    &inv_tmp) ) {
    return nullptr;
  }
  bool inv = false;
  if ( inv_tmp != -1 ) {
    inv = static_cast<bool>(inv_tmp);
  }
  auto& val = PyNpnMap::_get_ref(self);
  val.set_imap(src_var, dst_var, inv);
  Py_RETURN_NONE;
}

// set output polatity
PyObject*
set_oinv(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "inv",
    nullptr
  };
  int inv_tmp = -1;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "p",
                                    const_cast<char**>(kwlist),
                                    &inv_tmp) ) {
    return nullptr;
  }
  bool inv;
  if ( inv_tmp != -1 ) {
    inv = static_cast<bool>(inv_tmp);
  }
  auto& val = PyNpnMap::_get_ref(self);
  val.set_oinv(inv);
  Py_RETURN_NONE;
}

// get input mapping
PyObject*
imap(
  PyObject* self,
  PyObject* args
)
{
  int var;
  if ( !PyArg_ParseTuple(args, "i",
                         &var) ) {
    return nullptr;
  }
  auto& val = PyNpnMap::_get_ref(self);
  auto imap = val.imap(var);
  auto dst_var = imap.var();
  auto inv = imap.inv();
  return Py_BuildValue("kp", dst_var, inv);
}

// メソッド定義
PyMethodDef methods[] = {
  {"identity",
   reinterpret_cast<PyCFunction>(identity),
   METH_VARARGS | METH_KEYWORDS | METH_STATIC,
   PyDoc_STR("make the identity map")},
  {"clear",
   clear,
   METH_NOARGS,
   PyDoc_STR("clear")},
  {"resize",
   resize,
   METH_VARARGS,
   PyDoc_STR("resize")},
  {"set_identity",
   reinterpret_cast<PyCFunction>(set_identity),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("set itself to identity map")},
  {"set_imap",
   reinterpret_cast<PyCFunction>(set_imap),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("set input mapping")},
  {"set_oinv",
   reinterpret_cast<PyCFunction>(set_oinv),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("set output polatity")},
  {"imap",
   imap,
   METH_VARARGS,
   PyDoc_STR("get input mapping")},
  // end-marker
  {nullptr, nullptr, 0, nullptr}
};

PyObject*
get_input_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyNpnMap::_get_ref(self);
  return PyInt::ToPyObject(val.input_num());
}

PyObject*
get_input_num2(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyNpnMap::_get_ref(self);
  return PyInt::ToPyObject(val.input_num2());
}

PyObject*
get_oinv(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyNpnMap::_get_ref(self);
  return PyBool_FromLong(val.oinv());
}

// getter/setter定義
PyGetSetDef getsets[] = {
  {"input_num", get_input_num, nullptr, PyDoc_STR("number of inputs"), nullptr},
  {"input_num2", get_input_num2, nullptr, PyDoc_STR("number of mapped inputs"), nullptr},
  {"oinv", get_oinv, nullptr, PyDoc_STR("output polarity"), nullptr},
  // end-marker
  {nullptr, nullptr, nullptr, nullptr}
};

// init 関数
int
init_func(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  int ni1 = -1;
  int ni2 = -1;
  if ( !PyArg_ParseTuple(args, "|ii",
                         &ni1,
                         &ni2) ) {
    return -1;
  }
  auto& npnmap = PyNpnMap::_get_ref(self);
  if ( ni1 == -1 ) {
    npnmap = NpnMap();
  }
  else if ( ni2 == -1 ) {
    npnmap = NpnMap(ni1);
  }
  else {
    npnmap = NpnMap(ni1, ni2);
  }
  return 0;
}

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
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "",
                                    const_cast<char**>(kwlist),
    return nullptr;
  }
  auto self = type->tp_alloc(type, 0);
  return self;
}

END_NONAMESPACE


// @brief NpnMap オブジェクトを使用可能にする．
bool
PyNpnMap::init(
  PyObject* m
)
{
  NpnMap_Type.tp_name = "NpnMap";
  NpnMap_Type.tp_basicsize = sizeof(NpnMap_Object);
  NpnMap_Type.tp_itemsize = 0;
  NpnMap_Type.tp_dealloc = dealloc_func;
  NpnMap_Type.tp_as_number = &number;
  NpnMap_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  NpnMap_Type.tp_doc = PyDoc_STR("Python extended object for NpnMap");
  NpnMap_Type.tp_methods = methods;
  NpnMap_Type.tp_getset = getsets;
  NpnMap_Type.tp_init = init_func;
  NpnMap_Type.tp_new = new_func;
  if ( !PyModule::reg_type(m, "NpnMap", &NpnMap_Type) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// NpnMap を PyObject に変換する．
PyObject*
PyNpnMap::Conv::operator()(
  const NpnMap& val
)
{
  auto type = PyNpnMap::_typeobject();
  auto obj = type->tp_alloc(type, 0);
  auto my_obj = reinterpret_cast<NpnMap_Object*>(obj);
  new (&my_obj->mVal) NpnMap(val);
  return obj;
}

// PyObject を NpnMap に変換する．
bool
PyNpnMap::Deconv::operator()(
  PyObject* obj,
  NpnMap& val
)
{
  if ( PyNpnMap::Check(obj) ) {
    val = PyNpnMap::_get_ref(obj);
    return true;
  }
  return false;
}

// @brief PyObject が NpnMap タイプか調べる．
bool
PyNpnMap::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, &NpnMap_Type);
}

// @brief PyObject から NpnMap を取り出す．
NpnMap&
PyNpnMap::_get_ref(
  PyObject* obj
)
{
  auto my_obj = reinterpret_cast<NpnMap_Object*>(obj);
  return my_obj->mVal;
}

// @brief NpnMap を表すオブジェクトの型定義を返す．
PyTypeObject*
PyNpnMap::_typeobject()
{
  return &NpnMap_Type;
}

END_NAMESPACE_YM
