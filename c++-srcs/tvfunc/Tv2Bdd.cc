
/// @file Tv2Bdd.cc
/// @brief Tv2Bdd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Tv2Bdd.h"
#include "BddGen.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス Tv2Bdd
//////////////////////////////////////////////////////////////////////

// @brief TvFunc を BDD に変換する．
Bdd
Tv2Bdd::gen_bdd(
  const TvFunc& f,
  BddMgr& mgr
)
{
  auto ni = f.input_num();
  vector<BddVar> var_list(ni);
  for ( SizeType i = 0; i < ni; ++ i ) {
    var_list[i] = mgr.variable(ni - i - 1);
  }
  return gen_bdd(f, mgr, var_list);
}

// @brief TvFunc を BDD に変換する．
Bdd
Tv2Bdd::gen_bdd(
  const TvFunc& f,
  BddMgr& mgr,
  const vector<BddVar>& var_list
)
{
  BddGen gen{f, mgr, var_list};
  auto r = gen.run();
  return r;
}


//////////////////////////////////////////////////////////////////////
// クラス BddGen
//////////////////////////////////////////////////////////////////////

// @brief BDD を作る．
Bdd
BddGen::run()
{
  SizeType ni = mFunc.input_num();
  SizeType nexp = 1UL << ni;
  return decomp_step(0, 0, nexp);
}

// @brief run() の下請け関数
Bdd
BddGen::decomp_step(
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
  auto r = Bdd::ite(v, r1, r0);
  return r;
}

END_NAMESPACE_YM
