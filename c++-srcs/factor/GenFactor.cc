
/// @file SopCover_GenFactor.cc
/// @brief SopCover の GenFactor 関係の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/SopCover.h"
#include "GenFactor.h"
#include "OneLevel0Kernel.h"
#include "BestKernel.h"
#include "WeakDivision.h"


BEGIN_NAMESPACE_YM_SOP

//////////////////////////////////////////////////////////////////////
// クラス SopCover
//////////////////////////////////////////////////////////////////////

// @brief "quick factor" を行う．
Expr
SopCover::quick_factor() const
{
  using namespace nsFactor;
  GenFactor<OneLevel0Kernel, WeakDivision> qf;
  return qf(*this);
}

// @brief "good factor" を行う．
Expr
SopCover::good_factor() const
{
  using namespace nsFactor;
  GenFactor<BestKernel, WeakDivision> gf;
  return gf(*this);
}

END_NAMESPACE_YM_SOP
