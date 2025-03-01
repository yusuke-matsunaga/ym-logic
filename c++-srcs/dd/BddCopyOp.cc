
/// @file BddCopyOp.cc
/// @brief BddCopyOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/BddMgr.h"
#include "BddCopyOp.h"
#include "BddMgrImpl.h"


BEGIN_NAMESPACE_YM_DD

//////////////////////////////////////////////////////////////////////
// クラス BddMgrHolder
//////////////////////////////////////////////////////////////////////

// @brief BDD をコピーする．
Bdd
BddMgr::copy(
  const Bdd& src
)
{
  if ( src.is_invalid() ) {
    // 不正な ZDD はそのまま
    return src;
  }
  if ( has_same_mgr(src) ) {
    // 自分自身に属している場合もそのまま
    return src;
  }
  // src のサポート変数を取り出す．
  auto var_list = src.get_support_list();
  // こちら側にも同じ分の変数を確保する．
  for ( auto& var: var_list ) {
    auto vid = var.id();
    (void) variable(vid);
  }
  // var_list のレベルを求める．
  auto level_list = BddVar::conv_to_levellist(var_list);
  std::sort(level_list.begin(), level_list.end());
  BddCopyOp op(get(), var_list, level_list);
  auto edge = op.copy_step(src, 0);
  return _bdd(edge);
}


//////////////////////////////////////////////////////////////////////
// クラス BddCopyOp
//////////////////////////////////////////////////////////////////////

// @brief コピーする．
DdEdge
BddCopyOp::copy_step(
  const Bdd& bdd,
  SizeType pos
)
{
  if ( bdd.is_zero() ) {
    return DdEdge::zero();
  }
  if ( bdd.is_one() ) {
    return DdEdge::one();
  }

  if ( mTable.count(bdd) > 0 ) {
    return mTable.at(bdd);
  }

  auto var = mVarList[pos];
  auto bdd0 = bdd / ~var;
  auto bdd1 = bdd /  var;
  auto r0 = copy_step(bdd0, pos + 1);
  auto r1 = copy_step(bdd1, pos + 1);
  auto level = mLevelList[pos];
  auto result = mMgr->new_node(level, r0, r1);
  mTable.emplace(bdd, result);
  return result;
}

END_NAMESPACE_YM_DD
