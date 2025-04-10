
/// @file PyBddVar.cc
/// @brief PyBddVar の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyBddVar.h"
#include "pym/PyBdd.h"


BEGIN_NAMESPACE_YM

// BddVar を PyObject に変換する．
PyObject*
PyBddVar::Conv::operator()(
  const BddVar& val
)
{
  // 実は PyBdd を使う．
  return PyBdd::ToPyObject(val);
}

// PyObject を BddVar に変換する．
bool
PyBddVar::Deconv::operator()(
  PyObject* obj,
  BddVar& val
)
{
  Bdd bdd;
  if ( PyBdd::FromPyObject(obj, bdd) ) {
    if ( bdd.is_variable() ) {
      val = BddVar::from_bdd(bdd);
      return true;
    }
  }
  return false;
}

// @brief PyObject が BddVar タイプか調べる．
bool
PyBddVar::Check(
  PyObject* obj
)
{
  if ( PyBdd::Check(obj) ) {
    auto& bdd = PyBdd::_get_ref(obj);
    return bdd.is_variable();
  }
  return false;
}

END_NAMESPACE_YM
