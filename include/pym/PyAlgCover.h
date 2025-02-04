#ifndef PYALGCOVER_H
#define PYALGCOVER_H

/// @file PyAlgCover.h
/// @brief PyAlgCover のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "ym/AlgCover.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class PyAlgCover PyAlgCover.h "PyAlgCover.h"
/// @brief Python 用の AlgCover 拡張
///
/// 複数の関数をひとまとめにしているだけなので実は名前空間として用いている．
//////////////////////////////////////////////////////////////////////
class PyAlgCover
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

  /// @brief AlgCover を表す PyObject を作る．
  /// @return 生成した PyObject を返す．
  ///
  /// 返り値は新しい参照が返される．
  static
  PyObject*
  ToPyObject(
    const AlgCover& val ///< [in] 値
  );

  /// @brief AlgCover を表す PyObject を作る．
  /// @return 生成した PyObject を返す．
  ///
  /// 返り値は新しい参照が返される．
  static
  PyObject*
  ToPyObject(
    const AlgCover&& val ///< [in] 値
  );

  /// @brief PyObject が AlgCover タイプか調べる．
  static
  bool
  Check(
    PyObject* obj ///< [in] 対象の PyObject
  );

  /// @brief AlgCover を表す PyObject から AlgCover を取り出す．
  /// @return AlgCover を返す．
  ///
  /// Check(obj) == true であると仮定している．
  static
  AlgCover&
  Get(
    PyObject* obj ///< [in] 変換元の PyObject
  );

  /// @brief AlgCover を表すオブジェクトの型定義を返す．
  static
  PyTypeObject*
  _typeobject();

};

END_NAMESPACE_YM

#endif // PYALGCOVER_H
