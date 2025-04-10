#ifndef PYBDDLIT_H
#define PYBDDLIT_H

/// @file PyBddLit.h
/// @brief PyBddLit のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "ym/BddLit.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class PyBddLit PyBddLit.h "PyBddLit.h"
/// @brief BddLit と PyObject* の間の変換を行うクラス
///
/// 実は BddLit は対応する Python の拡張クラスを持たない．
/// 内部では PyBdd で表される．
//////////////////////////////////////////////////////////////////////
class PyBddLit
{
public:

  using ElemType = BddLit;

public:

  /// @brief BddLit を PyObject* に変換するファンクタクラス
  struct Conv {
    PyObject*
    operator()(
      const ElemType& val
    );
  };

  /// @brief PyObject* から BddLit を取り出すファンクタクラス
  struct Deconv {
    bool
    operator()(
      PyObject* obj,
      ElemType& val
    );
  };


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief BddLit を表す PyObject を作る．
  /// @return 生成した PyObject を返す．
  ///
  /// 返り値は新しい参照が返される．
  static
  PyObject*
  ToPyObject(
    const ElemType& val ///< [in] 値
  )
  {
    Conv conv;
    return conv(val);
  }

  /// @brief PyObject から BddLit を取り出す．
  /// @return 正しく変換できた時に true を返す．
  static
  bool
  FromPyObject(
    PyObject* obj, ///< [in] Python のオブジェクト,
    ElemType& val  ///< [out] 結果を格納する変数
  )
  {
    Deconv deconv;
    return deconv(obj, val);
  }

  /// @brief PyObject が BddLit タイプか調べる．
  static
  bool
  Check(
    PyObject* obj ///< [in] 対象の PyObject
  );

  /// @brief PyObject から BddLit を取り出す．
  static
  ElemType
  Get(
    PyObject* obj ///< [in] 対象の Python オブジェクト
  )
  {
    ElemType val;
    if ( FromPyObject(obj, val) ) {
      return val;
    }
    PyErr_SetString(PyExc_TypeError, "Could not convert to BddLit");
    return val;
  }

};

END_NAMESPACE_YM

#endif // PYBDDLIT_H
