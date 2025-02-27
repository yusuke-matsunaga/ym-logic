
/// @file SopCover_GenFactor.cc
/// @brief SopCover の GenFactor 関係の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/SopCover.h"
#include "ym/Tv2Sop.h"
#include "GenFactor.h"
#include "OneLevel0Kernel.h"
#include "BestKernel.h"
#include "WeakDivision.h"
#include "BoolDivision.h"


BEGIN_NAMESPACE_YM_SOP

//////////////////////////////////////////////////////////////////////
// クラス SopCover
//////////////////////////////////////////////////////////////////////

// @brief "quick factor" を行う．
Expr
SopCover::quick_factor() const
{
  GenFactor<OneLevel0Kernel, WeakDivision> factor;
  return factor(*this);
}

// @brief "good factor" を行う．
Expr
SopCover::good_factor() const
{
  GenFactor<BestKernel, WeakDivision> factor;
  return factor(*this);
}

// @brief "bool factor" を行う．
Expr
SopCover::bool_factor() const
{
  auto func = tvfunc();
  auto new_cover = SopCover(variable_num(), Tv2Sop::isop(func));
  GenFactor<BestKernel, BoolDivision> factor;
  return factor(new_cover);
}

END_NAMESPACE_YM_SOP
