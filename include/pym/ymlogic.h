#ifndef PYM_YMLOGIC_H
#define PYM_YMLOGIC_H

/// @file ymlogic.h
/// @brief ymlogic の初期化関数のエクスポート
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"


BEGIN_NAMESPACE_YM

/// @brief ymlogic モジュールの初期化関数
extern "C" PyObject* PyInit_ymlogic();

END_NAMESPACE_YM

#endif // PYM_YMLOGIC_H
