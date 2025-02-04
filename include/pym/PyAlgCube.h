#ifndef PYALGCUBE_H
#define PYALGCUBE_H

/// @file PyAlgCube.h
/// @brief PyAlgCube のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "ym/AlgCube.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class PyAlgCube PyAlgCube.h "PyAlgCube.h"
/// @brief Python 用の AlgCube 拡張
///
/// 複数の関数をひとまとめにしているだけなので実は名前空間として用いている．
//////////////////////////////////////////////////////////////////////
class PyAlgCube
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

  /// @brief AlgCube を表す PyObject を作る．
  /// @return 生成した PyObject を返す．
  ///
  /// 返り値は新しい参照が返される．
  static
  PyObject*
  ToPyObject(
    const AlgCube& val ///< [in] 値
  );

  /// @brief AlgCube を表す PyObject を作る．
  /// @return 生成した PyObject を返す．
  ///
  /// 返り値は新しい参照が返される．
  static
  PyObject*
  ToPyObject(
    const AlgCube&& val ///< [in] 値
  );

  /// @brief PyObject が AlgCube タイプか調べる．
  static
  bool
  Check(
    PyObject* obj ///< [in] 対象の PyObject
  );

  /// @brief AlgCube を表す PyObject から AlgCube を取り出す．
  /// @return AlgCube を返す．
  ///
  /// _check(obj) == true であると仮定している．
  static
  AlgCube&
  Get(
    PyObject* obj ///< [in] 変換元の PyObject
  );

  /// @brief AlgCube を表すオブジェクトの型定義を返す．
  static
  PyTypeObject*
  _typeobject();

};

END_NAMESPACE_YM

#endif // PYALGCUBE_H
