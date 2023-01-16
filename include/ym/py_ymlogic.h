#ifndef PY_YMLOGIC_H
#define PY_YMLOGIC_H

/// @file py_ymlogic.h
/// @brief py_ymlogic のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"


BEGIN_NAMESPACE_YM

/// @brief ymlogic モジュールの初期化関数
extern "C" PyObject* PyInit_ymlogic();

END_NAMESPACE_YM

#endif // PY_YMLOGIC_H
