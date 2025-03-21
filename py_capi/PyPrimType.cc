
/// @file PyPrimType.cc
/// @brief Python PrimType の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyPrimType.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// 定数を表すオブジェクトの配列
PyObject* PrimType_ConstList[10];

// Python 用のオブジェクト定義
struct PrimTypeObject
{
  PyObject_HEAD
  PrimType mVal;
};

// Python 用のタイプ定義
PyTypeObject PrimTypeType = {
  PyVarObject_HEAD_INIT(nullptr, 0)
};

// 生成関数
PyObject*
PrimType_new(
  PyTypeObject* type,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kw_list[] = {
    "",
    nullptr
  };
  const char* val_str = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "s",
				    const_cast<char**>(kw_list),
				    &val_str) ) {
    return nullptr;
  }

  auto val = PrimType::None;
  if ( strcasecmp(val_str, "c0") == 0 ) {
    val = PrimType::C0;
  }
  else if ( strcasecmp(val_str, "c1") == 0 ) {
    val = PrimType::C1;
  }
  else if ( strcasecmp(val_str, "buff") == 0 ) {
    val = PrimType::Buff;
  }
  else if ( strcasecmp(val_str, "not") == 0 ) {
    val = PrimType::Not;
  }
  else if ( strcasecmp(val_str, "and") == 0 ) {
    val = PrimType::And;
  }
  else if ( strcasecmp(val_str, "nand") == 0 ) {
    val = PrimType::Nand;
  }
  else if ( strcasecmp(val_str, "or") == 0 ) {
    val = PrimType::Or;
  }
  else if ( strcasecmp(val_str, "nor") == 0 ) {
    val = PrimType::Nor;
  }
  else if ( strcasecmp(val_str, "xor") == 0 ) {
    val = PrimType::Xor;
  }
  else if ( strcasecmp(val_str, "xnor") == 0 ) {
    val = PrimType::Xnor;
  }
  if ( val == PrimType::None ) {
    ostringstream buf;
    buf << val_str << ": unknown type for PrimType";
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }

  return PyPrimType::ToPyObject(val);
}

// 終了関数
void
PrimType_dealloc(
  PyObject* self
)
{
  // auto primtype_obj = reinterpret_cast<PrimTypeObject*>(self);
  // 必要なら primtype_obj->mVal の終了処理を行う．
  // delete primtype_obj->mVal
  Py_TYPE(self)->tp_free(self);
}

// repr() 関数
PyObject*
PrimType_repr(
  PyObject* self
)
{
  auto val = PyPrimType::_get_ref(self);
  // val から 文字列を作る．
  const char* tmp_str = nullptr;
  switch ( val ) {
  case PrimType::None: tmp_str = "---"; break;
  case PrimType::C0:   tmp_str = "C0"; break;
  case PrimType::C1:   tmp_str = "C1"; break;
  case PrimType::Buff: tmp_str = "Buff"; break;
  case PrimType::Not:  tmp_str = "Not"; break;
  case PrimType::And:  tmp_str = "And"; break;
  case PrimType::Nand: tmp_str = "Nand"; break;
  case PrimType::Or:   tmp_str = "Or"; break;
  case PrimType::Nor:  tmp_str = "Nor"; break;
  case PrimType::Xor:  tmp_str = "Xor"; break;
  case PrimType::Xnor: tmp_str = "Xnor"; break;
  }
  return Py_BuildValue("s", tmp_str);
}

// メソッド定義
PyMethodDef PrimType_methods[] = {
  {nullptr, nullptr, 0, nullptr}
};

// 比較関数
PyObject*
PrimType_richcmpfunc(
  PyObject* self,
  PyObject* other,
  int op
)
{
  if ( PyPrimType::Check(self) &&
       PyPrimType::Check(other) ) {
    auto val1 = PyPrimType::_get_ref(self);
    auto val2 = PyPrimType::_get_ref(other);
    if ( op == Py_EQ ) {
      return PyBool_FromLong( val1 == val2 );
    }
    if ( op == Py_NE ) {
      return PyBool_FromLong( val1 != val2 );
    }
  }
  Py_RETURN_NOTIMPLEMENTED;
}

