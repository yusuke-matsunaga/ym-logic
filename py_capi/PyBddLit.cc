
/// @file PyBddLit.cc
/// @brief PyBddLit の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyBddLit.h"
#include "pym/PyBdd.h"


BEGIN_NAMESPACE_YM

// BddLit を PyObject に変換する．
PyObject*
PyBddLit::Conv::operator()(
  const BddLit& val
)
{
  // 実は PyBdd を使う．
  return PyBdd::ToPyObject(val);
}

// PyObject を BddLit に変換する．
bool
PyBddLit::Deconv::operator()(
  PyObject* obj,
  BddLit& val
)
{
  Bdd bdd;
  if ( PyBdd::FromPyObject(obj, bdd) ) {
    if ( bdd.is_literal() ) {
      val = BddLit::from_bdd(bdd);
      return true;
    }
  }
  return false;
}

// @brief PyObject が BddLit タイプか調べる．
bool
PyBddLit::Check(
  PyObject* obj
)
{
  if ( PyBdd::Check(obj) ) {
    auto& bdd = PyBdd::_get_ref(obj);
    return bdd.is_literal();
  }
  return false;
}

END_NAMESPACE_YM
