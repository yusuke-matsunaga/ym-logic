
/// @file Zdd.cc
/// @brief Zdd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Zdd.h"
#include "ZddEdge.h"
#include "ZddMgrImpl.h"
#include "ZddNode.h"
#include "CopyOp.h"
#include "InvOp.h"
#include "RootHolder.h"


BEGIN_NAMESPACE_YM_ZDD

// @brief 空のコンストラクタ
Zdd::Zdd(
) : mMgr{nullptr},
    mRoot{0}
{
  // 不正値となる．
}

// @brief 内容を指定したコンストラクタ
Zdd::Zdd(
  ZddMgrImpl* mgr,
  ZddEdge root
) : mMgr{mgr},
    mRoot{root.body()}
{
  if ( mMgr != nullptr ) {
    mMgr->activate(ZddEdge{mRoot});
  }
}

// @brief コピーコンストラクタ
Zdd::Zdd(
  const Zdd& src
) : Zdd{src.mMgr, ZddEdge{src.mRoot}}
{
  // いわゆる delegate constructor
}

// @brief コピー代入演算子
Zdd&
Zdd::operator=(
  const Zdd& src
)
{
  // この順序なら自分自身に代入しても正しく動作する．
  if ( src.mMgr != nullptr ) {
    src.mMgr->activate(ZddEdge{src.mRoot});
  }
  if ( mMgr != nullptr ) {
    mMgr->deactivate(ZddEdge{mRoot});
  }
  mMgr = src.mMgr;
  mRoot = src.mRoot;
  return *this;
}

// @brief デストラクタ
Zdd::~Zdd()
{
  if ( mMgr != nullptr ) {
    mMgr->deactivate(ZddEdge{mRoot});
  }
}

// @brief マネージャが異なる場合コピーする．
ZddEdge
Zdd::copy_edge(
  const Zdd& src
) const
{
  if ( src.mMgr == mMgr ) {
    return ZddEdge{src.mRoot};
  }
  else {
    CopyOp op{mMgr};
    return op.copy_step(ZddEdge{src.mRoot});
  }
}

// @brief 否定した関数を返す．
Zdd
Zdd::invert() const
{
  InvOp op{mMgr};
  auto e = op.inv_step(ZddEdge{mRoot});
  return Zdd{mMgr, e};
}

// @brief 自分自身を否定する．
Zdd&
Zdd::invert_int()
{
  if ( mMgr != nullptr ) {
    // 不正値の時には演算を行わない．
    InvOp op{mMgr};
    change_root(op.inv_step(ZddEdge{mRoot}));
  }
  return *this;
}

// @brief 定数0の時 true を返す．
bool
Zdd::is_zero() const
{
  return is_valid() && ZddEdge{mRoot}.is_zero();
}

// @brief 定数1の時 true を返す．
bool
Zdd::is_one() const
{
  return is_valid() && ZddEdge{mRoot}.is_one();
}

// @brief 定数の時 true を返す．
bool
Zdd::is_const() const
{
  return is_valid() && ZddEdge{mRoot}.is_const();
}

// @brief 根の変数とコファクターを求める．
SizeType
Zdd::root_decomp(
  Zdd& f0,
  Zdd& f1
) const
{
  _check_valid();

  auto root = ZddEdge{mRoot};
  auto node = root.node();
  if ( node == nullptr ) {
    f0 = *this;
    f1 = *this;
    return BAD_VARID;
  }
  else {
    f0 = Zdd{mMgr, node->edge0()};
    f1 = Zdd{mMgr, node->edge1()};
    return node->index();
  }
}

// @brief 根の変数を得る．
SizeType
Zdd::root_var() const
{
  _check_valid();

  auto root = ZddEdge{mRoot};
  auto node = root.node();
  if ( node == nullptr ) {
    return BAD_VARID;
  }
  else {
    return node->index();
  }
}

// @brief 負のコファクターを返す．
Zdd
Zdd::root_cofactor0() const
{
  _check_valid();

  auto root = ZddEdge{mRoot};
  auto node = root.node();
  if ( node == nullptr ) {
    return *this;
  }
  else {
    return Zdd{mMgr, node->edge0()};
  }
}

// @brief 正のコファクターを返す．
Zdd
Zdd::root_cofactor1() const
{
  _check_valid();

  auto root = ZddEdge{mRoot};
  auto node = root.node();
  if ( node == nullptr ) {
    return *this;
  }
  else {
    return Zdd{mMgr, node->edge1()};
  }
}

#if 0
// @brief 評価を行う．
bool
Zdd::eval(
  const vector<bool>& inputs ///< [in] 入力値ベクタ
) const
{
  _check_valid();

  auto edge = ZddEdge{mRoot};
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
    edge ^= inv;
  }
}
#endif

// @brief 等価比較演算
bool
Zdd::operator==(
  const Zdd& right ///< [in] オペランド
) const
{
  return mMgr == right.mMgr && mRoot == right.mRoot;
}

// @brief ハッシュ値を返す．
SizeType
Zdd::hash() const
{
  _check_valid();

  auto root = ZddEdge{mRoot};
  return root.hash();
}

// @brief 根の枝を変更する．
void
Zdd::change_root(
  ZddEdge new_root
)
{
  _check_valid();

  // この順序なら new_root と mRoot が等しくても
  // 正しく動く
  mMgr->activate(new_root);
  mMgr->deactivate(ZddEdge{mRoot});
  mRoot = new_root.body();
}

// @brief ZDDの根の枝のリストを作る．
ZddMgrImpl*
Zdd::root_list(
  const vector<Zdd>& bdd_list,
  vector<ZddEdge>& edge_list
)
{
  if ( bdd_list.empty() ) {
    throw std::invalid_argument("empty list");
  }

  SizeType n = bdd_list.size();
  edge_list.clear();
  edge_list.reserve(n);
  for ( auto& bdd: bdd_list ) {
    bdd._check_valid();
    edge_list.push_back(ZddEdge{bdd.mRoot});
  }
  auto mgr = bdd_list[0].mMgr;
  return mgr;
}


//////////////////////////////////////////////////////////////////////
// RootHolder
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
RootHolder::RootHolder(
  ZddMgrImpl* mgr,
  ZddEdge edge
) : mMgr{mgr},
    mEdge{edge}
{
  mMgr->activate(edge);
}

// @brief デストラクタ
RootHolder::~RootHolder()
{
  mMgr->deactivate(mEdge);
}

END_NAMESPACE_YM_ZDD
