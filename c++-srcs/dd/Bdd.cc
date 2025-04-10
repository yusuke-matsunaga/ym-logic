
/// @file Bdd.cc
/// @brief Bdd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Bdd.h"
#include "ym/BddVar.h"
#include "ym/BddLit.h"
#include "ym/BddMgr.h"
#include "DdEdge.h"
#include "DdNode.h"
#include "DdInfoMgr.h"
#include "BddMgrImpl.h"


BEGIN_NAMESPACE_YM_DD

// @brief 内容を指定したコンストラクタ
Bdd::Bdd(
  const BddMgrHolder& holder,
  DdEdge edge
) : BddMgrHolder(holder),
    mRoot{edge.body()}
{
  if ( is_valid() ) {
    get()->activate(root());
  }
}

// @brief コピーコンストラクタ
Bdd::Bdd(
  const Bdd& src
) : Bdd(src, src.root())
{
  // delegate constructor パタン
}

// @brief コピー代入演算子
Bdd&
Bdd::operator=(
  const Bdd& src
)
{
  // この順序なら自分自身に代入しても正しく動作する．
  if ( src.is_valid() ) {
    src.get()->activate(src.root());
  }
  if ( is_valid() ) {
    get()->deactivate(root());
  }
  BddMgrHolder::operator=(src);
  mRoot = src.mRoot;
  return *this;
}

// @brief デストラクタ
Bdd::~Bdd()
{
  if ( is_valid() ) {
    get()->deactivate(root());
  }
}

// @brief 否定した関数を返す．
Bdd
Bdd::invert() const
{
  return Bdd(*this, ~root());
}

// @brief 極性をかけ合わせる．
Bdd
Bdd::operator^(
  bool inv
) const
{
  return Bdd(*this, root() ^ inv);
}

// @brief 自分自身を否定する．
Bdd&
Bdd::invert_int()
{
  if ( is_valid() ) {
    // 実はこれは BddMgr を介さない．
    // ただ不正値の時には演算を行わない．
    mRoot = (~root()).body();
  }
  return *this;
}

// @brief 極性をかけ合わせて代入する．
Bdd&
Bdd::operator^=(
  bool inv
)
{
  if ( is_valid() ) {
    // 実はこれは BddMgr を介さない．
    // ただ不正値の時には演算を行わない．
    mRoot = (root() ^ inv).body();
  }
  return *this;
}

// @brief 論理積を返す．
Bdd
Bdd::and_op(
  const Bdd& right
) const
{
  auto edge = _and_op(right);
  return _bdd(edge);
}

// @brief 論理積を計算して代入する．
Bdd&
Bdd::and_int(
  const Bdd& right
)
{
  auto edge = _and_op(right);
  _change_root(edge);
  return *this;
}

// @brief 論理和を返す．
Bdd
Bdd::or_op(
  const Bdd& right
) const
{
  auto edge = _or_op(right);
  return _bdd(edge);
}

// @brief 論理和を計算して代入する．
Bdd&
Bdd::or_int(
  const Bdd& right
)
{
  auto edge = _or_op(right);
  _change_root(edge);
  return *this;
}

// @brief 排他的論理和を返す．
Bdd
Bdd::xor_op(
  const Bdd& right
) const
{
  auto edge = _xor_op(right);
  return _bdd(edge);
}

// @brief 排他的論理和を計算して代入する．
Bdd&
Bdd::xor_int(
  const Bdd& right
)
{
  auto edge = _xor_op(right);
  _change_root(edge);
  return *this;
}

// @brief コファクターを計算する．
Bdd
Bdd::cofactor(
  const BddLit& lit
) const
{
  return cofactor(lit.var(), lit.is_negative());
}

// @brief コファクターを計算して代入する．
Bdd&
Bdd::cofactor_int(
  const BddLit& lit
)
{
  return cofactor_int(lit.var(), lit.is_negative());
}

