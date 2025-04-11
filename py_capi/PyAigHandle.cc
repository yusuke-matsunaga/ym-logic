
/// @file PyAigHandle.cc
/// @brief PyAigHandle の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyAigHandle.h"
#include "pym/PyLong.h"
#include "pym/PyBool.h"
#include "pym/PyList.h"
#include "pym/PyJsonValue.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
// この構造体は同じサイズのヒープから作られるので
// mVal のコンストラクタは起動されないことに注意．
// そのためあとでコンストラクタを明示的に起動する必要がある．
// またメモリを開放するときにも明示的にデストラクタを起動する必要がある．
struct AigHandle_Object
{
  PyObject_HEAD
  AigHandle mVal;
};

// Python 用のタイプ定義
PyTypeObject AigHandle_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PyAigHandle::init() 中で初期化する．
};

// 終了関数
void
dealloc_func(
  PyObject* self
)
{
  auto obj = reinterpret_cast<AigHandle_Object*>(self);
  obj->mVal.~AigHandle();
  Py_TYPE(self)->tp_free(self);
}

PyObject*
nb_multiply(
  PyObject* self,
  PyObject* other
)
{
  if ( PyAigHandle::Check(self) ) {
    auto& val1 = PyAigHandle::_get_ref(self);
    if ( PyBool_Check(other) ) {
      auto inv = PyBool::Get(other);
      return PyAigHandle::ToPyObject(val1 * inv);
    }
  }
  Py_RETURN_NOTIMPLEMENTED;
}

PyObject*
nb_invert(
  PyObject* self
)
{
  auto& val = PyAigHandle::_get_ref(self);
  return PyAigHandle::ToPyObject(~val);
}

PyObject*
nb_and(
  PyObject* self,
  PyObject* other
)
{
  if ( PyAigHandle::Check(self) ) {
    auto& val1 = PyAigHandle::_get_ref(self);
    if ( PyAigHandle::Check(other) ) {
      auto& val2 = PyAigHandle::_get_ref(other);
      return PyAigHandle::ToPyObject(val1 & val2);
    }
  }
  Py_RETURN_NOTIMPLEMENTED;
}

PyObject*
nb_xor(
  PyObject* self,
  PyObject* other
)
{
  if ( PyAigHandle::Check(self) ) {
    auto& val1 = PyAigHandle::_get_ref(self);
    if ( PyAigHandle::Check(other) ) {
      auto& val2 = PyAigHandle::_get_ref(other);
      return PyAigHandle::ToPyObject(val1 ^ val2);
    }
  }
  Py_RETURN_NOTIMPLEMENTED;
}

PyObject*
nb_or(
  PyObject* self,
  PyObject* other
)
{
  if ( PyAigHandle::Check(self) ) {
    auto& val1 = PyAigHandle::_get_ref(self);
    if ( PyAigHandle::Check(other) ) {
      auto& val2 = PyAigHandle::_get_ref(other);
      return PyAigHandle::ToPyObject(val1 | val2);
    }
  }
  Py_RETURN_NOTIMPLEMENTED;
}

PyObject*
nb_inplace_and(
  PyObject* self,
  PyObject* other
)
{
  if ( PyAigHandle::Check(self) ) {
    auto& val1 = PyAigHandle::_get_ref(self);
    if ( PyAigHandle::Check(other) ) {
      auto& val2 = PyAigHandle::_get_ref(other);
      val1 &= val2;
      Py_XINCREF(self);
      return self;
    }
  }
  Py_RETURN_NOTIMPLEMENTED;
}

PyObject*
nb_inplace_xor(
  PyObject* self,
  PyObject* other
)
{
  if ( PyAigHandle::Check(self) ) {
    auto& val1 = PyAigHandle::_get_ref(self);
    if ( PyAigHandle::Check(other) ) {
      auto& val2 = PyAigHandle::_get_ref(other);
      val1 ^= val2;
      Py_XINCREF(self);
      return self;
    }
  }
  Py_RETURN_NOTIMPLEMENTED;
}

PyObject*
nb_inplace_or(
  PyObject* self,
  PyObject* other
)
{
  if ( PyAigHandle::Check(self) ) {
    auto& val1 = PyAigHandle::_get_ref(self);
    if ( PyAigHandle::Check(other) ) {
      auto& val2 = PyAigHandle::_get_ref(other);
      val1 |= val2;
      Py_XINCREF(self);
      return self;
    }
  }
  Py_RETURN_NOTIMPLEMENTED;
}

// Numberオブジェクト構造体
PyNumberMethods number = {
  .nb_multiply = nb_multiply,
  .nb_invert = nb_invert,
  .nb_and = nb_and,
  .nb_xor = nb_xor,
  .nb_or = nb_or,
  .nb_inplace_and = nb_inplace_and,
  .nb_inplace_xor = nb_inplace_xor,
  .nb_inplace_or = nb_inplace_or
};

