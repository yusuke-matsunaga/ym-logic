#ifndef PYAIGMGR_H
#define PYAIGMGR_H

/// @file PyAigMgr.h
/// @brief PyAigMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "ym/AigMgr.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class PyAigMgr PyAigMgr.h "PyAigMgr.h"
/// @brief Python 用の AigMgr 拡張
///
/// 複数の関数をひとまとめにしているだけなので実は名前空間として用いている．
//////////////////////////////////////////////////////////////////////
class PyAigMgr
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

  /// @brief PyObject が AigMgr タイプか調べる．
  static
  bool
  _check(
    PyObject* obj ///< [in] 対象の PyObject
  );

  /// @brief AigMgr を表す PyObject から AigMgr を取り出す．
  /// @return AigMgr を返す．
  ///
  /// Check(obj) == true であると仮定している．
  static
  AigMgr&
  _get_ref(
    PyObject* obj ///< [in] 変換元の PyObject
  );

  /// @brief AigMgr を表すオブジェクトの型定義を返す．
  static
  PyTypeObject*
  _typeobject();

};

END_NAMESPACE_YM

#endif // PYAIGMGR_H
