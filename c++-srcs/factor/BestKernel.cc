
/// @file BestKernel.cc
/// @brief BestKernel の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "BestKernel.h"
#include "KernelGen.h"


BEGIN_NAMESPACE_YM_FACTOR

//////////////////////////////////////////////////////////////////////
// クラス BestKernel
//////////////////////////////////////////////////////////////////////

// @brief 除数を求める．
SopCover
BestKernel::divisor(
  const SopCover& f
)
{
  KernelGen kernel_gen;
  auto kernel = kernel_gen.best_kernel(f);
  return kernel;
}

END_NAMESPACE_YM_FACTOR