// hash 関数
Py_hash_t
hash_func(
  PyObject* self
)
{
  auto& val = PyAigHandle::_get_ref(self);
  return val.hash();
}

// richcompare 関数
PyObject*
richcompare_func(
  PyObject* self,
  PyObject* other,
  int op
)
{
  auto& val = PyAigHandle::_get_ref(self);
  if ( PyAigHandle::Check(other) ) {
    auto& val2 = PyAigHandle::_get_ref(other);
    Py_RETURN_RICHCOMPARE(val, val2, op);
  }
  Py_RETURN_NOTIMPLEMENTED;
}

// return INV flag
PyObject*
inv(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyAigHandle::_get_ref(self);
  return PyBool_FromLong(val.inv());
}

// True if ZERO
PyObject*
is_zero(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyAigHandle::_get_ref(self);
  return PyBool_FromLong(val.is_zero());
}

// True if ONE
PyObject*
is_one(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyAigHandle::_get_ref(self);
  return PyBool_FromLong(val.is_one());
}

// True if constant(ZERO/ONE)
PyObject*
is_const(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyAigHandle::_get_ref(self);
  return PyBool_FromLong(val.is_const());
}

// True if INPUT
PyObject*
is_input(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyAigHandle::_get_ref(self);
  return PyBool_FromLong(val.is_input());
}

