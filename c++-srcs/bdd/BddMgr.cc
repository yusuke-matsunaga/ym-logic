
/// @file BddMgr.cc
/// @brief BddMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022, 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/BddMgr.h"
#include "ym/Bdd.h"
#include "BddMgrImpl.h"
#include "CopyOp.h"
#include "TruthOp.h"


BEGIN_NAMESPACE_YM_BDD

// @brief コンストラクタ
BddMgr::BddMgr(
) : mImpl{new BddMgrImpl}
{
}

// @brief デストラクタ
BddMgr::~BddMgr()
{
}

// @brief BDD をコピーする．
Bdd
BddMgr::copy(
  const Bdd& src
)
{
  if ( src.mMgr == impl() ) {
    return Bdd{impl(), src.mRoot};
  }
  else {
    CopyOp op{impl()};
    auto e = op.copy_step(src.mRoot);
    return Bdd{impl(), e};
  }
}

// @brief 恒儀関数を作る．
Bdd
BddMgr::zero()
{
  return Bdd{impl(), BddEdge::zero()};
}

// @brief 恒新関数を作る．
Bdd
BddMgr::one()
{
  return Bdd{impl(), BddEdge::one()};
}

// @brief リテラル関数を作る．
Bdd
BddMgr::literal(
  SizeType var,
  bool inv
)
{
  auto e = mImpl->make_literal(var) ^ inv;
  return Bdd{impl(), e};
}

// @brief 肯定のリテラル関数を作る．
Bdd
BddMgr::posi_literal(
  SizeType var
)
{
  return literal(var, false);
}

// @brief 否定のリテラル関数を作る．
Bdd
BddMgr::nega_literal(
  SizeType var
)
{
  return literal(var, true);
}

// @brief 真理値表形式の文字列からBDDを作る．
Bdd
BddMgr::from_truth(
  const string& str
)
{
  // str が適切な文字列か確認する．
  for ( char c: str ) {
    if ( c != '0' && c != '1' ) {
      throw std::invalid_argument("only '0' or '1' are expected");
    }
  }
  for ( SizeType n = str.size(); n > 1; n >>= 1 ) {
    if ( (n % 2) != 0 ) {
      throw std::invalid_argument("the length is expected to be the power of 2");
    }
  }

  TruthOp op{impl()};
  auto e = op.op_step(str, 0);
  return Bdd{impl(), e};
}

// @brief ノード数を返す．
SizeType
BddMgr::node_num() const
{
  return mImpl->node_num();
}

// @brief GC を起動するしきい値を返す．
SizeType
BddMgr::gc_limit() const
{
  return mImpl->gc_limit();
}

// @brief GC を起動するしきい値を設定する．
void
BddMgr::set_gc_limit(
  SizeType limit
)
{
  mImpl->set_gc_limit(limit);
}

// @brief GC を許可する．
void
BddMgr::enable_gc()
{
  mImpl->enable_gc();
}

// @brief GC を禁止する．
void
BddMgr::disable_gc()
{
  mImpl->disable_gc();
}


//////////////////////////////////////////////////////////////////////
// クラス TruthOp
//////////////////////////////////////////////////////////////////////

// @brief 真理値表の文字列からBDDを作る．
BddEdge
TruthOp::op_step(
  const string& str,
  SizeType index
)
{
  if ( str == "0" ) {
    return BddEdge::zero();
  }
  if ( str == "1" ) {
    return BddEdge::one();
  }
  if ( mTable.count(str) > 0 ) {
    // 同じ文字列の結果が記録されていたらそれを用いる．
    return mTable.at(str);
  }

  SizeType n = str.size();
  SizeType h = n / 2;
  string str0 = str.substr(h);
  string str1 = str.substr(0, h);
  auto e0 = op_step(str0, index + 1);
  auto e1 = op_step(str1, index + 1);
  auto ans = new_node(index, e0, e1);
  mTable.emplace(str, ans);
  return ans;
}


//////////////////////////////////////////////////////////////////////
// クラス CopyOp
//////////////////////////////////////////////////////////////////////

// @brief コピーする．
BddEdge
CopyOp::copy_step(
  BddEdge edge
)
{
  if ( edge.is_zero() ) {
    return BddEdge::zero();
  }
  if ( edge.is_one() ) {
    return BddEdge::one();
  }

  auto node = edge.node();
  auto inv = edge.inv();
  auto index = node->index();
  if ( mTable.count(node) > 0 ) {
    auto rnode = mTable.at(node);
    return BddEdge{rnode, inv};
  }
  auto edge0 = node->edge0() ^ inv;
  auto edge1 = node->edge1() ^ inv;
  auto redge0 = copy_step(edge0);
  auto redge1 = copy_step(edge1);
  auto redge = new_node(index, redge0, redge1);
  auto rnode = redge.node();
  ASSERT_COND( redge.inv() == inv );
  mTable.emplace(node, rnode);
  return redge;
}

END_NAMESPACE_YM_BDD
