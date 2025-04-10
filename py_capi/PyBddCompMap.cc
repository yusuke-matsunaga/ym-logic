
/// @file PyBddCompMap.cc
/// @brief PyBddCompMap の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyBddCompMap.h"
#include "pym/PyBdd.h"
#include "pym/PyBddVar.h"


BEGIN_NAMESPACE_YM

// 辞書を PyObject に変換する．
PyObject*
PyBddCompMap::Conv::operator()(
  const ElemType& val
)
{
  // 未実装
  PyErr_SetString(PyExc_ValueError, "not implemented");
  return nullptr;
}

// PyObject* を辞書に変換する．
bool
PyBddCompMap::Deconv::operator()(
  PyObject* obj,
  ElemType& val
)
{
  val.clear();
  if ( !PyDict_Check(obj) ) {
    return false;
  }
  Py_ssize_t pos = 0;
  PyObject* key_obj = nullptr;
  PyObject* val_obj = nullptr;
  while ( PyDict_Next(obj, &pos, &key_obj, &val_obj) ) {
    BddVar var;
    if ( !PyBddVar::FromPyObject(key_obj, var) ) {
      return false;
    }
    Bdd bdd;
    if ( !PyBdd::FromPyObject(val_obj, bdd) ) {
      return false;
    }
    val.emplace(var, bdd);
  }
  return true;
}

// @brief PyObject が BddCompMap タイプか調べる．
bool
PyBddCompMap::Check(
  PyObject* obj
)
{
  // 非常に効率の悪いコード
  ElemType dummy;
  return FromPyObject(obj, dummy);
}

END_NAMESPACE_YM
