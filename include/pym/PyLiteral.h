#ifndef PYLITERAL_H
#define PYLITERAL_H

/// @file PyLiteral.h
/// @brief PyLiteral のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "ym/Literal.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class PyLiteralConv PyLiteral.h "PyLiteral.h"
/// @brief Literal を PyObject* に変換するファンクタクラス
///
/// 実はただの関数
//////////////////////////////////////////////////////////////////////
class PyLiteralConv
{
public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief Literal を PyObject* に変換する．
  PyObject*
  operator()(
    Literal val
  );

};


//////////////////////////////////////////////////////////////////////
/// @class PyLiteralDeconv PyLiteral.h "PyLiteral.h"
/// @brief Literal を取り出すファンクタクラス
///
/// 実はただの関数
//////////////////////////////////////////////////////////////////////
class PyLiteralDeconv
{
public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief PyObject* から Literal を取り出す．
  bool
  operator()(
    PyObject* obj,
    Literal& val
  );

};


//////////////////////////////////////////////////////////////////////
/// @class PyLiteral PyLiteral.h "PyLiteral.h"
/// @brief Python 用の Literal 拡張
///
/// 複数の関数をひとまとめにしているだけなので実は名前空間として用いている．
//////////////////////////////////////////////////////////////////////
class PyLiteral
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

  /// @brief Literal を表す PyObject を作る．
  /// @return 生成した PyObject を返す．
  ///
  /// 返り値は新しい参照が返される．
  static
  PyObject*
  ToPyObject(
    Literal val ///< [in] 値
  )
  {
    PyLiteralConv conv;
    return conv(val);
  }

  /// @brief PyObject が Literal タイプか調べる．
  static
  bool
  _check(
    PyObject* obj ///< [in] 対象の PyObject
  );

  /// @brief Literal を表す PyObject から Literal を取り出す．
  /// @return Literal を返す．
  ///
  /// _check(obj) == true であると仮定している．
  static
  Literal&
  _get_ref(
    PyObject* obj ///< [in] 変換元の PyObject
  );

  /// @brief Literal を表すオブジェクトの型定義を返す．
  static
  PyTypeObject*
  _typeobject();

};

END_NAMESPACE_YM

#endif // PYLITERAL_H
