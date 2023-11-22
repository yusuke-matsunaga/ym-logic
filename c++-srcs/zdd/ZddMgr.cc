
/// @file ZddMgr.cc
/// @brief ZddMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022, 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/ZddMgr.h"
#include "ym/Zdd.h"
#include "ZddMgrImpl.h"
#include "CopyOp.h"
#include "InvOp.h"


BEGIN_NAMESPACE_YM_ZDD

// @brief コンストラクタ
ZddMgr::ZddMgr(
) : mImpl{new ZddMgrImpl}
{
}

// @brief デストラクタ
ZddMgr::~ZddMgr()
{
}

// @brief ZDD をコピーする．
Zdd
ZddMgr::copy(
  const Zdd& src
)
{
  auto src_root = ZddEdge{src.mRoot};
  if ( src.mMgr == impl() ) {
    return Zdd{impl(), src_root};
  }
  else {
    CopyOp op{impl()};
    auto e = op.copy_step(src_root);
    return Zdd{impl(), e};
  }
}

// @brief ZDD を反転する．
Zdd
ZddMgr::invert(
  const Zdd& src
)
{
  InvOp op{impl()};
  auto e = op.inv_step(ZddEdge{src.mRoot});
  return Zdd{impl(), e};
}

// @brief 恒儀関数を作る．
Zdd
ZddMgr::zero()
{
  return Zdd{impl(), ZddEdge::zero()};
}

// @brief 恒新関数を作る．
Zdd
ZddMgr::one()
{
  return Zdd{impl(), ZddEdge::one()};
}

// @brief 新しいノードを作る．
Zdd
ZddMgr::new_node(
  SizeType index,
  const Zdd& edge0,
  const Zdd& edge1
)
{
  return Zdd{impl(), impl()->new_node(index,
				      ZddEdge{edge0.mRoot},
				      ZddEdge{edge1.mRoot})};
}

// @brief 部分集合を作る．
Zdd
ZddMgr::make_set(
  const vector<SizeType> elem_list
)
{
  auto e = mImpl->make_set(elem_list);
  return Zdd{impl(), e};
}

// @brief ノード数を返す．
SizeType
ZddMgr::node_num() const
{
  return mImpl->node_num();
}

// @brief GC を起動するしきい値を返す．
SizeType
ZddMgr::gc_limit() const
{
  return mImpl->gc_limit();
}

// @brief GC を起動するしきい値を設定する．
void
ZddMgr::set_gc_limit(
  SizeType limit
)
{
  mImpl->set_gc_limit(limit);
}

// @brief GC を許可する．
void
ZddMgr::enable_gc()
{
  mImpl->enable_gc();
}

// @brief GC を禁止する．
void
ZddMgr::disable_gc()
{
  mImpl->disable_gc();
}


//////////////////////////////////////////////////////////////////////
// クラス CopyOp
//////////////////////////////////////////////////////////////////////

// @brief コピーする．
ZddEdge
CopyOp::copy_step(
  ZddEdge edge
)
{
  if ( edge.is_zero() ) {
    return ZddEdge::zero();
  }
  if ( edge.is_one() ) {
    return ZddEdge::one();
  }

  auto node = edge.node();
  auto index = node->index();
  if ( mTable.count(node) > 0 ) {
    auto rnode = mTable.at(node);
    return ZddEdge{rnode};
  }
  auto edge0 = node->edge0();
  auto edge1 = node->edge1();
  auto redge0 = copy_step(edge0);
  auto redge1 = copy_step(edge1);
  auto redge = new_node(index, redge0, redge1);
  auto rnode = redge.node();
  mTable.emplace(node, rnode);
  return redge;
}


//////////////////////////////////////////////////////////////////////
// クラス InvOp
//////////////////////////////////////////////////////////////////////

// @brief コピーする．
ZddEdge
InvOp::inv_step(
  ZddEdge edge
)
{
  if ( edge.is_zero() ) {
    return ZddEdge::one();
  }
  if ( edge.is_one() ) {
    return ZddEdge::zero();
  }

  auto node = edge.node();
  auto index = node->index();
  if ( mTable.count(node) > 0 ) {
    auto rnode = mTable.at(node);
    return ZddEdge{rnode};
  }
  auto edge0 = node->edge0();
  auto edge1 = node->edge1();
  auto redge0 = inv_step(edge0);
  auto redge1 = inv_step(edge1);
  auto redge = new_node(index, redge0, redge1);
  auto rnode = redge.node();
  mTable.emplace(node, rnode);
  return redge;
}

END_NAMESPACE_YM_ZDD
