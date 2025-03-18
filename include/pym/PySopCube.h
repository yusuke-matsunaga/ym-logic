#ifndef PYSOPCUBE_H
#define PYSOPCUBE_H

/// @file PySopCube.h
/// @brief PySopCube のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "ym/SopCube.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class PySopCubeConv PySopCube.h "PySopCube.h"
/// @brief SopCube を PyObject* に変換するファンクタクラス
///
/// 実はただの関数
//////////////////////////////////////////////////////////////////////
class PySopCubeConv
{
public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief SopCube を PyObject* に変換する．
  PyObject*
  operator()(
    const SopCube& val
  );

  /// @brief SopCube を PyObject* に変換する．
  PyObject*
  operator()(
    SopCube&& val
  );

};


//////////////////////////////////////////////////////////////////////
/// @class PySopCubeDeconv PySopCube.h "PySopCube.h"
/// @brief SopCube を取り出すファンクタクラス
///
/// 実はただの関数
//////////////////////////////////////////////////////////////////////
class PySopCubeDeconv
{
public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief PyObject* から SopCube を取り出す．
  bool
  operator()(
    PyObject* obj,
    SopCube& val
  );

};


//////////////////////////////////////////////////////////////////////
/// @class PySopCube PySopCube.h "PySopCube.h"
/// @brief Python 用の SopCube 拡張
///
/// 複数の関数をひとまとめにしているだけなので実は名前空間として用いている．
//////////////////////////////////////////////////////////////////////
class PySopCube
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

  /// @brief SopCube を表す PyObject を作る．
  /// @return 生成した PyObject を返す．
  ///
  /// 返り値は新しい参照が返される．
  static
  PyObject*
  ToPyObject(
    const SopCube& val ///< [in] 値
  )
  {
    PySopCubeConv conv;
    return conv(val);
  }

  /// @brief SopCube を表す PyObject を作る．
  /// @return 生成した PyObject を返す．
  ///
  /// 返り値は新しい参照が返される．
  static
  PyObject*
  ToPyObject(
    SopCube&& val ///< [in] 値
  )
  {
    PySopCubeConv conv;
    return conv(val);
  }

  /// @brief PyObject が SopCube タイプか調べる．
  static
  bool
  _check(
    PyObject* obj ///< [in] 対象の PyObject
  );

  /// @brief SopCube を表す PyObject から SopCube を取り出す．
  /// @return SopCube を返す．
  ///
  /// _check(obj) == true であると仮定している．
  static
  SopCube&
  _get_ref(
    PyObject* obj ///< [in] 変換元の PyObject
  );

  /// @brief SopCube を表すオブジェクトの型定義を返す．
  static
  PyTypeObject*
  _typeobject();

};

END_NAMESPACE_YM

#endif // PYSOPCUBE_H
