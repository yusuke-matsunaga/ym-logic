
/// @file PyLiteral.cc
/// @brief Python Literal の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyLiteral.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
struct LiteralObject
{
  PyObject_HEAD
  Literal mVal;
};

// Python 用のタイプ定義
PyTypeObject LiteralType = {
  PyVarObject_HEAD_INIT(nullptr, 0)
};

// 生成関数
PyObject*
Literal_new(
  PyTypeObject* type,
  PyObject* Py_UNUSED(args),
  PyObject* Py_UNUSED(kwds)
)
{
  auto self = type->tp_alloc(type, 0);
  // auto literal_obj = reinterpret_cast<LiteralObject*>(self);
  // 必要なら literal_obj->mVal の初期化を行う．
  return self;
}

// 終了関数
void
Literal_dealloc(
  PyObject* self
)
{
  // auto literal_obj = reinterpret_cast<LiteralObject*>(self);
  // 必要なら literal_obj->mVal の終了処理を行う．
  Py_TYPE(self)->tp_free(self);
}

// 初期化関数(__init__()相当)
int
Literal_init(
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
  SizeType id = -1;
  int inv_int = false;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "|k$p",
				    const_cast<char**>(kwlist),
				    &id, &inv_int) ) {
    return -1;
  }
  auto lit_obj = reinterpret_cast<LiteralObject*>(self);
  if ( id == -1 ) {
    lit_obj->mVal = Literal::x();
  }
  else {
    bool inv = inv_int;
    lit_obj->mVal = Literal{id, inv};
  }
  return 0;
}

// repr() 関数
PyObject*
Literal_repr(
  PyObject* self
)
{
  auto val = PyLiteral::Get(self);
  // val から 文字列を作る．
  const char* tmp_str = nullptr;
  return Py_BuildValue("s", tmp_str);
}

PyObject*
Literal_set(
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
  SizeType id = -1;
  int inv = false;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "k|$p",
				    const_cast<char**>(kwlist),
				    &id, &inv) ) {
    return nullptr;
  }
  auto lit_obj = reinterpret_cast<LiteralObject*>(self);
  if ( id == -1 ) {
    lit_obj->mVal = Literal::x();
  }
  else {
    lit_obj->mVal.set(id, inv);
  }
  Py_RETURN_NONE;
}

PyObject*
Literal_is_valid(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto val = PyLiteral::Get(self);
  auto r = val.is_valid();
  return PyBool_FromLong(r);
}

PyObject*
Literal_is_positive(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto val = PyLiteral::Get(self);
  auto r = val.is_positive();
  return PyBool_FromLong(r);
}

PyObject*
Literal_is_negative(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto val = PyLiteral::Get(self);
  auto r = val.is_negative();
  return PyBool_FromLong(r);
}

PyObject*
Literal_make_positive(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto val = PyLiteral::Get(self);
  auto ans = val.make_positive();
  return PyLiteral::ToPyObject(ans);
}

PyObject*
Literal_make_negative(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto val = PyLiteral::Get(self);
  auto ans = val.make_negative();
  return PyLiteral::ToPyObject(ans);
}

// メソッド定義
PyMethodDef Literal_methods[] = {
  {"set", reinterpret_cast<PyCFunction>(Literal_set),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("set the contents")},
  {"is_valid", Literal_is_valid, METH_NOARGS,
   PyDoc_STR("True if valid")},
  {"is_positive", Literal_is_positive, METH_NOARGS,
   PyDoc_STR("True if positive")},
  {"is_negative", Literal_is_negative, METH_NOARGS,
   PyDoc_STR("True if negative")},
  {"make_positive", Literal_make_positive, METH_NOARGS,
   PyDoc_STR("return positive literal with the same variable")},
  {"make_negative", Literal_make_negative, METH_NOARGS,
   PyDoc_STR("return negative literal with the same variable")},
  {nullptr, nullptr, 0, nullptr}
};

PyObject*
Literal_varid(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto val = PyLiteral::Get(self);
  auto varid = val.varid();
  return PyLong_FromLong(varid);
}

PyObject*
Literal_index(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto val = PyLiteral::Get(self);
  auto index = val.index();
  return PyLong_FromLong(index);
}

PyGetSetDef Literal_getsetters[] = {
  {"varid", Literal_varid, nullptr, PyDoc_STR("Variable ID"), nullptr},
  {"index", Literal_index, nullptr, PyDoc_STR("index"), nullptr},
  {nullptr, nullptr, nullptr, nullptr}
};

// 比較関数
PyObject*
Literal_richcmpfunc(
  PyObject* obj1,
  PyObject* obj2,
  int op
)
{
  if ( PyLiteral::Check(obj1) &&
       PyLiteral::Check(obj2) ) {
    auto val1 = PyLiteral::Get(obj1);
    auto val2 = PyLiteral::Get(obj2);
    Py_RETURN_RICHCOMPARE(val1, val2, op);
  }
  Py_RETURN_NOTIMPLEMENTED;
}

// 否定演算(単項演算の例)
PyObject*
Literal_invert(
  PyObject* self
)
{
  if ( PyLiteral::Check(self) ) {
    auto val = PyLiteral::Get(self);
    return PyLiteral::ToPyObject(~val);
  }
  Py_RETURN_NOTIMPLEMENTED;
}

// 数値演算メソッド定義
PyNumberMethods Literal_number = {
  .nb_invert = Literal_invert
};

// ハッシュ関数
Py_hash_t
Literal_hash(
  PyObject* self
)
{
  auto val = PyLiteral::Get(self);
  return val.hash();
}

END_NONAMESPACE


// @brief 'Literal' オブジェクトを使用可能にする．
bool
PyLiteral::init(
  PyObject* m
)
{
  LiteralType.tp_name = "Literal";
  LiteralType.tp_basicsize = sizeof(LiteralObject);
  LiteralType.tp_itemsize = 0;
  LiteralType.tp_dealloc = Literal_dealloc;
  LiteralType.tp_flags = Py_TPFLAGS_DEFAULT;
  LiteralType.tp_doc = PyDoc_STR("Literal object");
  LiteralType.tp_richcompare = Literal_richcmpfunc;
  LiteralType.tp_methods = Literal_methods;
  LiteralType.tp_getset = Literal_getsetters;
  LiteralType.tp_init = Literal_init;
  LiteralType.tp_new = Literal_new;
  LiteralType.tp_repr = Literal_repr;
  LiteralType.tp_as_number = &Literal_number;
  LiteralType.tp_hash = Literal_hash;

  // 型オブジェクトの登録
  if ( !PyModule::reg_type(m, "Literal", &LiteralType) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// @brief Literal を PyObject に変換する．
PyObject*
PyLiteral::ToPyObject(
  Literal val
)
{
  auto obj = LiteralType.tp_alloc(&LiteralType, 0);
  auto literal_obj = reinterpret_cast<LiteralObject*>(obj);
  literal_obj->mVal = val;
  return obj;
}

// @brief PyObject が Literal タイプか調べる．
bool
PyLiteral::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, _typeobject());
}

// @brief Literal を表す PyObject から Literal を取り出す．
Literal
PyLiteral::Get(
  PyObject* obj
)
{
  auto literal_obj = reinterpret_cast<LiteralObject*>(obj);
  return literal_obj->mVal;
}

// @brief Literal を表すオブジェクトの型定義を返す．
PyTypeObject*
PyLiteral::_typeobject()
{
  return &LiteralType;
}

END_NAMESPACE_YM