// return INPUT ID
PyObject*
input_id(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyAigHandle::_get_ref(self);
  try {
    return PyLong::ToPyObject(val.input_id());
  }
  catch ( std::invalid_argument err ) {
    std::ostringstream buf;
    buf << "invalid argument" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// True if AND
PyObject*
is_and(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyAigHandle::_get_ref(self);
  return PyBool_FromLong(val.is_and());
}

// return fanin handle
PyObject*
fanin(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "pos",
    nullptr
  };
  int pos;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "i",
                                    const_cast<char**>(kwlist),
                                    &pos) ) {
    return nullptr;
  }
  auto& val = PyAigHandle::_get_ref(self);
  try {
    return PyAigHandle::ToPyObject(val.fanin(pos));
  }
  catch ( std::invalid_argument err ) {
    std::ostringstream buf;
    buf << "invalid argument" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// return fanin0 handle
PyObject*
fanin0(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyAigHandle::_get_ref(self);
  try {
    return PyAigHandle::ToPyObject(val.fanin0());
  }
  catch ( std::invalid_argument err ) {
    std::ostringstream buf;
    buf << "invalid argument" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// return fanin1 handle
PyObject*
fanin1(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyAigHandle::_get_ref(self);
  try {
    return PyAigHandle::ToPyObject(val.fanin1());
  }
  catch ( std::invalid_argument err ) {
    std::ostringstream buf;
    buf << "invalid argument" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// return EXTENDED fanin list
PyObject*
ex_fanin_list(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyAigHandle::_get_ref(self);
  try {
    return PyList<AigHandle, PyAigHandle>::ToPyObject(val.ex_fanin_list());
  }
  catch ( std::invalid_argument err ) {
    std::ostringstream buf;
    buf << "invalid argument" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// generate DOT file
PyObject*
gen_dot(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "filename",
    "option",
    nullptr
  };
  const char* filename_tmp = nullptr;
  PyObject* option_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "s|$O",
                                    const_cast<char**>(kwlist),
                                    &filename_tmp,
                                    &option_obj) ) {
    return nullptr;
  }
  std::string filename;
  if ( filename_tmp != nullptr ) {
    filename = std::string(filename_tmp);
  }
  JsonValue option;
  if ( option_obj != nullptr ) {
    if ( !PyJsonValue::FromPyObject(option_obj, option) ) {
      PyErr_SetString(PyExc_ValueError, "could not convert to JsonValue");
      return nullptr;
    }
  }
  auto& val = PyAigHandle::_get_ref(self);
  std::ofstream ofs(filename);
  if ( !ofs ) {
    std::ostringstream buf;
    buf << "Could not create file '" << filename << "'";
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
  val.gen_dot(ofs, option);
  Py_RETURN_NONE;
}

// generate DOT file for AigHandles
PyObject*
gen_dot2(
  PyObject* Py_UNUSED(self),
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "filename",
    "aig_list",
    "option",
    nullptr
  };
  const char* filename_tmp = nullptr;
  PyObject* list_obj = nullptr;
  PyObject* option_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "sO|$O",
                                    const_cast<char**>(kwlist),
                                    &filename_tmp,
                                    &list_obj,
                                    &option_obj) ) {
    return nullptr;
  }
  std::string filename;
  if ( filename_tmp != nullptr ) {
    filename = std::string(filename_tmp);
  }
  JsonValue option;
  if ( option_obj != nullptr ) {
    if ( !PyJsonValue::FromPyObject(option_obj, option) ) {
      PyErr_SetString(PyExc_ValueError, "could not convert to JsonValue");
      return nullptr;
    }
  }
  std::ofstream ofs(filename);
  if ( !ofs ) {
    std::ostringstream buf;
    buf << "Could not create file '" << filename << "'";
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
  std::vector<AigHandle> aig_list;
  if ( !PyList<AigHandle, PyAigHandle>::FromPyObject(list_obj, aig_list) ) {
    PyErr_SetString(PyExc_TypeError, "argument 2 should be a sequence of 'AigHandle'");
    return nullptr;
  }
  AigHandle::gen_dot(ofs, aig_list, option);
  Py_RETURN_NONE;
}

// メソッド定義
PyMethodDef methods[] = {
  {"inv",
   inv,
   METH_NOARGS,
   PyDoc_STR("return INV flag")},
  {"is_zero",
   is_zero,
   METH_NOARGS,
   PyDoc_STR("True if ZERO")},
  {"is_one",
   is_one,
   METH_NOARGS,
   PyDoc_STR("True if ONE")},
  {"is_const",
   is_const,
   METH_NOARGS,
   PyDoc_STR("True if constant(ZERO/ONE)")},
  {"is_input",
   is_input,
   METH_NOARGS,
   PyDoc_STR("True if INPUT")},
  {"input_id",
   input_id,
   METH_NOARGS,
   PyDoc_STR("return INPUT ID")},
  {"is_and",
   is_and,
   METH_NOARGS,
   PyDoc_STR("True if AND")},
  {"fanin",
   reinterpret_cast<PyCFunction>(fanin),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return fanin handle")},
  {"fanin0",
   fanin0,
   METH_NOARGS,
   PyDoc_STR("return fanin0 handle")},
  {"fanin1",
   fanin1,
   METH_NOARGS,
   PyDoc_STR("return fanin1 handle")},
  {"ex_fanin_list",
   ex_fanin_list,
   METH_NOARGS,
   PyDoc_STR("return EXTENDED fanin list")},
  {"gen_dot",
   reinterpret_cast<PyCFunction>(gen_dot),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("generate DOT file")},
  {"gen_dot2",
   reinterpret_cast<PyCFunction>(gen_dot2),
   METH_VARARGS | METH_KEYWORDS | METH_STATIC,
   PyDoc_STR("generate DOT file for AigHandles")},
  // end-marker
  {nullptr, nullptr, 0, nullptr}
};

PyObject*
get_index(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyAigHandle::_get_ref(self);
  return PyLong::ToPyObject(val.index());
}

// getter/setter定義
PyGetSetDef getsets[] = {
  {"index", get_index, nullptr, PyDoc_STR(""), nullptr},
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
  PyErr_SetString(PyExc_TypeError, "instantiation of 'AigHandle' is disabled");
  return nullptr;
}

END_NONAMESPACE


// @brief AigHandle オブジェクトを使用可能にする．
bool
PyAigHandle::init(
  PyObject* m
)
{
  AigHandle_Type.tp_name = "AigHandle";
  AigHandle_Type.tp_basicsize = sizeof(AigHandle_Object);
  AigHandle_Type.tp_itemsize = 0;
  AigHandle_Type.tp_dealloc = dealloc_func;
  AigHandle_Type.tp_as_number = &number;
  AigHandle_Type.tp_hash = hash_func;
  AigHandle_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  AigHandle_Type.tp_doc = PyDoc_STR("Python extended object for AigHandle");
  AigHandle_Type.tp_richcompare = richcompare_func;
  AigHandle_Type.tp_methods = methods;
  AigHandle_Type.tp_getset = getsets;
  AigHandle_Type.tp_new = new_func;
  if ( !PyModule::reg_type(m, "AigHandle", &AigHandle_Type) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// AigHandle を PyObject に変換する．
PyObject*
PyAigHandle::Conv::operator()(
  const AigHandle& val
)
{
  auto type = PyAigHandle::_typeobject();
  auto obj = type->tp_alloc(type, 0);
  auto my_obj = reinterpret_cast<AigHandle_Object*>(obj);
  new (&my_obj->mVal) AigHandle(val);
  return obj;
}

// PyObject を AigHandle に変換する．
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
  return Py_IS_TYPE(obj, &AigHandle_Type);
}

// @brief PyObject から AigHandle を取り出す．
AigHandle&
PyAigHandle::_get_ref(
  PyObject* obj
)
{
  auto my_obj = reinterpret_cast<AigHandle_Object*>(obj);
  return my_obj->mVal;
}

// @brief AigHandle を表すオブジェクトの型定義を返す．
PyTypeObject*
PyAigHandle::_typeobject()
{
  return &AigHandle_Type;
}

END_NAMESPACE_YM
