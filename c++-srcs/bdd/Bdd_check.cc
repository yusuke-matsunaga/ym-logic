
/// @file Bdd.cc
/// @brief Bdd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Bdd.h"
#include "CheckSupOp.h"
#include "CheckSymOp.h"


BEGIN_NAMESPACE_YM_BDD

// @brief 与えられた変数がサポートの時 true を返す．
bool
Bdd::check_sup(
  VarId var
) const
{
  CheckSupOp op{var.val()};
  return op.op_step(mRoot);
}

// @brief 与えられた変数に対して対称の時 true を返す．
bool
Bdd::check_sym(
  VarId var1,
  VarId var2,
  bool inv
) const
{
  CheckSymOp op{var1.val(), var2.val(), inv};
  return op.op_step(mRoot);
}

END_NAMESPACE_YM_BDD