// @brief コファクターを計算する．
Bdd
Bdd::cofactor(
  const BddVar& var,
  bool inv
) const
{
  auto edge = _cofactor(var, inv);
  return _bdd(edge);
}

// @brief コファクターを計算して代入する．
Bdd&
Bdd::cofactor_int(
  const BddVar& var,
  bool inv
)
{
  auto edge = _cofactor(var, inv);
  _change_root(edge);
  return *this;
}

// @brief コファクターを計算する．
Bdd
Bdd::cofactor(
  const Bdd& cube
) const
{
  auto edge = _cofactor(cube);
  return _bdd(edge);
}

/// @brief コファクターを計算して代入する．
Bdd&
Bdd::cofactor_int(
  const Bdd& cube
)
{
  auto edge = _cofactor(cube);
  _change_root(edge);
  return *this;
}

// @brief (単一)compose演算
Bdd
Bdd::compose(
  const BddVar& var,
  const Bdd& cfunc
) const
{
  return multi_compose({{var, cfunc}});
}

// @brief (単一)compose演算を行って代入する．
Bdd&
Bdd::compose_int(
  const BddVar& var,
  const Bdd& cfunc
)
{
  return multi_compose_int({{var, cfunc}});
}

// @brief 複合compose演算
Bdd
Bdd::multi_compose(
  const unordered_map<BddVar, Bdd>& compose_map
) const
{
  auto edge = _multi_compose(compose_map);
  return _bdd(edge);
}

// @brief 複合compose演算を行って代入する．
Bdd&
Bdd::multi_compose_int(
  const unordered_map<BddVar, Bdd>& compose_map
)
{
  auto edge = _multi_compose(compose_map);
  _change_root(edge);
  return *this;
}

// @brief 変数順を入れ替える演算
Bdd
Bdd::remap_vars(
  const unordered_map<BddVar, BddLit>& varmap
) const
{
  auto edge = _remap_vars(varmap);
  return _bdd(edge);
}

// @brief 親のマネージャを返す．
BddMgr
Bdd::mgr() const
{
  return BddMgr(*this);
}

// @brief 定数0の時 true を返す．
bool
Bdd::is_zero() const
{
  return is_valid() && root().is_zero();
}

// @brief 定数1の時 true を返す．
bool
Bdd::is_one() const
{
  return is_valid() && root().is_one();
}

// @brief 定数の時 true を返す．
bool
Bdd::is_const() const
{
  return is_valid() && root().is_const();
}

// @brief 変数の時 true を返す．
bool
Bdd::is_variable() const
{
  return is_positive_literal();
}

// @brief リテラルの時 true を返す．
bool
Bdd::is_literal() const
{
  if ( is_invalid() ) {
    return false;
  }
  auto node = root().node();
  auto e0 = node->edge0();
  if ( !e0.is_const() ) {
    return false;
  }
  auto e1 = node->edge1();
  if ( !e1.is_const() ) {
    return false;
  }
  return true;
}

// @brief 肯定のリテラルの時 true を返す．
bool
Bdd::is_positive_literal() const
{
  if ( !is_literal() ) {
    return false;
  }
  return !root().inv();
}

// @brief 否定のリテラルの時 true を返す．
bool
Bdd::is_negative_literal() const
{
  if ( !is_literal() ) {
    return false;
  }
  return root().inv();
}

// @brief 根の変数とコファクターを求める．
BddVar
Bdd::root_decomp(
  Bdd& f0,
  Bdd& f1
) const
{
  _check_valid();

  auto node = root().node();
  if ( node == nullptr ) {
    f0 = *this;
    f1 = *this;
    return BddVar::invalid();
  }
  auto oinv = root().inv();
  f0 = _bdd(node->edge0() ^ oinv);
  f1 = _bdd(node->edge1() ^ oinv);
  auto var = _level_to_var(node->level());
  return var;
}

// @brief 根の変数を得る．
BddVar
Bdd::root_var() const
{
  _check_valid();

  auto node = root().node();
  if ( node == nullptr ) {
    return BddVar::invalid();
  }
  auto var = _level_to_var(node->level());
  return var;
}

