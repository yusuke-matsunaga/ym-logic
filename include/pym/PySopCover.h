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
  );

  /// @brief SopCover を表す PyObject を作る．
  /// @return 生成した PyObject を返す．
  ///
  /// 返り値は新しい参照が返される．
  static
  PyObject*
  ToPyObject(
    const SopCover&& val ///< [in] 値
  );

  /// @brief PyObject が SopCover タイプか調べる．
  static
  bool
  Check(
    PyObject* obj ///< [in] 対象の PyObject
  );

  /// @brief SopCover を表す PyObject から SopCover を取り出す．
  /// @return SopCover を返す．
  ///
  /// Check(obj) == true であると仮定している．
  static
  SopCover&
  Get(
    PyObject* obj ///< [in] 変換元の PyObject
  );

  /// @brief SopCover を表すオブジェクトの型定義を返す．
  static
  PyTypeObject*
  _typeobject();

};

END_NAMESPACE_YM

#endif // PYSOPCOVER_H
