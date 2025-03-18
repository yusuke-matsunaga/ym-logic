#ifndef PYSOPCOVER_H
#define PYSOPCOVER_H

/// @file PySopCover.h
/// @brief PySopCover のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "ym/SopCover.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class PySopCoverConv PySopCover.h "PySopCover.h"
/// @brief SopCover を PyObject* に変換するファンクタクラス
///
/// 実はただの関数
//////////////////////////////////////////////////////////////////////
class PySopCoverConv
{
public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief SopCover を PyObject* に変換する．
  PyObject*
  operator()(
    const SopCover& val
  );

  /// @brief SopCover を PyObject* に変換する．
  PyObject*
  operator()(
    SopCover&& val
  );

};


//////////////////////////////////////////////////////////////////////
/// @class PySopCoverDeconv PySopCover.h "PySopCover.h"
/// @brief SopCover を取り出すファンクタクラス
///
/// 実はただの関数
//////////////////////////////////////////////////////////////////////
class PySopCoverDeconv
{
public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief PyObject* から SopCover を取り出す．
  bool
  operator()(
    PyObject* obj,
    SopCover& val
  );

};


//////////////////////////////////////////////////////////////////////
/// @class PySopCover PySopCover.h "PySopCover.h"
/// @brief Python 用の SopCover 拡張
///
/// 複数の関数をひとまとめにしているだけなので実は名前空間として用いている．
//////////////////////////////////////////////////////////////////////
class PySopCover
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

  /// @brief SopCover を表す PyObject を作る．
  /// @return 生成した PyObject を返す．
  ///
  /// 返り値は新しい参照が返される．
  static
  PyObject*
  ToPyObject(
    const SopCover& val ///< [in] 値
  )
  {
    PySopCoverConv conv;
    return conv(val);
  }

  /// @brief SopCover を表す PyObject を作る．
  /// @return 生成した PyObject を返す．
  ///
  /// 返り値は新しい参照が返される．
  static
  PyObject*
  ToPyObject(
    SopCover&& val ///< [in] 値
  )
  {
    PySopCoverConv conv;
    return conv(val);
  }

  /// @brief PyObject が SopCover タイプか調べる．
  static
  bool
  _check(
    PyObject* obj ///< [in] 対象の PyObject
  );

  /// @brief SopCover を表す PyObject から SopCover を取り出す．
  /// @return SopCover を返す．
  ///
  /// Check(obj) == true であると仮定している．
  static
  SopCover&
  _get_ref(
    PyObject* obj ///< [in] 変換元の PyObject
  );

  /// @brief SopCover を表すオブジェクトの型定義を返す．
  static
  PyTypeObject*
  _typeobject();

};

END_NAMESPACE_YM

#endif // PYSOPCOVER_H
