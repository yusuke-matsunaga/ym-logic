
/// @file BestKernel.cc
/// @brief BestKernel の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.


#include "BestKernel.h"
#include "ym/AlgCover.h"
#include "ym/AlgCube.h"
#include "AlgKernelGen.h"


BEGIN_NAMESPACE_YM_LOGIC

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
// @param[in] f 対象の論理式
// @return 除数を表す論理式を返す．
AlgCover
BestKernel::operator()(const AlgCover& f) const
{
  AlgKernelGen kernel_gen;

  return kernel_gen.best_kernel(f);
}

END_NAMESPACE_YM_LOGIC
