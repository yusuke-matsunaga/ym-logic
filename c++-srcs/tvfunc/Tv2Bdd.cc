
/// @file Tv2Bdd.cc
/// @brief Tv2Bdd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/TvFunc.h"
#include "Tv2Bdd.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス TvFunc
//////////////////////////////////////////////////////////////////////

// @brief BDD に変換する．
Bdd
TvFunc::bdd(
  BddMgr& mgr
) const
{
  auto ni = input_num();
  vector<SizeType> order(ni);
  for ( SizeType i = 0; i < ni; ++ i ) {
    order[i] = i;
  }
  return bdd(mgr, order);
}

// @brief BDD に変換する．
Bdd
TvFunc::bdd(
  BddMgr& mgr,
  const vector<SizeType>& order
) const
{
  Tv2Bdd tv2bdd{mgr, *this};
  auto r = tv2bdd.run(order);
  return r;
}


//////////////////////////////////////////////////////////////////////
// クラス Tv2Bdd
//////////////////////////////////////////////////////////////////////

// @brief BDD を作る．
Bdd
Tv2Bdd::run(
  const vector<SizeType>& order
)
{
  SizeType ni = mFunc.input_num();
  // TvFunc の内部構造の都合上逆順になる．
  for ( SizeType i = 0; i < ni; ++ i ) {
    mOrder[i] = order[ni - i - 1];
  }
  SizeType nexp = 1UL << ni;
  return decomp_step(0, 0, nexp);
}

// @brief run() の下請け関数
Bdd
Tv2Bdd::decomp_step(
  SizeType var,
  SizeType start,
  SizeType end
)
{
  SizeType size = end - start;
  if ( size == 1 ) {
    if ( mFunc.value(start) ) {
      return mMgr.one();
    }
    else {
      return mMgr.zero();
    }
  }

  SizeType mid = (start + end) / 2;
  auto r0 = decomp_step(var + 1, start, mid);
  auto r1 = decomp_step(var + 1, mid, end);
  auto c = mMgr.literal(mOrder[var]);
  auto r = ite(c, r1, r0);
  return r;
}

END_NAMESPACE_YM