// @brief 負のコファクターを返す．
Bdd
Bdd::root_cofactor0() const
{
  _check_valid();

  auto node = root().node();
  if ( node == nullptr ) {
    return *this;
  }
  auto oinv = root().inv();
  return _bdd(node->edge0() ^ oinv);
}

// @brief 正のコファクターを返す．
Bdd
Bdd::root_cofactor1() const
{
  _check_valid();

  auto node = root().node();
  if ( node == nullptr ) {
    return *this;
  }
  auto oinv = root().inv();
  return _bdd(node->edge1() ^ oinv);
}

// @brief 根が否定されている時 true を返す．
bool
Bdd::root_inv() const
{
  _check_valid();

  return static_cast<bool>(mRoot & 1UL);
}

// @brief 評価を行う．
bool
Bdd::eval(
  const vector<bool>& inputs ///< [in] 入力値ベクタ
) const
{
  _check_valid();

  auto edge = root();
  for ( ; ; ) {
    if ( edge.is_zero() ) {
      return false;
    }
    if ( edge.is_one() ) {
      return true;
    }
    auto node = edge.node();
    auto inv = edge.inv();
    auto level = node->level();
    if ( inputs[level] ) {
      edge = node->edge1();
    }
    else {
      edge = node->edge0();
    }
    edge ^= inv;
  }
}

// @brief 等価比較演算
bool
Bdd::operator==(
  const Bdd& right ///< [in] オペランド
) const
{
  return get() == right.get() && mRoot == right.mRoot;
}

// @brief ハッシュ値を返す．
SizeType
Bdd::hash() const
{
  _check_valid();

  return root().hash();
}

// @brief 根の枝を変更する．
void
Bdd::_change_root(
  DdEdge new_root
)
{
  _check_valid();

  // この順序なら new_root と mRoot が等しくても
  // 正しく動く
  get()->activate(new_root);
  get()->deactivate(root());
  mRoot = new_root.body();
}

// @brief 内容を出力する．
void
Bdd::display(
  ostream& s
) const
{
  auto edge = root();
  auto mgr = get();
  DdInfoMgr info_mgr({edge}, mgr);
  info_mgr.display(s);
}

// @brief 構造を表す整数配列を作る．
vector<SizeType>
Bdd::rep_data() const
{
  auto edge = root();
  auto mgr = get();
  DdInfoMgr info_mgr({edge}, mgr);
  return info_mgr.rep_data();
}

// @brief 独自形式でバイナリダンプする．
void
Bdd::dump(
  BinEnc& s
) const
{
  auto edge = root();
  auto mgr = get();
  DdInfoMgr info_mgr({edge}, mgr);
  mgr->dump(s, info_mgr);
}

// @brief ノード数を返す．
SizeType
Bdd::size() const
{
  if ( is_invalid() ) {
    return 0;
  }
  auto edge = root();
  auto mgr = get();
  DdInfoMgr info_mgr({edge}, mgr);
  return info_mgr.node_num();
}

// @brief 根の枝を返す．
DdEdge
Bdd::root() const
{
  return DdEdge{mRoot};
}

// @brief 複数のBDDのノード数を数える．
SizeType
Bdd::bdd_size(
  const vector<Bdd>& bdd_list
)
{
  if ( bdd_list.empty() ) {
    return 0;
  }
  auto mgr = _mgr(bdd_list);
  auto edge_list = _conv_to_edgelist(bdd_list);
  DdInfoMgr info_mgr(edge_list, mgr);
  return info_mgr.node_num();
}

// @brief 複数のBDDの内容を出力する．
void
Bdd::display(
  ostream& s,
  const vector<Bdd>& bdd_list
)
{
  if ( bdd_list.empty() ) {
    return;
  }
  auto mgr = _mgr(bdd_list);
  auto edge_list = _conv_to_edgelist(bdd_list);
  DdInfoMgr info_mgr(edge_list, mgr);
  info_mgr.display(s);
}