// ハッシュ関数
Py_hash_t
PrimType_hash(
  PyObject* self
)
{
  auto val = PyPrimType::_get_ref(self);
  return static_cast<SizeType>(val);
}

// 定数オブジェクトの登録
bool
reg_const(
  const char* name,
  PrimType val
)
{
  auto obj = PrimTypeType.tp_alloc(&PrimTypeType, 0);
  auto primtype_obj = reinterpret_cast<PrimTypeObject*>(obj);
  primtype_obj->mVal = val;
  if ( PyDict_SetItemString(PrimTypeType.tp_dict, name, obj) < 0 ) {
    return false;
  }
  PrimType_ConstList[static_cast<SizeType>(val) - 1] = obj;
  return true;
}

END_NONAMESPACE


// @brief 'PrimType' オブジェクトを使用可能にする．
bool
PyPrimType::init(
  PyObject* m
)
{
  PrimTypeType.tp_name = "PrimType";
  PrimTypeType.tp_basicsize = sizeof(PrimTypeObject);
  PrimTypeType.tp_itemsize = 0;
  PrimTypeType.tp_dealloc = PrimType_dealloc;
  PrimTypeType.tp_flags = Py_TPFLAGS_DEFAULT;
  PrimTypeType.tp_doc = PyDoc_STR("PrimType object");
  PrimTypeType.tp_richcompare = PrimType_richcmpfunc;
  PrimTypeType.tp_methods = PrimType_methods;
  PrimTypeType.tp_new = PrimType_new;
  PrimTypeType.tp_repr = PrimType_repr;
  PrimTypeType.tp_hash = PrimType_hash;

  // 型オブジェクトの登録
  if ( !PyModule::reg_type(m, "PrimType", &PrimTypeType) ) {
    goto error;
  }

  // 定数オブジェクトの生成・登録
  for ( SizeType i = 0; i < 10; ++ i ) {
    PrimType_ConstList[i] = nullptr;
  }
  if ( !reg_const("C0", PrimType::C0) ) {
    goto error;
  }
  if ( !reg_const("C1", PrimType::C1) ) {
    goto error;
  }
  if ( !reg_const("Buff", PrimType::Buff) ) {
    goto error;
  }
  if ( !reg_const("Not", PrimType::Not) ) {
    goto error;
  }
  if ( !reg_const("And", PrimType::And) ) {
    goto error;
  }
  if ( !reg_const("Nand", PrimType::Nand) ) {
    goto error;
  }
  if ( !reg_const("Or", PrimType::Or) ) {
    goto error;
  }
  if ( !reg_const("Nor", PrimType::Nor) ) {
    goto error;
  }
  if ( !reg_const("Xor", PrimType::Xor) ) {
    goto error;
  }
  if ( !reg_const("Xnor", PrimType::Xnor) ) {
    goto error;
  }
  return true;

 error:
  for ( SizeType i = 0; i < 10; ++ i ) {
    Py_XDECREF(PrimType_ConstList[i]);
  }

  return false;
}

// @brief PrimType を PyObject に変換する．
PyObject*
PyPrimType::Conv::operator()(
  const PrimType& val
)
{
  if ( val == PrimType::None ) {
    Py_RETURN_NONE;
  }
  auto obj = PrimType_ConstList[static_cast<SizeType>(val) - 1];
  Py_INCREF(obj);
  return obj;
}

// @brief PyObject* から PrimType を取り出す．
bool
PyPrimType::Deconv::operator()(
  PyObject* obj,
  PrimType& val
)
{
  if ( PyPrimType::Check(obj) ) {
    val = PyPrimType::_get_ref(obj);
    return true;
  }
  return false;
}

// @brief PyObject が PrimType タイプか調べる．
bool
PyPrimType::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, _typeobject());
}

// @brief PrimType を表す PyObject から PrimType を取り出す．
PrimType&
PyPrimType::_get_ref(
  PyObject* obj
)
{
  auto primtype_obj = reinterpret_cast<PrimTypeObject*>(obj);
  return primtype_obj->mVal;
}

// @brief PrimType を表すオブジェクトの型定義を返す．
PyTypeObject*
PyPrimType::_typeobject()
{
  return &PrimTypeType;
}

END_NAMESPACE_YM
