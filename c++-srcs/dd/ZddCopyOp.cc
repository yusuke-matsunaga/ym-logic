
/// @file ZddCopyOp.cc
/// @brief ZddCopyOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "dd/ZddMgrImpl.h"
#include "ZddCopyOp.h"


BEGIN_NAMESPACE_YM_DD


//////////////////////////////////////////////////////////////////////
// クラス ZddMgrImpl
//////////////////////////////////////////////////////////////////////

// @brief BDD をコピーする．
Zdd
ZddMgrImpl::copy(
  const Zdd& src
)
{
  if ( src.mMgr == nullptr ) {
    // 不正な ZDD はそのまま
    return src;
  }
  if ( src.mMgr == this ) {
    // 自分自身に属している場合もそのまま
    return src;
  }
  // src のサポート変数を取り出す．
  auto item_list = src.get_support_list();
  // item_list を変数順にしたがって並べ替える．
  sort(item_list.begin(), item_list.end(),
       [&](const ZddItem& a, const ZddItem& b){
	 auto vid_a = a.id();
	 auto vid_b = b.id();
	 auto index_a = varid_to_index(vid_a);
	 auto index_b = varid_to_index(vid_b);
	 return index_a < index_b;
       });
  // それを自身のインデックスに変換する．
  vector<SizeType> index_list;
  index_list.reserve(item_list.size());
  for ( auto& item: item_list ) {
    auto vid = item.id();
    auto index = varid_to_index(vid);
    index_list.push_back(index);
  }
  ZddCopyOp op{*this, item_list, index_list};
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
  if ( zdd.is_const() ) {
    return mMgr._edge(zdd);
  }

  if ( mTable.count(zdd) > 0 ) {
    return mTable.at(zdd);
  }

  auto item = mItemList[pos];
  auto zdd0 = zdd.offset(item);
  auto zdd1 = zdd.onset(item);
  auto r0 = copy_step(zdd0, pos + 1);
  auto r1 = copy_step(zdd1, pos + 1);
  auto index = mIndexList[pos];
  auto result = mMgr.new_node(index, r0, r1);
  mTable.emplace(zdd, result);
  return result;
}

END_NAMESPACE_YM_DD