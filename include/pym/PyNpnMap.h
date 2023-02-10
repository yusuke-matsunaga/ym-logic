#ifndef PYNPNMAP_H
#define PYNPNMAP_H

/// @file PyNpnMap.h
/// @brief PyNpnMap のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "ym/NpnMap.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class PyNpnMap PyNpnMap.h "PyNpnMap.h"
/// @brief Python 用の NpnMap 拡張
///
/// 複数の関数をひとまとめにしているだけなので実は名前空間として用いている．
//////////////////////////////////////////////////////////////////////
class PyNpnMap
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

  /// @brief NpnMap を表す PyObject を作る．
  /// @return 生成した PyObject を返す．
  ///
  /// 返り値は新しい参照が返される．
  static
  PyObject*
  ToPyObject(
    const NpnMap& val ///< [in] 値
  );

  /// @brief PyObject が NpnMap タイプか調べる．
  static
  bool
  Check(
    PyObject* obj ///< [in] 対象の PyObject
  );

  /// @brief NpnMap を表す PyObject から NpnMap を取り出す．
  /// @return NpnMap を返す．
  ///
  /// Check(obj) == true であると仮定している．
  static
  NpnMap&
  Get(
    PyObject* obj ///< [in] 変換元の PyObject
  );

  /// @brief NpnMap を表すオブジェクトの型定義を返す．
  static
  PyTypeObject*
  _typeobject();

};

END_NAMESPACE_YM

#endif // PYNPNMAP_H
