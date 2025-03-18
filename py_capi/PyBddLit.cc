
/// @file PyBddLit.cc
/// @brief Python BddLit の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyBddLit.h"
#include "pym/PyBddVar.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
struct BddLitObject
{
  PyObject_HEAD
  BddLit mVal;
};

// Python 用のタイプ定義
PyTypeObject BddLitType = {
  PyVarObject_HEAD_INIT(nullptr, 0)
};

// 生成関数
PyObject*
BddLit_new(
  PyTypeObject* type,
  PyObject* Py_UNUSED(args),
  PyObject* Py_UNUSED(kwds)
)
{
  PyErr_SetString(PyExc_TypeError, "instantiation of 'BddVar' is disabled");
  return nullptr;
}

// 終了関数
void
BddLit_dealloc(
  PyObject* self
)
{
  auto bddlit_obj = reinterpret_cast<BddLitObject*>(self);
  bddlit_obj->mVal.~BddList();
  Py_TYPE(self)->tp_free(self);
}

// repr() 関数
PyObject*
BddLit_repr(
  PyObject* self
)
{
  auto val = PyBddLit::_get(self);
  // val から 文字列を作る．
  const char* tmp_str = nullptr;
  return Py_BuildValue("s", tmp_str);
}

PyObject*
BddLit_invalid(
  PyObject* Py_UNUSED(self),
  PyObject* Py_UNUSED(args)
)
{
  auto lit = BddLit::invalid();
  return PyBddLit::ToPyObject(lit);
}

PyObject*
BddLit_positive_literal(
  PyObject* Py_UNUSED(self),
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kw_list[] = {
    "var",
    nullptr
  };
  PyObject* var_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O!",
				    const_cast<char**>(kw_list),
				    PyBddVar::_typeobject(), &var_obj) ) {
    return nullptr;
  }
  auto var = PyBddVar::Get(var_obj);
  auto ans = BddLit::positive_literal(var);
  return PyBddLit::ToPyObject(ans);
}

PyObject*
BddLit_negative_literal(
  PyObject* Py_UNUSED(self),
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kw_list[] = {
    "var",
    nullptr
  };
  PyObject* var_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O!",
				    const_cast<char**>(kw_list),
				    PyBddVar::_typeobject(), &var_obj) ) {
    return nullptr;
  }
  auto var = PyBddVar::Get(var_obj);
  auto ans = BddLit::negative_literal(var);
  return PyBddLit::ToPyObject(ans);
}

PyObject*
BddLit_make_positive(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto lit = PyBddLit::_get(self);
  auto ans = lit.make_positive();
  return PyBddLit::ToPyObject(ans);
}

PyObject*
BddLit_make_negative(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto lit = PyBddLit::_get(self);
  auto ans = lit.make_negative();
  return PyBddLit::ToPyObject(ans);
}

PyObject*
BddLit_is_positive(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto lit = PyBddLit::_get(self);
  auto ans = lit.is_positive();
  return PyBool_FromLong(ans);
}

PyObject*
BddLit_is_negative(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto lit = PyBddLit::_get(self);
  auto ans = lit.is_negative();
  return PyBool_FromLong(ans);
}

// メソッド定義
PyMethodDef BddLit_methods[] = {
  {"invalid", BddLit_invalid,
   METH_NOARGS | METH_STATIC,
   PyDoc_STR("make an invalid object")},
  {"positive_literal", reinterpret_cast<PyCFunction>(BddLit_positive_literal),
   METH_VARARGS | METH_KEYWORDS | METH_STATIC,
   PyDoc_STR("make a positive literal")},
  {"negative_literal", reinterpret_cast<PyCFunction>(BddLit_positive_literal),
   METH_VARARGS | METH_KEYWORDS | METH_STATIC,
   PyDoc_STR("make a negative literal")},
  {"make_positive", BddLit_make_positive,
   METH_NOARGS,
   PyDoc_STR("convert to a positive literal")},
  {"make_negative", BddLit_make_positive,
   METH_NOARGS,
   PyDoc_STR("convert to a negative literal")},
  {"is_positive", BddLit_is_positive,
   METH_NOARGS,
   PyDoc_STR("return True if positive")},
  {"is_negative", BddLit_is_negative,
   METH_NOARGS,
   PyDoc_STR("return True if negative")},
  {nullptr, nullptr, 0, nullptr}
};

