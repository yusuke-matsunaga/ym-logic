#ifndef PYBDDLIT_H
#define PYBDDLIT_H

/// @file PyBddLit.h
/// @brief PyBddLit のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "ym/BddLit.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class PyBddLit PyBddLit.h "pym/PyBddLit.h"
/// @brief Python 用の BddLit 拡張
///
/// 複数の関数をひとまとめにしているだけなので実は名前空間として用いている．
//////////////////////////////////////////////////////////////////////
class PyBddLit
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

  /// @brief BddLit を表す PyObject を作る．
  /// @return 生成した PyObject を返す．
  ///
  /// 返り値は新しい参照が返される．
  static
  PyObject*
  ToPyObject(
    const BddLit& val ///< [in] 値
  );

  /// @brief PyObject が BddLit タイプか調べる．
  static
  bool
  Check(
    PyObject* obj ///< [in] 対象の PyObject
  );

  /// @brief BddLit を表す PyObject から BddLit を取り出す．
  /// @return BddLit を返す．
  ///
  /// Check(obj) == true であると仮定している．
  static
  BddLit
  Get(
    PyObject* obj ///< [in] 変換元の PyObject
  );

  /// @brief BddLit を表す PyObject から BddLit を取り出す．
  /// @return BddLit を返す．
  ///
  /// Check(obj) == true であると仮定している．
  static
  BddLit&
  _get(
    PyObject* obj ///< [in] 変換元の PyObject
  );

  /// @brief BddLit を表すオブジェクトの型定義を返す．
  static
  PyTypeObject*
  _typeobject();

};

END_NAMESPACE_YM

#endif // PYBDDLIT_H
