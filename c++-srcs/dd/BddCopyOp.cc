
/// @file BddCopyOp.cc
/// @brief BddCopyOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "dd/BddMgrImpl.h"
#include "BddCopyOp.h"


BEGIN_NAMESPACE_YM_DD


//////////////////////////////////////////////////////////////////////
// クラス BddMgrImpl
//////////////////////////////////////////////////////////////////////

// @brief BDD をコピーする．
Bdd
BddMgrImpl::copy(
  const Bdd& src
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
  auto var_list = src.get_support_list();
  for ( auto& var: var_list ) {
    auto vid = var.id();
    (void) variable(vid);
  }
  // var_list を変数順にしたがって並べ替える．
  sort(var_list.begin(), var_list.end(),
       [&](const BddVar& a, const BddVar& b){
	 auto vid_a = a.id();
	 auto vid_b = b.id();
	 auto index_a = varid_to_index(vid_a);
	 auto index_b = varid_to_index(vid_b);
	 return index_a < index_b;
       });
  // それを自身のインデックスに変換する．
  vector<SizeType> index_list;
  index_list.reserve(var_list.size());
  for ( auto& var: var_list ) {
    auto vid = var.id();
    auto index = varid_to_index(vid);
    index_list.push_back(index);
  }
  BddCopyOp op{*this, var_list, index_list};
  auto e = op.copy_step(src, 0);
  return _bdd(e);
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
  if ( bdd.is_const() ) {
    return mMgr._edge(bdd);
  }

  if ( mTable.count(bdd) > 0 ) {
    return mTable.at(bdd);
  }

  auto var = mVarList[pos];
  auto bdd0 = bdd / ~var;
  auto bdd1 = bdd /  var;
  auto r0 = copy_step(bdd0, pos + 1);
  auto r1 = copy_step(bdd1, pos + 1);
  auto index = mIndexList[pos];
  auto result = mMgr.new_node(index, r0, r1);
  mTable.emplace(bdd, result);
  return result;
}

END_NAMESPACE_YM_DD
