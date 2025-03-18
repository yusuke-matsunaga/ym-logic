#ifndef PYPRIMTYPE_H
#define PYPRIMTYPE_H

/// @file PyPrimType.h
/// @brief PyPrimType のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "ym/logic.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class PyPrimTypeConv PyPrimType.h "PyPrimType.h"
/// @brief PrimType を PyObject* に変換するファンクタクラス
///
/// 実はただの関数
//////////////////////////////////////////////////////////////////////
class PyPrimTypeConv
{
public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief PrimType を PyObject* に変換する．
  PyObject*
  operator()(
    PrimType val
  );

};


//////////////////////////////////////////////////////////////////////
/// @class PyPrimTypeDeconv PyPrimType.h "PyPrimType.h"
/// @brief PrimType を取り出すファンクタクラス
///
/// 実はただの関数
//////////////////////////////////////////////////////////////////////
class PyPrimTypeDeconv
{
public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief PyObject* から PrimType を取り出す．
  bool
  operator()(
    PyObject* obj,
    PrimType& val
  );

};


//////////////////////////////////////////////////////////////////////
/// @class PyPrimType PyPrimType.h "PyPrimType.h"
/// @brief Python 用の PrimType 拡張
///
/// 複数の関数をひとまとめにしているだけなので実は名前空間として用いている．
//////////////////////////////////////////////////////////////////////
class PyPrimType
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

  /// @brief PrimType を表す PyObject を作る．
  /// @return 生成した PyObject を返す．
  ///
  /// 返り値は新しい参照が返される．
  static
  PyObject*
  ToPyObject(
    PrimType val ///< [in] 値
  )
  {
    PyPrimTypeConv conv;
    return conv(val);
  }

  /// @brief PyObject が PrimType タイプか調べる．
  static
  bool
  _check(
    PyObject* obj ///< [in] 対象の PyObject
  );

  /// @brief PrimType を表す PyObject から PrimType を取り出す．
  /// @return PrimType を返す．
  ///
  /// _check(obj) == true であると仮定している．
  static
  PrimType&
  _get_ref(
    PyObject* obj ///< [in] 変換元の PyObject
  );

  /// @brief PrimType を表すオブジェクトの型定義を返す．
  static
  PyTypeObject*
  _typeobject();

};

END_NAMESPACE_YM

#endif // PYPRIMTYPE_H
