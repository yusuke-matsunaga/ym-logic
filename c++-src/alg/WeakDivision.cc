
/// @file WeakDivision.cc
/// @brief WeakDivision の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.


#include "WeakDivision.h"
#include "ym/AlgCover.h"


BEGIN_NAMESPACE_YM_LOGIC

//////////////////////////////////////////////////////////////////////
// クラス WeakDivision
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
WeakDivision::WeakDivision()
{
}

// @brief デストラクタ
WeakDivision::~WeakDivision()
{
}

// @brief 除算を行う．
// @param[in] f 被除数
// @param[in] d 除数
// @return 商q と余りr の pair を返す．
pair<AlgCover, AlgCover>
WeakDivision::operator()(const AlgCover& f,
			 const AlgCover& d) const
{
  AlgCover q = f / d;
  AlgCover r = f - (q * d);
  return make_pair(std::move(q), std::move(r));
}

END_NAMESPACE_YM_LOGIC
