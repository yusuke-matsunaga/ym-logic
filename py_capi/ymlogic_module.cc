
/// @file ymlogic_module.cc
/// @brief Python 用の ymlogic モジュールを定義する．
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "pym/PyPrimType.h"
#include "pym/PyLiteral.h"
#include "pym/PyNpnMap.h"
#include "pym/PyTvFunc.h"
#include "pym/PyExpr.h"
#include "pym/PyBdd.h"
#include "pym/PyBddMgr.h"
#include "pym/PySopCover.h"
#include "pym/PySopCube.h"
#include "pym/PyAigHandle.h"
#include "pym/PyAigMgr.h"
#include "pym/PyModule.h"


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

  if ( !PyPrimType::init(m) ) {
    goto error;
  }
  if ( !PyLiteral::init(m) ) {
    goto error;
  }
  if ( !PyNpnMap::init(m) ) {
    goto error;
  }
  if ( !PyTvFunc::init(m) ) {
    goto error;
  }
  if ( !PyExpr::init(m) ) {
    goto error;
  }
  if ( !PyBdd::init(m) ) {
    goto error;
  }
  if ( !PyBddMgr::init(m) ) {
    goto error;
  }
  if ( !PySopCover::init(m) ) {
    goto error;
  }
  if ( !PySopCube::init(m) ) {
    goto error;
  }
  if ( !PyAigHandle::init(m) ) {
    goto error;
  }
  if ( !PyAigMgr::init(m) ) {
    goto error;
  }

  return m;

 error:
  Py_DECREF(m);
  return nullptr;
}

END_NAMESPACE_YM
