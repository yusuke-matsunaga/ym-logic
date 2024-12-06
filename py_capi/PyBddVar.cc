
/// @file PyBddVar.cc
/// @brief Python BddVar の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyBddVar.h"
#include "pym/PyBddLit.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
struct BddVarObject
{
  PyObject_HEAD
  BddVar mVal;
};

// Python 用のタイプ定義
PyTypeObject BddVarType = {
  PyVarObject_HEAD_INIT(nullptr, 0)
};

// 生成関数
PyObject*
BddVar_new(
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
BddVar_dealloc(
  PyObject* self
)
{
  auto bddvar_obj = reinterpret_cast<BddVarObject*>(self);
  bddvar_obj->mVal.~BddVar();
  Py_TYPE(self)->tp_free(self);
}

// repr() 関数
PyObject*
BddVar_repr(
  PyObject* self
)
{
  auto& val = PyBddVar::_get(self);
  // val から 文字列を作る．
  const char* tmp_str = nullptr;
  return Py_BuildValue("s", tmp_str);
}

PyObject*
BddVar_invalid(
  PyObject* Py_UNUSED(self),
  PyObject* Py_UNUSED(args)
)
{
  auto var = BddVar::invalid();
  return PyBddVar::ToPyObject(var);
}

PyObject*
BddVar_posilit(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyBddVar::_get(self);
  auto lit = val.posilit();
  return PyBddLit::ToPyObject(lit);
}

PyObject*
BddVar_negalit(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyBddVar::_get(self);
  auto lit = val.negalit();
  return PyBddLit::ToPyObject(lit);
}

// メソッド定義
PyMethodDef BddVar_methods[] = {
  {"invalid", BddVar_invalid,
   METH_NOARGS | METH_STATIC,
   PyDoc_STR("make an invalid object")},
  {"id", BddVar_id,
   METH_NOARGS,
   PyDoc_STR("return ID")},
  {"index", BddVar_index,
   METH_NOARGS,
   PyDoc_STR("return index")},
  {"posilit", BddVar_posilit,
   METH_NOARGS,
   PyDoc_STR("convert to positive literal")},
  {"negalit", BddVar_negalit,
   METH_NOARGS,
   PyDoc_STR("convert to negative literal")},
  {nullptr, nullptr, 0, nullptr}
};

// 比較関数
PyObject*
BddVar_richcmpfunc(
  PyObject* self,
  PyObject* other,
  int op
)
{
  if ( PyBddVar::Check(self) &&
       PyBddVar::Check(other) ) {
    auto& val1 = PyBddVar::_get(self);
    auto& val2 = PyBddVar::_get(other);
    if ( op == Py_EQ ) {
      return PyBool_FromLong(val1 == val2);
    }
    if ( op == Py_NE ) {
      return PyBool_FromLong(val1 == val2);
    }
  }
  Py_RETURN_NOTIMPLEMENTED;
}

// ハッシュ関数
Py_hash_t
BddVar_hash(
  PyObject* self
)
{
  auto& val = PyBddVar::_get(self);
  return val.hash();
}

END_NONAMESPACE


// @brief 'BddVar' オブジェクトを使用可能にする．
bool
PyBddVar::init(
  PyObject* m
)
{
  BddVarType.tp_name = "BddVar";
  BddVarType.tp_basicsize = sizeof(BddVarObject);
  BddVarType.tp_itemsize = 0;
  BddVarType.tp_dealloc = BddVar_dealloc;
  BddVarType.tp_flags = Py_TPFLAGS_DEFAULT;
  BddVarType.tp_doc = PyDoc_STR("BddVar object");
  BddVarType.tp_richcompare = BddVar_richcmpfunc;
  BddVarType.tp_methods = BddVar_methods;
  BddVarType.tp_new = BddVar_new;
  BddVarType.tp_repr = BddVar_repr;
  BddVarType.tp_hash = BddVar_hash;

  // 型オブジェクトの登録
  if ( !PyModule::reg_type(m, "BddVar", &BddVarType) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// @brief BddVar を PyObject に変換する．
PyObject*
PyBddVar::ToPyObject(
  const BddVar& val
)
{
  auto obj = BddVarType.tp_alloc(&BddVarType, 0);
  auto bddvar_obj = reinterpret_cast<BddVarObject*>(obj);
  new (&bddvar_obj->mVal) BddVar{val};
  return obj;
}

// @brief PyObject が BddVar タイプか調べる．
bool
PyBddVar::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, _typeobject());
}

// @brief BddVar を表す PyObject から BddVar を取り出す．
BddVar
PyBddVar::Get(
  PyObject* obj
)
{
  auto bddvar_obj = reinterpret_cast<BddVarObject*>(obj);
  return bddvar_obj->mVal;
}

// @brief BddVar を表す PyObject から BddVar を取り出す．
BddVar&
PyBddVar::_get(
  PyObject* obj
)
{
  auto bddvar_obj = reinterpret_cast<BddVarObject*>(obj);
  return bddvar_obj->mVal;
}

// @brief BddVar を表すオブジェクトの型定義を返す．
PyTypeObject*
PyBddVar::_typeobject()
{
  return &BddVarType;
}

END_NAMESPACE_YM
