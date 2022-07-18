
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
#include "CofactorOp.h"
#include "CheckSupOp.h"
#include "CheckSymOp.h"
#include "NodeCounter.h"
#include "NodeDisp.h"
#include "DotGen.h"
#include "IteOp.h"
#include "SupOp.h"
#include "OneOp.h"


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
  if ( mRoot != src.mRoot ) {
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
)
{
  return Bdd{mMgr, BddEdge{mRoot} * inv};
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
  VarId var,
  bool inv
) const
{
  ASSERT_COND( mMgr != nullptr );
  CofactorOp op{mMgr, var.val(), inv};
  auto e = op.op_step(mRoot);
  return Bdd{mMgr, e};
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
  VarId var,
  bool inv
)
{
  ASSERT_COND( mMgr != nullptr );
  CofactorOp op{mMgr, var.val(), inv};
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
  VarId var
) const
{
  CheckSupOp op{var.val()};
  return op.op_step(mRoot);
}

// @brief 与えられた変数に対して対称の時 true を返す．
bool
Bdd::check_sym(
  VarId var1,
  VarId var2,
  bool inv
) const
{
  CheckSymOp op{var1.val(), var2.val(), inv};
  return op.op_step(mRoot);
}

// @brief サポート変数のリストを得る．
Bdd
Bdd::get_support() const
{
  ASSERT_COND( mMgr != nullptr );
  SupOp op{mMgr};
  auto e = op.op_step(mRoot);
  return Bdd{mMgr, e};
}

// @brief 1となるパスを求める．
Bdd
Bdd::get_onepath() const
{
  ASSERT_COND( mMgr != nullptr );
  OneOp op{mMgr};
  auto e = op.op_step(mRoot);
  return Bdd{mMgr, e};
}

// @brief 0となるパスを求める．
Bdd
Bdd::get_zeropath() const
{
  ASSERT_COND( mMgr != nullptr );
  OneOp op{mMgr};
  auto e = op.op_step(~BddEdge{mRoot});
  return Bdd{mMgr, e};
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
    f0 = Bdd{mMgr, node->edge0(oinv)};
    f1 = Bdd{mMgr, node->edge1(oinv)};
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

// @brief 評価を行う．
bool
Bdd::eval(
  const vector<bool>& inputs ///< [in] 入力値ベクタ
) const
{
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
      edge = node->edge1(inv);
    }
    else {
      edge = node->edge0(inv);
    }
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

// @brief 変数のリストに変換する．
vector<VarId>
Bdd::to_varlist() const
{
  ASSERT_COND( mMgr != nullptr );
  BddEdge edge{mRoot};
  vector<VarId> var_list;
  while ( !edge.is_const() ) {
    auto node = edge.node();
    if ( edge.inv() ) {
      throw BddError{"Bdd::to_varlist(): Not a variable list."};
    }
    var_list.push_back(VarId{node->index()});
    if ( !node->edge0().is_zero() ) {
      throw BddError{"Bdd::to_varlist(): Not a variable list."};
    }
    edge = node->edge1();
  }
  return var_list;
}

// @brief リテラルのリストの変換する．
vector<Literal>
Bdd::to_litlist() const
{
  ASSERT_COND( mMgr != nullptr );
  BddEdge edge{mRoot};
  vector<Literal> lit_list;
  while ( !edge.is_const() ) {
    auto node = edge.node();
    auto inv = edge.inv();
    auto e0 = node->edge0(inv);
    auto e1 = node->edge1(inv);
    VarId var{node->index()};
    if ( e0.is_zero() ) {
      lit_list.push_back(Literal{var, false});
      edge = e1;
    }
    else if ( e1.is_zero() ) {
      lit_list.push_back(Literal{var, true});
      edge = e0;
    }
    else {
      throw BddError{"Bdd::to_litlist(): Not a literal list."};
    }
  }
  return lit_list;
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
