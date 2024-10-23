
/// @file py_ymlogic.cc
/// @brief py_ymlogic の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "ym_config.h"


BEGIN_NAMESPACE_YM

extern "C" PyObject* PyInit_ymlogic();

END_NAMESPACE_YM

int
main(
  int argc,
  char** argv
)
{
  PyImport_AppendInittab("ymlogic", &YM_NAMESPACE::PyInit_ymlogic);

  return Py_BytesMain(argc, argv);
}
