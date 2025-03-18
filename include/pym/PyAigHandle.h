#ifndef PYAIGHANDLE_H
#define PYAIGHANDLE_H

/// @file PyAigHandle.h
/// @brief PyAigHandle のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "ym/AigHandle.h"
#include "pym/PyList.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class PyAigHandleConv PyAigHandle.h "PyAigHandle.h"
/// @brief AigHandle を PyObject* に変換するファンクタクラス
///
/// 実はただの関数
//////////////////////////////////////////////////////////////////////
class PyAigHandleConv
{
public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief AigHandle を PyObject* に変換する．
  PyObject*
  operator()(
    const AigHandle& val
  );

};


//////////////////////////////////////////////////////////////////////
/// @class PyAigHandleDeconv PyAigHandle.h "PyAigHandle.h"
/// @brief AigHandle を取り出すファンクタクラス
///
/// 実はただの関数
//////////////////////////////////////////////////////////////////////
class PyAigHandleDeconv
{
public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief PyObject* から AigHandle を取り出す．
  bool
  operator()(
    PyObject* obj,
    AigHandle& val
  );

};


//////////////////////////////////////////////////////////////////////
/// @class PyAigHandle PyAigHandle.h "PyAigHandle.h"
/// @brief Python 用の AigHandle 拡張
///
/// 複数の関数をひとまとめにしているだけなので実は名前空間として用いている．
//////////////////////////////////////////////////////////////////////
class PyAigHandle
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

  /// @brief AigHandle を表す PyObject を作る．
  /// @return 生成した PyObject を返す．
  ///
  /// 返り値は新しい参照が返される．
  static
  PyObject*
  ToPyObject(
    const AigHandle& val ///< [in] 値
  )
  {
    PyAigHandleConv conv;
    return conv(val);
  }

  /// @brief AigHandle のリストを表す PyObject を作る．
  /// @return 生成した PyObject を返す．
  ///
  /// 返り値は新しい参照が返される．
  static
  PyObject*
  ToPyList(
    const vector<AigHandle>& val ///< [in] 値
  )
  {
    return PyList::ToPyObject<AigHandle, PyAigHandleConv>(val);
  }

  /// @brief AigHandle のリストを表す PyObject からの変換
  /// @return エラーが起こったら false を返す．
  static
  bool
  FromPyList(
    PyObject* obj,                 ///< [in] 変換元の PyObject
    vector<AigHandle>& handle_list ///< [out] 結果を格納するオブジェクト
  )
  {
    return PyList::FromPyObject<AigHandle, PyAigHandleDeconv>(obj, handle_list);
  }

  /// @brief PyObject が AigHandle タイプか調べる．
  static
  bool
  _check(
    PyObject* obj ///< [in] 対象の PyObject
  );

  /// @brief AigHandle を表す PyObject から AigHandle を取り出す．
  /// @return AigHandle を返す．
  ///
  /// _check(obj) == true であると仮定している．
  static
  AigHandle&
  _get_ref(
    PyObject* obj ///< [in] 変換元の PyObject
  );

  /// @brief AigHandle を表すオブジェクトの型定義を返す．
  static
  PyTypeObject*
  _typeobject();

};

END_NAMESPACE_YM

#endif // PYAIGHANDLE_H
