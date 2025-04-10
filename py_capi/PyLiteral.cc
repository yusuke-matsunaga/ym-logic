
/// @file PyLiteral.cc
/// @brief PyLiteral の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyLiteral.h"
#include "pym/PyLong.h"
#include "pym/PyString.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
// この構造体は同じサイズのヒープから作られるので
// mVal のコンストラクタは起動されないことに注意．
// そのためあとでコンストラクタを明示的に起動する必要がある．
// またメモリを開放するときにも明示的にデストラクタを起動する必要がある．
struct Literal_Object
{
  PyObject_HEAD
  Literal mVal;
};

// Python 用のタイプ定義
PyTypeObject Literal_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PyLiteral::init() 中で初期化する．
};

// 終了関数
void
dealloc_func(
  PyObject* self
)
{
  Py_TYPE(self)->tp_free(self);
}

// repr 関数
PyObject*
repr_func(
  PyObject* self
)
{
  auto& val = PyLiteral::_get_ref(self);
  // val から文字列を作る．
  std::ostringstream buf;
  buf << val;
  auto str_val = buf.str();
  return PyString::ToPyObject(str_val);
}

PyObject*
nb_invert(
  PyObject* self
)
{
  auto& val = PyLiteral::_get_ref(self);
  return PyLiteral::ToPyObject(~val);
}

// Numberオブジェクト構造体
PyNumberMethods number = {
  .nb_invert = nb_invert
};

// hash 関数
Py_hash_t
hash_func(
  PyObject* self
)
{
  auto& val = PyLiteral::_get_ref(self);
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
  auto& val = PyLiteral::_get_ref(self);
  if ( PyLiteral::Check(other) ) {
    auto& val2 = PyLiteral::_get_ref(other);
    Py_RETURN_RICHCOMPARE(val, val2, op);
  }
  Py_RETURN_NOTIMPLEMENTED;
}

// 
PyObject*
set(
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
  int id = -1;
  int inv_tmp = -1;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "i|$p",
                                    const_cast<char**>(kwlist),
                                    &id,
                                    &inv_tmp) ) {
    return nullptr;
  }
  bool inv = false;
  if ( inv_tmp != -1 ) {
    inv = static_cast<bool>(inv_tmp);
  }
  auto& val = PyLiteral::_get_ref(self);
  val.set(id, inv);
  Py_RETURN_NONE;
}

// 
PyObject*
is_valid(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyLiteral::_get_ref(self);
  return PyBool_FromLong(val.is_valid());
}

// 
PyObject*
is_positive(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyLiteral::_get_ref(self);
  return PyBool_FromLong(val.is_positive());
}

// 
PyObject*
is_negative(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyLiteral::_get_ref(self);
  return PyBool_FromLong(val.is_negative());
}

// 
PyObject*
make_positive(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyLiteral::_get_ref(self);
  return PyLiteral::ToPyObject(val.make_positive());
}

// 
PyObject*
make_negative(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyLiteral::_get_ref(self);
  return PyLiteral::ToPyObject(val.make_negative());
}

// メソッド定義
PyMethodDef methods[] = {
  {"set",
   reinterpret_cast<PyCFunction>(set),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("")},
  {"is_valid",
   is_valid,
   METH_NOARGS,
   PyDoc_STR("")},
  {"is_positive",
   is_positive,
   METH_NOARGS,
   PyDoc_STR("")},
  {"is_negative",
   is_negative,
   METH_NOARGS,
   PyDoc_STR("")},
  {"make_positive",
   make_positive,
   METH_NOARGS,
   PyDoc_STR("")},
  {"make_negative",
   make_negative,
   METH_NOARGS,
   PyDoc_STR("")},
  // end-marker
  {nullptr, nullptr, 0, nullptr}
};

PyObject*
varid_getter(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyLiteral::_get_ref(self);
  auto ans = val.varid();
  return PyLong::ToPyObject(ans);
}

PyObject*
index_getter(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyLiteral::_get_ref(self);
  auto ans = val.index();
  return PyLong::ToPyObject(ans);
}

// getter/setter定義
PyGetSetDef getsets[] = {
  {"varid", varid_getter, nullptr, PyDoc_STR(""), nullptr},
  {"index", index_getter, nullptr, PyDoc_STR(""), nullptr},
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
    "",
    "inv",
    nullptr
  };
  int id = -1;
  int inv_tmp = -1;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "|i$p",
                                    const_cast<char**>(kwlist),
                                    &id,
                                    &inv_tmp) ) {
    return nullptr;
  }
  bool inv = false;
  if ( inv_tmp != -1 ) {
    inv = static_cast<bool>(inv_tmp);
  }
  auto self = type->tp_alloc(type, 0);
  auto my_obj = reinterpret_cast<Literal_Object*>(self);
  if ( id == -1 ) {
    my_obj->mVal = Literal::x();
  }
  else {
    my_obj->mVal = Literal(id, inv);
  }
  return self;
}

END_NONAMESPACE


// @brief Literal オブジェクトを使用可能にする．
bool
PyLiteral::init(
  PyObject* m
)
{
  Literal_Type.tp_name = "Literal";
  Literal_Type.tp_basicsize = sizeof(Literal_Object);
  Literal_Type.tp_itemsize = 0;
  Literal_Type.tp_dealloc = dealloc_func;
  Literal_Type.tp_repr = repr_func;
  Literal_Type.tp_as_number = &number;
  Literal_Type.tp_hash = hash_func;
  Literal_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  Literal_Type.tp_doc = PyDoc_STR("Python extended object for Literal");
  Literal_Type.tp_richcompare = richcompare_func;
  Literal_Type.tp_methods = methods;
  Literal_Type.tp_getset = getsets;
  Literal_Type.tp_new = new_func;
  if ( !PyModule::reg_type(m, "Literal", &Literal_Type) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// Literal を PyObject に変換する．
PyObject*
PyLiteral::Conv::operator()(
  const Literal& val
)
{
  auto type = PyLiteral::_typeobject();
  auto obj = type->tp_alloc(type, 0);
  auto my_obj = reinterpret_cast<Literal_Object*>(obj);
  new (&my_obj->mVal) Literal(val);
  return obj;
}

// PyObject を Literal に変換する．
bool
PyLiteral::Deconv::operator()(
  PyObject* obj,
  Literal& val
)
{
  if ( PyLong::Check(obj) ) {
    auto var = static_cast<SizeType>(PyLong::Get(obj));
    val = Literal(var);
    return true;
  }
  if ( PyLiteral::Check(obj) ) {
    val = PyLiteral::_get_ref(obj);
    return true;
  }
  return false;
}

// @brief PyObject が Literal タイプか調べる．
bool
PyLiteral::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, &Literal_Type);
}

// @brief PyObject から Literal を取り出す．
Literal&
PyLiteral::_get_ref(
  PyObject* obj
)
{
  auto my_obj = reinterpret_cast<Literal_Object*>(obj);
  return my_obj->mVal;
}

// @brief Literal を表すオブジェクトの型定義を返す．
PyTypeObject*
PyLiteral::_typeobject()
{
  return &Literal_Type;
}

END_NAMESPACE_YM
