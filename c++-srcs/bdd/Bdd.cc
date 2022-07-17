
/// @file Bdd.cc
/// @brief Bdd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Bdd.h"
#include "ym/BddVar.h"
#include "BddEdge.h"
#include "BddMgrImpl.h"
#include "BddNode.h"
#include "CofactorOp.h"
#include "CheckSupOp.h"
#include "CheckSymOp.h"
#include "NodeCounter.h"
#include "NodeDisp.h"
#include "DotGen.h"
#include "IteOp.h"


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
}

// @brief コピーコンストラクタ
Bdd::Bdd(
  const Bdd& src
) : mMgr{src.mMgr},
    mRoot{src.mRoot}
{
  if ( mMgr != nullptr ) {
    mMgr->inc_ref(mRoot);
  }
}

// @brief 代入演算子
Bdd&
Bdd::operator=(
  const Bdd& src
)
{
  if ( mMgr != nullptr ) {
    mMgr->dec_ref(mRoot);
  }
  mMgr = src.mMgr;
  mRoot = src.mRoot;
  if ( mMgr != nullptr ) {
    mMgr->inc_ref(mRoot);
  }
  return *this;
}

// @brief デストラクタ
Bdd::~Bdd()
{
  if ( mMgr != nullptr ) {
    mMgr->dec_ref(mRoot);
  }
}

// @brief 否定した関数を返す．
Bdd
Bdd::invert() const
{
  return Bdd{mMgr, ~BddEdge{mRoot}};
}

// @brief 論理積を返す．
Bdd
Bdd::and_op(
  const Bdd& right
) const
{
  ASSERT_COND( mMgr != nullptr );
  ASSERT_COND( mMgr == right.mMgr );
  IteOp op{mMgr};
  auto e = op.and_step(mRoot, right.mRoot);
  return Bdd{mMgr, e};
}

// @brief 論理和を返す．
Bdd
Bdd::or_op(
  const Bdd& right
) const
{
  ASSERT_COND( mMgr != nullptr );
  ASSERT_COND( mMgr == right.mMgr );
  IteOp op{mMgr};
  auto e = op.or_step(mRoot, right.mRoot);
  return Bdd{mMgr, e};
}

// @brief 排他的論理和を返す．
Bdd
Bdd::xor_op(
  const Bdd& right
) const
{
  ASSERT_COND( mMgr != nullptr );
  ASSERT_COND( mMgr == right.mMgr );
  IteOp op{mMgr};
  auto e = op.xor_step(mRoot, right.mRoot);
  return Bdd{mMgr, e};
}

// @brief コファクターを計算する．
Bdd
Bdd::cofactor(
  BddVar var,
  bool inv
) const
{
  ASSERT_COND( mMgr != nullptr );
  CofactorOp op{mMgr, var.index(), inv};
  auto e = op.op_step(mRoot);
  return Bdd{mMgr, e};
}

// @brief 自分自身を否定する．
Bdd&
Bdd::invert_int()
{
  // これは BddMgr を介さない．
  mRoot = ~mRoot;
  return *this;
}

// @brief 論理積を計算して代入する．
Bdd&
Bdd::and_int(
  const Bdd& right
)
{
  ASSERT_COND( mMgr != nullptr );
  ASSERT_COND( mMgr == right.mMgr );
  IteOp op{mMgr};
  auto e = op.and_step(mRoot, right.mRoot);
  change_root(e);
  return *this;
}

// @brief 論理和を計算して代入する．
Bdd&
Bdd::or_int(
  const Bdd& right
)
{
  ASSERT_COND( mMgr != nullptr );
  ASSERT_COND( mMgr == right.mMgr );
  IteOp op{mMgr};
  auto e = op.or_step(mRoot, right.mRoot);
  change_root(e);
  return *this;
}

// @brief 排他的論理和を計算して代入する．
Bdd&
Bdd::xor_int(
  const Bdd& right
)
{
  ASSERT_COND( mMgr != nullptr );
  ASSERT_COND( mMgr == right.mMgr );
  IteOp op{mMgr};
  auto e = op.xor_step(mRoot, right.mRoot);
  change_root(e);
  return *this;
}

// @brief コファクターを計算して代入する．
Bdd&
Bdd::cofactor_int(
  BddVar var,
  bool inv
)
{
  ASSERT_COND( mMgr != nullptr );
  CofactorOp op{mMgr, var.index(), inv};
  auto e = op.op_step(mRoot);
  change_root(e);
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

// @brief 与えられた変数がサポートの時 true を返す．
bool
Bdd::check_sup(
  BddVar var
) const
{
  CheckSupOp op{var.index()};
  return op.op_step(mRoot);
}

// @brief 与えられた変数に対して対称の時 true を返す．
bool
Bdd::check_sym(
  BddVar var1,
  BddVar var2,
  bool inv
) const
{
  CheckSymOp op{var1.index(), var2.index(), inv};
  return op.op_step(mRoot);
}

// @brief 根の変数とコファクターを求める．
BddVar
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
    return BddVar::make_invalid();
  }
  else {
    auto oinv = root.inv();
    f0 = Bdd{mMgr, node->edge0(oinv)};
    f1 = Bdd{mMgr, node->edge1(oinv)};
    return mMgr->var(node->index());
  }
}

// @brief 根の変数を得る．
BddVar
Bdd::root_var() const
{
  ASSERT_COND( mMgr != nullptr );
  BddEdge root{mRoot};
  auto node = root.node();
  if ( node == nullptr ) {
    return BddVar::make_invalid();
  }
  else {
    return mMgr->var(node->index());
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
    return Bdd{mMgr, node->edge0(oinv)};
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
    return Bdd{mMgr, node->edge1(oinv)};
  }
}

// @brief 等価比較演算
bool
Bdd::operator==(
  const Bdd& right ///< [in] オペランド
) const
{
  ASSERT_COND( mMgr == right.mMgr );
  return mRoot == right.mRoot;
}

// @brief ノード数を返す．
SizeType
Bdd::size() const
{
  if ( mMgr == nullptr ) {
    return 0;
  }
  NodeCounter nc;
  return nc.count({mRoot});
}

// @brief 複数のBDDのノード数を数える．
SizeType
Bdd::size(
  const vector<Bdd>& bdd_list
)
{
  if ( bdd_list.empty() ) {
    return 0;
  }
  vector<BddEdge> edge_list;
  (void) root_list(bdd_list, edge_list);
  NodeCounter nc;
  return nc.count(edge_list);
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
    mMgr->dec_ref(mRoot);
    mMgr->inc_ref(new_root);
    mRoot = new_root.body();
  }
}

// @brief 内容を出力する．
void
Bdd::display(
  ostream& s
) const
{
  if ( mMgr == nullptr ) {
    s << "Invalid BDD" << endl;
  }
  else {
    NodeDisp nd{s};
    nd.write({mRoot});
  }
}

// @brief dot 形式で出力する．
void
Bdd::gen_dot(
  ostream& s
) const
{
  DotGen dg{s};
  dg.write({mRoot});
}

// @brief 複数のBDDを dot 形式で出力する．
void
Bdd::gen_dot(
  ostream& s,
  const vector<Bdd>& bdd_list
)
{
  vector<BddEdge> edge_list;
  (void) root_list(bdd_list, edge_list);
  DotGen dg{s};
  dg.write(edge_list);
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
  IteOp op{mgr};
  auto e = op.ite_step(cond.mRoot, then_f.mRoot, else_f.mRoot);
  return Bdd{mgr, e};
}

END_NAMESPACE_YM_BDD
