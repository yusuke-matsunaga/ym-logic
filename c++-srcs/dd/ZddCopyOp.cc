
/// @file ZddCopyOp.cc
/// @brief ZddCopyOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/ZddMgr.h"
#include "ZddMgrImpl.h"
#include "ZddCopyOp.h"


BEGIN_NAMESPACE_YM_DD

//////////////////////////////////////////////////////////////////////
// クラス ZddMgrImpl
//////////////////////////////////////////////////////////////////////

// @brief ZDD をコピーする．
Zdd
ZddMgr::copy(
  const Zdd& src
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
  auto item_list = src.get_support_list();
  // こちら側にも同じ分の変数を確保する．
  for ( auto& var: item_list ) {
    auto vid = var.id();
    (void) item(vid);
  }
  // item_list をレベルのリストに変換する．
  auto level_list = ZddItem::conv_to_levellist(item_list);
  // level_list を変数順にしたがって並べ替える．
  std::sort(level_list.begin(), level_list.end());
  ZddCopyOp op(get(), item_list, level_list);
  auto edge = op.copy_step(src, 0);
  return _zdd(edge);
}


//////////////////////////////////////////////////////////////////////
// クラス ZddCopyOp
//////////////////////////////////////////////////////////////////////

// @brief コピーする．
DdEdge
ZddCopyOp::copy_step(
  const Zdd& zdd,
  SizeType pos
)
{
  if ( zdd.is_zero() ) {
    return DdEdge::zero();
  }
  if ( zdd.is_one() ) {
    return DdEdge::one();
  }

  if ( mTable.count(zdd) > 0 ) {
    return mTable.at(zdd);
  }

  auto item = mItemList[pos];
  auto zdd0 = zdd.offset(item);
  auto zdd1 = zdd.onset(item);
  auto r0 = copy_step(zdd0, pos + 1);
  auto r1 = copy_step(zdd1, pos + 1);
  auto level = mLevelList[pos];
  auto result = mMgr->new_node(level, r0, r1);
  mTable.emplace(zdd, result);
  return result;
}

END_NAMESPACE_YM_DD
