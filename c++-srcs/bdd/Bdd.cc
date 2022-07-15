
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


BEGIN_NAMESPACE_YM

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
  auto e = mMgr->and_op(mRoot, right.mRoot);
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
  auto e = mMgr->or_op(mRoot, right.mRoot);
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
  auto e = mMgr->xor_op(mRoot, right.mRoot);
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
  auto e = mMgr->cofactor_op(mRoot, var.index(), inv);
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
  auto e = mMgr->and_op(mRoot, right.mRoot);
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
  auto e = mMgr->or_op(mRoot, right.mRoot);
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
  auto e = mMgr->xor_op(mRoot, right.mRoot);
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
  auto e = mMgr->cofactor_op(mRoot, var.index(), inv);
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
  ASSERT_COND( mMgr != nullptr );
  return mMgr->check_sup(mRoot, var.index());
}

// @brief 与えられた変数に対して対称の時 true を返す．
bool
Bdd::check_sym(
  BddVar var1,
  BddVar var2,
  bool inv
) const
{
  ASSERT_COND( mMgr != nullptr );
  return mMgr->check_sym(mRoot, var1.index(), var2.index(), inv);
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

// @brief ノード数を返す．
SizeType
Bdd::size() const
{
  ASSERT_COND( mMgr != nullptr );
  vector<BddEdge> edge_list{mRoot};
  return mMgr->count_size(edge_list);
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
  auto mgr = root_list(bdd_list, edge_list);
  return mgr->count_size(edge_list);
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

BEGIN_NONAMESPACE

void
dfs(
  BddEdge e,
  vector<BddNode*>& node_list,
  unordered_map<BddNode*, SizeType>& node_map
)
{
  if ( !e.is_const() ) {
    auto node = e.node();
    if ( node_map.count(node) == 0 ) {
      SizeType id = node_list.size();
      node_map.emplace(node, id);
      node_list.push_back(node);
      dfs(node->edge0(), node_list, node_map);
      dfs(node->edge1(), node_list, node_map);
    }
  }
}

void
display_edge(
  ostream& s,
  BddEdge e,
  const unordered_map<BddNode*, SizeType>& node_map
)
{
  if ( e.is_zero() ) {
    s << "   ZERO";
  }
  else if ( e.is_one() ) {
    s << "    ONE";
  }
  else {
    auto node = e.node();
    auto inv = e.inv();
    SizeType id = node_map.at(node);
    s << setw(6) << id;
    if ( inv ) {
      s << "~";
    }
    else {
      s << " ";
    }
  }
}

END_NONAMESPACE

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
    vector<BddNode*> node_list;
    unordered_map<BddNode*, SizeType> node_map;
    dfs(BddEdge{mRoot}, node_list, node_map);
    for ( auto node: node_list ) {
      SizeType id = node_map.at(node);
      s << setw(6) << id << ": " << setw(4) << node->index();
      auto edge0 = node->edge0();
      display_edge(s, edge0, node_map);
      s << ": ";
      auto edge1 = node->edge1();
      display_edge(s, edge1, node_map);
      s << endl;
    }
  }
}

// @brief dot 形式で出力する．
void
Bdd::gen_dot(
  ostream& s
) const
{
  gen_dot(s, {*this});
}

// @brief 複数のBDDを dot 形式で出力する．
void
Bdd::gen_dot(
  ostream& s,
  const vector<Bdd>& bdd_list
)
{
  vector<BddEdge> edge_list;
  auto mgr = root_list(bdd_list, edge_list);
  return mgr->gen_dot(s, edge_list);
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

END_NAMESPACE_YM
