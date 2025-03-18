#ifndef PYEXPR_H
#define PYEXPR_H

/// @file PyExpr.h
/// @brief PyExpr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022, 2023 Yusuke Matsunaga
/// All rights reserved.

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "ym/Expr.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class PyExprConv PyExpr.h "PyExpr.h"
/// @brief Expr を PyObject* に変換するファンクタクラス
///
/// 実はただの関数
//////////////////////////////////////////////////////////////////////
class PyExprConv
{
public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief Expr を PyObject* に変換する．
  PyObject*
  operator()(
    const Expr& val
  );

};


//////////////////////////////////////////////////////////////////////
/// @class PyExprDeconv PyExpr.h "PyExpr.h"
/// @brief Expr を取り出すファンクタクラス
///
/// 実はただの関数
//////////////////////////////////////////////////////////////////////
class PyExprDeconv
{
public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief PyObject* から Expr を取り出す．
  bool
  operator()(
    PyObject* obj,
    Expr& val
  );

};


//////////////////////////////////////////////////////////////////////
/// @class PyExpr PyExpr.h "PyExpr.h"
/// @brief Python 用の Expr 拡張
///
/// 複数の関数をひとまとめにしているだけなので実は名前空間として用いている．
//////////////////////////////////////////////////////////////////////
class PyExpr
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

  /// @brief Expr を表す PyObject を作る．
  /// @return 生成した PyObject を返す．
  ///
  /// 返り値は新しい参照が返される．
  static
  PyObject*
  ToPyObject(
    const Expr& val ///< [in] 値
  )
  {
    PyExprConv conv;
    return conv(val);
  }

  /// @brief PyObject が Expr タイプか調べる．
  static
  bool
  _check(
    PyObject* obj ///< [in] 対象の PyObject
  );

  /// @brief Expr を表す PyObject から Expr を取り出す．
  /// @return Expr を返す．
  ///
  /// _check(obj) == true であると仮定している．
  static
  Expr&
  _get_ref(
    PyObject* obj ///< [in] 変換元の PyObject
  );

  /// @brief Expr を表すオブジェクトの型定義を返す．
  static
  PyTypeObject*
  _typeobject();

};

END_NAMESPACE_YM

#endif // PYEXPR_H
