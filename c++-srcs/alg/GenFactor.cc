
/// @file AlgCover_GenFactor.cc
/// @brief AlgCover の GenFactor 関係の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/AlgCover.h"
#include "GenFactor.h"


BEGIN_NAMESPACE_YM_ALG

//////////////////////////////////////////////////////////////////////
// クラス AlgCover
//////////////////////////////////////////////////////////////////////

// @brief "quick factor" を行う．
Expr
AlgCover::quick_factor() const
{
  QuickFactor qf;
  return qf(*this);
}

// @brief "good factor" を行う．
Expr
AlgCover::good_factor() const
{
  GoodFactor gf;
  return gf(*this);
}

END_NAMESPACE_YM_ALG
