
/// @file ymlogic_module.cc
/// @brief Python 用の ymlogic モジュールを定義する．
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "ym_config.h"
#include "ym/PyExpr.h"
#include "ym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// メソッド定義構造体
PyMethodDef ymlogic_methods[] = {
  {nullptr, nullptr, 0, nullptr},
};

// モジュール定義構造体
PyModuleDef ymlogic_module = {
  PyModuleDef_HEAD_INIT,
  "ymlogic",
  PyDoc_STR("ymlogic: Extension module for ym-logic"),
  -1,
  ymlogic_methods,
};

END_NONAMESPACE

PyMODINIT_FUNC
PyInit_ymlogic()
{
  auto m = PyModule::init(&ymlogic_module);
  if ( m == nullptr ) {
    return nullptr;
  }

  if ( !PyExpr::init(m) ) {
    goto error;
  }

  return m;

 error:
  Py_DECREF(m);
  return nullptr;
}

END_NAMESPACE_YM
