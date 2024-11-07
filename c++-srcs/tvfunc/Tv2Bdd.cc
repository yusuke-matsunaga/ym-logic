
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
  vector<BddVar> var_list(ni);
  for ( SizeType i = 0; i < ni; ++ i ) {
    var_list[i] = mgr.variable(ni - i - 1);
  }
  return bdd(mgr, var_list);
}

// @brief BDD に変換する．
Bdd
TvFunc::bdd(
  BddMgr& mgr,
  const vector<BddVar>& var_list
) const
{
  Tv2Bdd tv2bdd{mgr, *this, var_list};
  auto r = tv2bdd.run();
  return r;
}


//////////////////////////////////////////////////////////////////////
// クラス Tv2Bdd
//////////////////////////////////////////////////////////////////////

// @brief BDD を作る．
Bdd
Tv2Bdd::run()
{
  SizeType ni = mFunc.input_num();
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
  auto v = mVarList[var];
  auto r = ite(v, r1, r0);
  return r;
}

END_NAMESPACE_YM
