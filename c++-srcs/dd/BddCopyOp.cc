
/// @file BddCopyOp.cc
/// @brief BddCopyOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/BddMgrPtr.h"
#include "BddCopyOp.h"
#include "BddMgrImpl.h"


BEGIN_NAMESPACE_YM_DD

//////////////////////////////////////////////////////////////////////
// クラス BddMgrPtr
//////////////////////////////////////////////////////////////////////

// @brief BDD をコピーする．
Bdd
BddMgrPtr::copy(
  const Bdd& src
) const
{
  if ( src.is_invalid() ) {
    // 不正な ZDD はそのまま
    return src;
  }
  if ( src.mMgr == *this ) {
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
  // var_list を変数順にしたがって並べ替える．
  sort(var_list.begin(), var_list.end(),
       [&](const BddVar& a, const BddVar& b) {
	 auto level_a = a.level();
	 auto level_b = b.level();
	 return level_a < level_b;
       });
  // それを自身のレベルに変換する．
  vector<SizeType> level_list;
  level_list.reserve(var_list.size());
  for ( auto& var: var_list ) {
    auto vid = var.id();
    auto level = mPtr->varid_to_level(vid);
    level_list.push_back(level);
  }
  BddCopyOp op{get(), var_list, level_list};
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