PyObject*
BddLit_var(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto lit = PyBddLit::_get(self);
  auto ans = lit.var();
  return PyBddVar::ToPyObject(ans);
}

// get/set 関数定義
PyGetSetDef BddLit_getset[] = {
  {"var", BddLit_var, nullptr, PyDoc_STR("var"), nullptr},
  {nullptr, nullptr, nullptr, nullptr, nullptr},
};

// 比較関数
PyObject*
BddLit_richcmpfunc(
  PyObject* self,
  PyObject* other,
  int op
)
{
  if ( PyBddLit::Check(self) &&
       PyBddLit::Check(other) ) {
    auto val1 = PyBddLit::_get(self);
    auto val2 = PyBddLit::_get(other);
    if ( op == Py_EQ ) {
      return PyBool_FromLong(val1 == val2);
    }
    if ( op == Py_NE ) {
      return PyBool_FromLong(val1 != val2);
    }
  }
  Py_RETURN_NOTIMPLEMENTED;
}

// 否定演算
PyObject*
BddLit_invert(
  PyObject* self
)
{
  if ( PyBddLit::Check(self) ) {
    auto val = PyBddLit::_get(self);
    return PyBddLit::ToPyObject(~val);
  }
  Py_RETURN_NOTIMPLEMENTED;
}

// 数値演算メソッド定義
PyNumberMethods BddLit_number = {
  .nb_invert = BddLit_invert,
};

// ハッシュ関数
Py_hash_t
BddLit_hash(
  PyObject* self
)
{
  auto val = PyBddLit::_get(self);
  return val.hash();
}

END_NONAMESPACE


// @brief 'BddLit' オブジェクトを使用可能にする．
bool
PyBddLit::init(
  PyObject* m
)
{
  BddLitType.tp_name = "BddLit";
  BddLitType.tp_basicsize = sizeof(BddLitObject);
  BddLitType.tp_itemsize = 0;
  BddLitType.tp_dealloc = BddLit_dealloc;
  BddLitType.tp_flags = Py_TPFLAGS_DEFAULT;
  BddLitType.tp_doc = PyDoc_STR("BddLit object");
  BddLitType.tp_richcompare = BddLit_richcmpfunc;
  BddLitType.tp_methods = BddLit_methods;
  BddLitType.tp_getset = BddLit_getset;
  BddLitType.tp_new = BddLit_new;
  BddLitType.tp_repr = BddLit_repr;
  BddLitType.tp_as_number = &BddLit_number;
  BddLitType.tp_hash = BddLit_hash;

  // 型オブジェクトの登録
  if ( !PyModule::reg_type(m, "BddLit", &BddLitType) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// @brief BddLit を PyObject に変換する．
PyObject*
PyBddLitConv::operator()(
  const BddLit& val
)
{
  auto obj = BddLitType.tp_alloc(&BddLitType, 0);
  auto bddlit_obj = reinterpret_cast<BddLitObject*>(obj);
  new (&bddlit_obj->mVal) BddLit{val};
  return obj;
}

// @brief PyObject が BddLit タイプか調べる．
bool
PyBddLit::_check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, _typeobject());
}

// @brief BddLit を表す PyObject から BddLit を取り出す．
BddLit
PyBddLit::_get(
  PyObject* obj
)
{
  auto bddlit_obj = reinterpret_cast<BddLitObject*>(obj);
  return bddlit_obj->mVal;
}

// @brief BddLit を表すオブジェクトの型定義を返す．
PyTypeObject*
PyBddLit::_typeobject()
{
  return &BddLitType;
}

END_NAMESPACE_YM
