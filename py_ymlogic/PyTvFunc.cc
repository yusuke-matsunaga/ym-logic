
/// @file PyTvFunc.cc
/// @brief Python TvFunc の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022, 2023 Yusuke Matsunaga
/// All rights reserved.

#include "PyTvFunc.h"
#include "ym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
struct TvFuncObject
{
  PyObject_HEAD
  TvFunc mVal;
};

// Python 用のタイプ定義
PyTypeObject TvFuncType = {
  PyVarObject_HEAD_INIT(nullptr, 0)
};

// 生成関数
PyObject*
TvFunc_new(
  PyTypeObject* type,
  PyObject* Py_UNUSED(args),
  PyObject* Py_UNUSED(kwds)
)
{
  auto self = type->tp_alloc(type, 0);
  // auto tvfunc_obj = reinterpret_cast<TvFuncObject*>(self);
  // 必要なら tvfunc_obj->mVal の初期化を行う．
  return self;
}

// 終了関数
void
TvFunc_dealloc(
  PyObject* self
)
{
  // auto tvfunc_obj = reinterpret_cast<TvFuncObject*>(self);
  // 必要なら tvfunc_obj->mVal の終了処理を行う．
  Py_TYPE(self)->tp_free(self);
}

// 初期化関数(__init__()相当)
int
TvFunc_init(
  PyObject* self,
  PyObject* args,
  PyObject* Py_UNUSED(kwds)
)
{
  return 0;
}

// repr() 関数
PyObject*
TvFunc_repr(
  PyObject* self
)
{
  auto val = PyTvFunc::_get(self);
  // val から 文字列を作る．
  const char* tmp_str = nullptr;
  return Py_BuildValue("s", tmp_str);
}

// メソッド定義
PyMethodDef TvFunc_methods[] = {
  {nullptr, nullptr, 0, nullptr}
};

// 比較関数
PyObject*
TvFunc_richcmpfunc(
  PyObject* self,
  PyObject* other,
  int op
)
{
  if ( PyTvFunc::_check(self) &&
       PyTvFunc::_check(other) ) {
    auto val1 = PyTvFunc::_get(self);
    auto val2 = PyTvFunc::_get(other);
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
TvFunc_invert(
  PyObject* self
)
{
  if ( PyTvFunc::_check(self) ) {
    auto val = PyTvFunc::_get(self);
    return PyTvFunc::ToPyObject(~val);
  }
  Py_RETURN_NOTIMPLEMENTED;
}

// 乗算(二項演算の例)
PyObject*
TvFunc_mul(
  PyObject* self,
  PyObject* other
)
{
  if ( PyTvFunc::_check(self) && PyTvFunc::_check(other) ) {
    auto val1 = PyTvFunc::_get(self);
    auto val2 = PyTvFunc::_get(other);
    return PyTvFunc::ToPyObject(val1 * val2);
  }
  Py_RETURN_NOTIMPLEMENTED;
}

// 数値演算メソッド定義
PyNumberMethods TvFunc_number = {
  .nb_invert = TvFunc_invert,
  .nb_mul = TvFunc_mul,
  .nb_inplace_mul = TvFunc_mul
};

// マップオブジェクトのサイズ
Py_ssize_t
TvFunc_Size(
  PyObject* self
)
{
  auto val = PyTvFunc::_get(self);
  return val.size();
}

// マップオブジェクトの要素取得関数
PyObject*
TvFunc_GetItem(
  PyObject* self,
  PyObject* key
)
{
  return nullptr;
}

// マップオブジェクトの要素設定関数
int
TvFunc_SetItem(
  PyObject* self,
  PyObject* key,
  PyObject* v
)
{
  return -1;
}

// マップオブジェクト構造体
PyMappingMethods TvFunc_mapping = {
  .mp_length = TvFunc_Size,
  .mp_subscript = TvFunc_GetItem,
  .mp_ass_subscript = TvFunc_SetItem
};

// ハッシュ関数
Py_hash_t
TvFunc_hash(
  PyObject* self
)
{
  auto val = PyTvFunc::_get(self);
  return val.hash();
}

END_NONAMESPACE


// @brief 'TvFunc' オブジェクトを使用可能にする．
bool
PyTvFunc::init(
  PyObject* m
)
{
  TvFuncType.tp_name = "TvFunc";
  TvFuncType.tp_basicsize = sizeof(TvFuncObject);
  TvFuncType.tp_itemsize = 0;
  TvFuncType.tp_dealloc = TvFunc_dealloc;
  TvFuncType.tp_flags = Py_TPFLAGS_DEFAULT;
  TvFuncType.tp_doc = PyDoc_STR("TvFunc object");
  TvFuncType.tp_richcompare = TvFunc_richcmpfunc;
  TvFuncType.tp_methods = TvFunc_methods;
  TvFuncType.tp_init = TvFunc_init;
  TvFuncType.tp_new = TvFunc_new;
  TvFuncType.tp_repr = TvFunc_repr;
  TvFuncType.tp_as_number = &TvFunc_number;
  TvFuncType.tp_as_mapping = &TvFunc_mapping;
  TvFuncType.tp_hash = TvFunc_hash;

  // 型オブジェクトの登録
  if ( !PyModule::reg_type(m, "TvFunc", &TvFuncType) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// @brief PyObject から TvFunc を取り出す．
bool
PyTvFunc::FromPyObject(
  PyObject* obj,
  TvFunc& val
)
{
  if ( !_check(obj) ) {
    PyErr_SetString(PyExc_TypeError, "object is not a TvFunc type");
    return false;
  }
  val = _get(obj);
  return true;
}

// @brief TvFunc を PyObject に変換する．
PyObject*
PyTvFunc::ToPyObject(
  TvFunc val
)
{
  auto obj = TvFuncType.tp_alloc(&TvFuncType, 0);
  auto tvfunc_obj = reinterpret_cast<TvFuncObject*>(obj);
  tvfunc_obj->mVal = val;
  return obj;
}

// @brief PyObject が TvFunc タイプか調べる．
bool
PyTvFunc::_check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, _typeobject());
}

// @brief TvFunc を表す PyObject から TvFunc を取り出す．
TvFunc
PyTvFunc::_get(
  PyObject* obj
)
{
  auto tvfunc_obj = reinterpret_cast<TvFuncObject*>(obj);
  return tvfunc_obj->mVal;
}

// @brief TvFunc を表すオブジェクトの型定義を返す．
PyTypeObject*
PyTvFunc::_typeobject()
{
  return &TvFuncType;
}

END_NAMESPACE_YM
