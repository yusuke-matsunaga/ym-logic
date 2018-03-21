﻿#ifndef EXPR_TYPES_H
#define EXPR_TYPES_H

/// @file lexp_types.h
/// @brief libym_lexp で内部的に用いる型定義
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/Expr.h"


BEGIN_NAMESPACE_YM_LOGIC

/// @brief 演算子の種類を表す列挙型
///
/// 定数やリテラルも含む．
/// 型判定の dirty trick のために値の指定が必要．
enum class ExprType {
  Const0      = 0,
  Const1      = 1,
  PosiLiteral = 2,
  NegaLiteral = 3,
  And         = 4,
  Or          = 5,
  Xor         = 6
};

/// @brief 参照回数の最大値
const ymuint32 kRefMax = 1UL << (sizeof(ymuint32) * 8 - 4);

END_NAMESPACE_YM_LOGIC

#endif // EXPR_TYPES_H