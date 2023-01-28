#ifndef PYBDDVARSET_H
#define PYBDDVARSET_H

/// @file PyBddVarSet.h
/// @brief PyBddVarSet のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "ym/BddVarSet.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class PyBddVarSet PyBddVarSet.h "PyBddVarSet.h"
/// @brief Python 用の BddVarSet 拡張
///
/// 複数の関数をひとまとめにしているだけなので実は名前空間として用いている．
//////////////////////////////////////////////////////////////////////
class PyBddVarSet
{
public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化する．
  /// @return 初期化が成功したら true を返す．
  static
  bool
  init(
    PyObject* m ///< [in] 親のモジュールを表す PyObject
  );

  /// @brief BddVarSet を表す PyObject を作る．
  /// @return 生成した PyObject を返す．
  ///
  /// 返り値は新しい参照が返される．
  static
  PyObject*
  ToPyObject(
    const BddVarSet& val ///< [in] 値
  );

  /// @brief PyObject が BddVarSet タイプか調べる．
  static
  bool
  _check(
    PyObject* obj ///< [in] 対象の PyObject
  );

  /// @brief BddVarSet を表す PyObject から BddVarSet を取り出す．
  /// @return BddVarSet を返す．
  ///
  /// _check(obj) == true であると仮定している．
  static
  BddVarSet&
  _get(
    PyObject* obj ///< [in] 変換元の PyObject
  );

  /// @brief BddVarSet を表すオブジェクトの型定義を返す．
  static
  PyTypeObject*
  _typeobject();

};

END_NAMESPACE_YM

#endif // PYBDDVARSET_H
