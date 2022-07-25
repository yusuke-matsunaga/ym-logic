
/// @file Bdd.cc
/// @brief Bdd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Bdd.h"
#include "ym/BddError.h"
#include "BddEdge.h"
#include "BddMgrImpl.h"
#include "BddNode.h"
#include "CopyOp.h"


BEGIN_NAMESPACE_YM_BDD

// @brief 空のコンストラクタ
Bdd::Bdd() : mMgr{nullptr}
{
}

// @brief 内容を指定したコンストラクタ
Bdd::Bdd(
  BddMgrImpl* mgr,
  BddEdge root
) : mMgr{mgr},
    mRoot{root.body()}
{
  if ( mMgr != nullptr ) {
    mMgr->activate(mRoot);
  }
}

// @brief コピーコンストラクタ
Bdd::Bdd(
  const Bdd& src
) : Bdd{src.mMgr, src.mRoot}
{
  // いわゆる delegate constructor
}

// @brief コピー代入演算子
Bdd&
Bdd::operator=(
  const Bdd& src
)
{
  if ( mMgr != src.mMgr || mRoot != src.mRoot ) {
    if ( mMgr != nullptr ) {
      mMgr->deactivate(mRoot);
    }
    mMgr = src.mMgr;
    mRoot = src.mRoot;
    if ( mMgr != nullptr ) {
      mMgr->activate(mRoot);
    }
  }
  return *this;
}

// @brief デストラクタ
Bdd::~Bdd()
{
  if ( mMgr != nullptr ) {
    mMgr->deactivate(mRoot);
    mMgr->garbage_collection();
  }
}

// @brief マネージャが異なる場合コピーする．
BddEdge
Bdd::copy_edge(
  const Bdd& src
) const
{
  if ( src.mMgr == mMgr ) {
    return src.mRoot;
  }
  else {
    CopyOp op{mMgr};
    return op.copy_step(src.mRoot);
  }
}

// @brief 否定した関数を返す．
Bdd
Bdd::invert() const
{
  return Bdd{mMgr, ~BddEdge{mRoot}};
}

// @brief 極性をかけ合わせる．
Bdd
Bdd::operator*(
  bool inv
) const
{
  return Bdd{mMgr, BddEdge{mRoot} * inv};
}

// @brief 自分自身を否定する．
Bdd&
Bdd::invert_int()
{
  if ( mMgr != nullptr ) {
    // 実はこれは BddMgr を介さない．
    // ただ不正値の時には演算を行わない．
    mRoot = ~mRoot;
  }
  return *this;
}

// @brief 極性をかけ合わせて代入する．
Bdd&
Bdd::operator*=(
  bool inv
)
{
  if ( mMgr != nullptr ) {
    // 実はこれは BddMgr を介さない．
    // ただ不正値の時には演算を行わない．
    mRoot *= inv;
  }
  return *this;
}

// @brief 定数0の時 true を返す．
bool
Bdd::is_zero() const
{
  return mMgr != nullptr && BddEdge{mRoot}.is_zero();
}

// @brief 定数1の時 true を返す．
bool
Bdd::is_one() const
{
  return mMgr != nullptr && BddEdge{mRoot}.is_one();
}

// @brief 根の変数とコファクターを求める．
VarId
Bdd::root_decomp(
  Bdd& f0,
  Bdd& f1
) const
{
  ASSERT_COND( mMgr != nullptr );
  BddEdge root{mRoot};
  auto node = root.node();
  if ( node == nullptr ) {
    f0 = *this;
    f1 = *this;
    return VarId::illegal();
  }
  else {
    auto oinv = root.inv();
    f0 = Bdd{mMgr, node->edge0() * oinv};
    f1 = Bdd{mMgr, node->edge1() * oinv};
    return VarId{node->index()};
  }
}

// @brief 根の変数を得る．
VarId
Bdd::root_var() const
{
  ASSERT_COND( mMgr != nullptr );
  BddEdge root{mRoot};
  auto node = root.node();
  if ( node == nullptr ) {
    return VarId::illegal();
  }
  else {
    return VarId{node->index()};
  }
}

// @brief 負のコファクターを返す．
Bdd
Bdd::root_cofactor0() const
{
  ASSERT_COND( mMgr != nullptr );
  BddEdge root{mRoot};
  auto node = root.node();
  if ( node == nullptr ) {
    return *this;
  }
  else {
    auto oinv = root.inv();
    return Bdd{mMgr, node->edge0() * oinv};
  }
}

// @brief 正のコファクターを返す．
Bdd
Bdd::root_cofactor1() const
{
  ASSERT_COND( mMgr != nullptr );
  BddEdge root{mRoot};
  auto node = root.node();
  if ( node == nullptr ) {
    return *this;
  }
  else {
    auto oinv = root.inv();
    return Bdd{mMgr, node->edge1() * oinv};
  }
}

// @brief 根が否定されている時 true を返す．
bool
Bdd::root_inv() const
{
  ASSERT_COND ( mMgr != nullptr );
  return static_cast<bool>(mRoot & 1UL);
}

// @brief 評価を行う．
bool
Bdd::eval(
  const vector<bool>& inputs ///< [in] 入力値ベクタ
) const
{
  ASSERT_COND ( mMgr != nullptr );
  BddEdge edge{mRoot};
  for ( ; ; ) {
    if ( edge.is_zero() ) {
      return false;
    }
    if ( edge.is_one() ) {
      return true;
    }
    auto node = edge.node();
    auto inv = edge.inv();
    auto index = node->index();
    if ( inputs[index] ) {
      edge = node->edge1();
    }
    else {
      edge = node->edge0();
    }
    edge *= inv;
  }
}

// @brief 等価比較演算
bool
Bdd::operator==(
  const Bdd& right ///< [in] オペランド
) const
{
  return mMgr == right.mMgr && mRoot == right.mRoot;
}

// @brief ハッシュ値を返す．
SizeType
Bdd::hash() const
{
  ASSERT_COND( mMgr != nullptr );
  BddEdge root{mRoot};
  return root.hash();
}

// @brief 根の枝を変更する．
void
Bdd::change_root(
  BddEdge new_root
)
{
  if ( mRoot != new_root ) {
    mMgr->deactivate(mRoot);
    mMgr->activate(new_root);
    mRoot = new_root.body();
  }
}

// @brief BDDの根の枝のリストを作る．
BddMgrImpl*
Bdd::root_list(
  const vector<Bdd>& bdd_list,
  vector<BddEdge>& edge_list
)
{
  ASSERT_COND ( !bdd_list.empty() );

  auto mgr = bdd_list[0].mMgr;
  ASSERT_COND( mgr != nullptr );
  SizeType n = bdd_list.size();
  edge_list.clear();
  edge_list.reserve(n);
  for ( auto& bdd: bdd_list ) {
    ASSERT_COND( bdd.mMgr == mgr );
    edge_list.push_back(bdd.mRoot);
  }
  return mgr;
}

// @brief If-Then-Else 演算
Bdd
ite(
  const Bdd& cond,
  const Bdd& then_f,
  const Bdd& else_f
)
{
  auto mgr = cond.mMgr;
  ASSERT_COND( mgr != nullptr );
  ASSERT_COND( mgr == then_f.mMgr );
  ASSERT_COND( mgr == else_f.mMgr );
  auto e = mgr->ite_op(cond.mRoot, then_f.mRoot, else_f.mRoot);
  return Bdd{mgr, e};
}

END_NAMESPACE_YM_BDD
