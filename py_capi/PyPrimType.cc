
/// @file PyPrimType.cc
/// @brief PyPrimType の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyPrimType.h"
#include "pym/PyString.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
// この構造体は同じサイズのヒープから作られるので
// mVal のコンストラクタは起動されないことに注意．
// そのためあとでコンストラクタを明示的に起動する必要がある．
// またメモリを開放するときにも明示的にデストラクタを起動する必要がある．
struct PrimType_Object
{
  PyObject_HEAD
  PrimType mVal;
};

// Python 用のタイプ定義
PyTypeObject PrimType_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PyPrimType::init() 中で初期化する．
};

// 定数を表すオブジェクト
PyObject* Const_C0 = nullptr;
PyObject* Const_C1 = nullptr;
PyObject* Const_Buff = nullptr;
PyObject* Const_Not = nullptr;
PyObject* Const_And = nullptr;
PyObject* Const_Nand = nullptr;
PyObject* Const_Or = nullptr;
PyObject* Const_Nor = nullptr;
PyObject* Const_Xor = nullptr;
PyObject* Const_Xnor = nullptr;

// 定数の登録を行う関数
bool
reg_const_obj(
  const char* name,
  PrimType val,
  PyObject*& const_obj
)
{
  auto type = PyPrimType::_typeobject();
  auto obj = type->tp_alloc(type, 0);
  auto my_obj = reinterpret_cast<PrimType_Object*>(obj);
  my_obj->mVal = val;
  if ( PyDict_SetItemString(type->tp_dict, name, obj) < 0 ) {
    return false;
  }
  Py_INCREF(obj);
  const_obj = obj;
  return true;
}

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
  auto& val = PyPrimType::_get_ref(self);
  std::string str_val;
  switch ( val ) {
    case PrimType::C0: str_val = "C0"; break;
    case PrimType::C1: str_val = "C1"; break;
    case PrimType::Buff: str_val = "Buff"; break;
    case PrimType::Not: str_val = "Not"; break;
    case PrimType::And: str_val = "And"; break;
    case PrimType::Nand: str_val = "Nand"; break;
    case PrimType::Or: str_val = "Or"; break;
    case PrimType::Nor: str_val = "Nor"; break;
    case PrimType::Xor: str_val = "Xor"; break;
    case PrimType::Xnor: str_val = "Xnor"; break;
    case PrimType::None: str_val = "None"; break;
  }
  return PyString::ToPyObject(str_val);
}

// hash 関数
Py_hash_t
hash_func(
  PyObject* self
)
{
  auto& val = PyPrimType::_get_ref(self);
  auto hash_val = static_cast<Py_hash_t>(val);
  return hash_val;
}

// richcompare 関数
PyObject*
richcompare_func(
  PyObject* self,
  PyObject* other,
  int op
)
{
  auto& val = PyPrimType::_get_ref(self);
  if ( PyPrimType::Check(self) && PyPrimType::Check(other) ) {
    auto& val1 = PyPrimType::_get_ref(self);
    auto& val2 = PyPrimType::_get_ref(other);
    if ( op == Py_EQ ) {
      return PyBool_FromLong(val1 == val2);
    }
    if ( op == Py_NE ) {
      return PyBool_FromLong(val1 != val2);
    }
  }
  Py_RETURN_NOTIMPLEMENTED;
}

// メソッド定義
PyMethodDef methods[] = {
  // end-marker
  {nullptr, nullptr, 0, nullptr}
};

// getter/setter定義
PyGetSetDef getsets[] = {
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
    nullptr
  };
  PyObject* val_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O",
                                    const_cast<char**>(kwlist),
                                    &val_obj) ) {
    return nullptr;
  }
  PrimType val;
  if ( val_obj != nullptr ) {
    if ( !PyPrimType::FromPyObject(val_obj, val) ) {
      PyErr_SetString(PyExc_ValueError, "could not convert to PrimType");
      return nullptr;
    }
  }
  return PyPrimType::ToPyObject(val);
}

END_NONAMESPACE


