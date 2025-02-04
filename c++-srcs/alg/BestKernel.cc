
/// @file BestKernel.cc
/// @brief BestKernel の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "BestKernel.h"
#include "ym/AlgCover.h"
#include "ym/AlgCube.h"
#include "KernelGen.h"


BEGIN_NAMESPACE_YM_ALG

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
AlgCover
BestKernel::operator()(
  const AlgCover& f
) const
{
  KernelGen kernel_gen;

  return kernel_gen.best_kernel(f);
}

END_NAMESPACE_YM_ALG
