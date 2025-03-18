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
/// @class PyNpnMapConv PyNpnMap.h "PyNpnMap.h"
/// @brief NpnMap を PyObject* に変換するファンクタクラス
///
/// 実はただの関数
//////////////////////////////////////////////////////////////////////
class PyNpnMapConv
{
public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief NpnMap を PyObject* に変換する．
  PyObject*
  operator()(
    const NpnMap& val
  );

};


//////////////////////////////////////////////////////////////////////
/// @class PyNpnMapDeconv PyNpnMap.h "PyNpnMap.h"
/// @brief NpnMap を取り出すファンクタクラス
///
/// 実はただの関数
//////////////////////////////////////////////////////////////////////
class PyNpnMapDeconv
{
public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief PyObject* から NpnMap を取り出す．
  bool
  operator()(
    PyObject* obj,
    NpnMap& val
  );

};


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
  )
  {
    PyNpnMapConv conv;
    return conv(val);
  }

  /// @brief PyObject が NpnMap タイプか調べる．
  static
  bool
  _check(
    PyObject* obj ///< [in] 対象の PyObject
  );

  /// @brief NpnMap を表す PyObject から NpnMap を取り出す．
  /// @return NpnMap を返す．
  ///
  /// _check(obj) == true であると仮定している．
  static
  NpnMap&
  _get_ref(
    PyObject* obj ///< [in] 変換元の PyObject
  );

  /// @brief NpnMap を表すオブジェクトの型定義を返す．
  static
  PyTypeObject*
  _typeobject();

};

END_NAMESPACE_YM

#endif // PYNPNMAP_H