// @brief 構造を表す整数配列を作る．
vector<SizeType>
Bdd::rep_data(
  const vector<Bdd>& bdd_list
)
{
  if ( bdd_list.empty() ) {
    return {};
  }
  auto mgr = _mgr(bdd_list);
  auto edge_list = _conv_to_edgelist(bdd_list);
  DdInfoMgr info_mgr(edge_list, mgr);
  return info_mgr.rep_data();
}

// @brief BDD の内容をバイナリダンプする．
void
Bdd::dump(
  BinEnc& s,
  const vector<Bdd>& bdd_list
)
{
  auto mgr = _mgr(bdd_list);
  auto edge_list = _conv_to_edgelist(bdd_list);
  DdInfoMgr info_mgr(edge_list, mgr);
  mgr->dump(s, info_mgr);
}

// @brief レベルを変数に変換する．
BddVar
Bdd::_level_to_var(
  SizeType level ///< [in] レベル
) const
{
  auto edge = get()->new_node(level, DdEdge::zero(), DdEdge::one());
  return _var(edge);
}

// @brief BDDのリストからマネージャを取り出す．
BddMgrImpl*
Bdd::_mgr(
  const vector<Bdd>& bdd_list ///< [in] BDDのリスト
)
{
  if ( bdd_list.empty() ) {
    return nullptr;
  }

  // sanity check
  auto bdd0 = bdd_list.front();
  bdd0._check_valid();
  for ( auto& bdd: bdd_list ) {
    bdd0._check_mgr(bdd);
  }
  return bdd0.get();
}

// @brief BDDのリストから枝のリストに変換する．
vector<DdEdge>
Bdd::_conv_to_edgelist(
  const vector<Bdd>& bdd_list
)
{
  if ( bdd_list.empty() ) {
    return {};
  }

  vector<DdEdge> edge_list;
  edge_list.reserve(bdd_list.size());
  for ( auto& bdd: bdd_list ) {
    auto edge = bdd.root();
    edge_list.push_back(edge);
  }
  return edge_list;
}


//////////////////////////////////////////////////////////////////////
// クラス BddVar
//////////////////////////////////////////////////////////////////////

// @brief 内容を指定したコンストラクタ
BddVar::BddVar(
  const BddMgrHolder& holder,
  DdEdge root
) : Bdd(holder, root)
{
  _check_valid();
}

// @brief Bdd からの変換関数
BddVar
BddVar::from_bdd(
  const Bdd& bdd
)
{
  if ( bdd.is_variable() ) {
    return BddVar(bdd);
  }
  return invalid();
}

// @brief 変数番号を返す．
SizeType
BddVar::id() const
{
  return get()->level_to_varid(level());
}

// @brief レベルを返す．
SizeType
BddVar::level() const
{
  _check_valid();
  auto node = root().node();
  return node->level();
}

// @brief 肯定のリテラルを返す．
BddLit
BddVar::positive_literal() const
{
  return BddLit(*this, false);
}

// @brief 否定のリテラルを返す．
BddLit
BddVar::negative_literal() const
{
  return BddLit(*this, true);
}

// @brief BddVar のリストから DdEdge のリストに変換する．
vector<DdEdge>
BddVar::conv_to_edgelist(
  const vector<BddVar>& var_list
)
{
  vector<DdEdge> edge_list;
  edge_list.reserve(var_list.size());
  for ( auto var: var_list ) {
    auto edge = var.root();
    edge_list.push_back(edge);
  }
  return edge_list;
}

// @brief BddVar のリストからレベルのリストに変換する．
vector<SizeType>
BddVar::conv_to_levellist(
  const vector<BddVar>& var_list
)
{
  vector<SizeType> level_list;
  level_list.reserve(var_list.size());
  for ( auto var: var_list ) {
    auto level = var.level();
    level_list.push_back(level);
  }
  return level_list;
}

END_NAMESPACE_YM_DD
