
/// @file BestKernel.cc
/// @brief BestKernel の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "BestKernel.h"
#include "ym/SopCover.h"
#include "ym/SopCube.h"
#include "KernelGen.h"


BEGIN_NAMESPACE_YM_SOP

//////////////////////////////////////////////////////////////////////
// クラス BestKernel
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
BestKernel::BestKernel()
{
}

// @brief デストラクタ
BestKernel::~BestKernel()
{
}

// @brief 除数を求める．
SopCover
BestKernel::operator()(
  const SopCover& f
) const
{
  KernelGen kernel_gen;

  return kernel_gen.best_kernel(f);
}

END_NAMESPACE_YM_SOP
