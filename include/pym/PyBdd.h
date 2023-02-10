#ifndef PYBDD_H
#define PYBDD_H

/// @file PyBdd.h
/// @brief PyBdd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "ym/Bdd.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class PyBdd PyBdd.h "PyBdd.h"
/// @brief Python 用の Bdd 拡張
///
/// 複数の関数をひとまとめにしているだけなので実は名前空間として用いている．
//////////////////////////////////////////////////////////////////////
class PyBdd
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

  /// @brief Bdd を表す PyObject を作る．
  /// @return 生成した PyObject を返す．
  ///
  /// 返り値は新しい参照が返される．
  static
  PyObject*
  ToPyObject(
    const Bdd& val ///< [in] 値
  );

  /// @brief PyObject が Bdd タイプか調べる．
  static
  bool
  Check(
    PyObject* obj ///< [in] 対象の PyObject
  );

  /// @brief Bdd を表す PyObject から Bdd を取り出す．
  /// @return Bdd を返す．
  ///
  /// Check(obj) == true であると仮定している．
  static
  Bdd
  Get(
    PyObject* obj ///< [in] 変換元の PyObject
  );

  /// @brief Bdd を表すオブジェクトの型定義を返す．
  static
  PyTypeObject*
  _typeobject();

};

END_NAMESPACE_YM

#endif // PYBDD_H
