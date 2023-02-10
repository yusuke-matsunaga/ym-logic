#ifndef PYTVFUNC_H
#define PYTVFUNC_H

/// @file PyTvFunc.h
/// @brief PyTvFunc のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022, 2023 Yusuke Matsunaga
/// All rights reserved.

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "ym/TvFunc.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class PyTvFunc PyTvFunc.h "PyTvFunc.h"
/// @brief Python 用の TvFunc 拡張
///
/// 複数の関数をひとまとめにしているだけなので実は名前空間として用いている．
//////////////////////////////////////////////////////////////////////
class PyTvFunc
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

  /// @brief TvFunc を表す PyObject を作る．
  /// @return 生成した PyObject を返す．
  ///
  /// 返り値は新しい参照が返される．
  static
  PyObject*
  ToPyObject(
    const TvFunc& val ///< [in] 値
  );

  /// @brief TvFunc を表す PyObject を作る．
  /// @return 生成した PyObject を返す．
  ///
  /// 返り値は新しい参照が返される．
  static
  PyObject*
  ToPyObject(
    TvFunc&& val ///< [in] 値
  );

  /// @brief PyObject が TvFunc タイプか調べる．
  static
  bool
  Check(
    PyObject* obj ///< [in] 対象の PyObject
  );

  /// @brief TvFunc を表す PyObject から TvFunc を取り出す．
  /// @return TvFunc を返す．
  ///
  /// _check(obj) == true であると仮定している．
  static
  TvFunc&
  Get(
    PyObject* obj ///< [in] 変換元の PyObject
  );

  /// @brief TvFunc を表すオブジェクトの型定義を返す．
  static
  PyTypeObject*
  _typeobject();

};

END_NAMESPACE_YM

#endif // PYTVFUNC_H