// @brief PrimType オブジェクトを使用可能にする．
bool
PyPrimType::init(
  PyObject* m
)
{
  PrimType_Type.tp_name = "PrimType";
  PrimType_Type.tp_basicsize = sizeof(PrimType_Object);
  PrimType_Type.tp_itemsize = 0;
  PrimType_Type.tp_dealloc = dealloc_func;
  PrimType_Type.tp_repr = repr_func;
  PrimType_Type.tp_hash = hash_func;
  PrimType_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  PrimType_Type.tp_doc = PyDoc_STR("Python extended object for PrimType");
  PrimType_Type.tp_richcompare = richcompare_func;
  PrimType_Type.tp_methods = methods;
  PrimType_Type.tp_getset = getsets;
  PrimType_Type.tp_new = new_func;
  if ( !PyModule::reg_type(m, "PrimType", &PrimType_Type) ) {
    goto error;
  }
  // 定数オブジェクトの生成・登録
  if ( !reg_const_obj("C0", PrimType::C0, Const_C0) ) {
    goto error;
  }
  if ( !reg_const_obj("C1", PrimType::C1, Const_C1) ) {
    goto error;
  }
  if ( !reg_const_obj("Buff", PrimType::Buff, Const_Buff) ) {
    goto error;
  }
  if ( !reg_const_obj("Not", PrimType::Not, Const_Not) ) {
    goto error;
  }
  if ( !reg_const_obj("And", PrimType::And, Const_And) ) {
    goto error;
  }
  if ( !reg_const_obj("Nand", PrimType::Nand, Const_Nand) ) {
    goto error;
  }
  if ( !reg_const_obj("Or", PrimType::Or, Const_Or) ) {
    goto error;
  }
  if ( !reg_const_obj("Nor", PrimType::Nor, Const_Nor) ) {
    goto error;
  }
  if ( !reg_const_obj("Xor", PrimType::Xor, Const_Xor) ) {
    goto error;
  }
  if ( !reg_const_obj("Xnor", PrimType::Xnor, Const_Xnor) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// PrimType を PyObject に変換する．
PyObject*
PyPrimType::Conv::operator()(
  const PrimType& val
)
{
  PyObject* obj = nullptr;
  switch ( val ) {
    case PrimType::C0: obj = Const_C0; break;
    case PrimType::C1: obj = Const_C1; break;
    case PrimType::Buff: obj = Const_Buff; break;
    case PrimType::Not: obj = Const_Not; break;
    case PrimType::And: obj = Const_And; break;
    case PrimType::Nand: obj = Const_Nand; break;
    case PrimType::Or: obj = Const_Or; break;
    case PrimType::Nor: obj = Const_Nor; break;
    case PrimType::Xor: obj = Const_Xor; break;
    case PrimType::Xnor: obj = Const_Xnor; break;
    case PrimType::None: Py_RETURN_NONE;
  }
  if ( obj == nullptr ) {
    PyErr_SetString(PyExc_ValueError, "invalid string for PrimType");
    return nullptr;
  }
  Py_INCREF(obj);
  return obj;
}

// PyObject を PrimType に変換する．
bool
PyPrimType::Deconv::operator()(
  PyObject* obj,
  PrimType& val
)
{
  std::string str_val;
  if ( PyString::FromPyObject(obj, str_val) ) {
    if ( strcasecmp(str_val.c_str(), "C0") == 0 ) {
      val = PrimType::C0;
      return true;
    }
    else if ( strcasecmp(str_val.c_str(), "C1") == 0 ) {
      val = PrimType::C1;
      return true;
    }
    else if ( strcasecmp(str_val.c_str(), "Buff") == 0 ) {
      val = PrimType::Buff;
      return true;
    }
    else if ( strcasecmp(str_val.c_str(), "Not") == 0 ) {
      val = PrimType::Not;
      return true;
    }
    else if ( strcasecmp(str_val.c_str(), "And") == 0 ) {
      val = PrimType::And;
      return true;
    }
    else if ( strcasecmp(str_val.c_str(), "Nand") == 0 ) {
      val = PrimType::Nand;
      return true;
    }
    else if ( strcasecmp(str_val.c_str(), "Or") == 0 ) {
      val = PrimType::Or;
      return true;
    }
    else if ( strcasecmp(str_val.c_str(), "Nor") == 0 ) {
      val = PrimType::Nor;
      return true;
    }
    else if ( strcasecmp(str_val.c_str(), "Xor") == 0 ) {
      val = PrimType::Xor;
      return true;
    }
    else if ( strcasecmp(str_val.c_str(), "Xnor") == 0 ) {
      val = PrimType::Xnor;
      return true;
    }
    return false;
  }
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
  return Py_IS_TYPE(obj, &PrimType_Type);
}

// @brief PyObject から PrimType を取り出す．
PrimType&
PyPrimType::_get_ref(
  PyObject* obj
)
{
  auto my_obj = reinterpret_cast<PrimType_Object*>(obj);
  return my_obj->mVal;
}

// @brief PrimType を表すオブジェクトの型定義を返す．
PyTypeObject*
PyPrimType::_typeobject()
{
  return &PrimType_Type;
}

END_NAMESPACE_YM
