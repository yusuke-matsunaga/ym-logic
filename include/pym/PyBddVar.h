#ifndef PYBDDVAR_H
#define PYBDDVAR_H

/// @file PyBddVar.h
/// @brief PyBddVar のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "ym/BddVar.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class PyBddVar PyBddVar.h "pym/PyBddVar.h"
/// @brief Python 用の BddVar 拡張
///
/// 複数の関数をひとまとめにしているだけなので実は名前空間として用いている．
//////////////////////////////////////////////////////////////////////
class PyBddVar
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

  /// @brief BddVar を表す PyObject を作る．
  /// @return 生成した PyObject を返す．
  ///
  /// 返り値は新しい参照が返される．
  static
  PyObject*
  ToPyObject(
    const BddVar& val ///< [in] 値
  );

  /// @brief PyObject が BddVar タイプか調べる．
  static
  bool
  Check(
    PyObject* obj ///< [in] 対象の PyObject
  );

  /// @brief BddVar を表す PyObject から BddVar を取り出す．
  /// @return BddVar を返す．
  ///
  /// Check(obj) == true であると仮定している．
  static
  BddVar
  Get(
    PyObject* obj ///< [in] 変換元の PyObject
  );

  /// @brief BddVar を表す PyObject から BddVar を取り出す．
  /// @return BddVar を返す．
  ///
  /// Check(obj) == true であると仮定している．
  static
  BddVar&
  _get(
    PyObject* obj ///< [in] 変換元の PyObject
  );

  /// @brief BddVar を表すオブジェクトの型定義を返す．
  static
  PyTypeObject*
  _typeobject();

};

END_NAMESPACE_YM

#endif // PYBDDVAR_H
