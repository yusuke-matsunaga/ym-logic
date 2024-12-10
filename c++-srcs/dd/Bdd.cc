
/// @file Bdd.cc
/// @brief Bdd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Bdd.h"
#include "ym/BddVar.h"
#include "ym/BddMgr.h"
#include "DdEdge.h"
#include "DdNode.h"
#include "BddMgrImpl.h"


BEGIN_NAMESPACE_YM_DD

// @brief 内容を指定したコンストラクタ
Bdd::Bdd(
  const BddMgrPtr& mgr,
  DdEdge edge
) : mMgr{mgr},
    mRoot{edge.body()}
{
  cout << "Bdd:const1" << endl;
  if ( is_valid() ) {
    mMgr->activate(root());
  }
}

// @brief コピーコンストラクタ
Bdd::Bdd(
  const Bdd& src
) : mMgr{src.mMgr},
    mRoot{src.mRoot}
{
  cout << "Bdd:copy_const" << endl;
  if ( is_valid() ) {
    mMgr->activate(root());
  }
}

// @brief コピー代入演算子
Bdd&
Bdd::operator=(
  const Bdd& src
)
{
  cout << "Bdd:copy_assign" << endl;
  // この順序なら自分自身に代入しても正しく動作する．
  if ( src.is_valid() ) {
    src.mMgr->activate(src.root());
  }
  if ( is_valid() ) {
    mMgr->deactivate(root());
  }
  mMgr = src.mMgr;
  mRoot = src.mRoot;
  return *this;
}

// @brief ムーブコンストラクタ
Bdd::Bdd(
  const Bdd&& src
) : mMgr{std::move(src.mMgr)},
    mRoot{src.mRoot}
{
  cout << "Bdd:move_const" << endl;
}

// @brief デストラクタ
Bdd::~Bdd()
{
  cout << "Bdd:destr" << endl;
  if ( is_valid() ) {
    mMgr->deactivate(root());
  }
}

// @brief 否定した関数を返す．
Bdd
Bdd::invert() const
{
  return Bdd{mMgr, ~root()};
}

// @brief 極性をかけ合わせる．
Bdd
Bdd::operator^(
  bool inv
) const
{
  return Bdd{mMgr, root() ^ inv};
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
  _check_valid();
#if 0
  return mMgr->and_op(*this, right);
#else
  cout << "before and_op()" << endl;
  auto e = mMgr->and_op(*this, right);
  cout << "after and_op()" << endl;
  return e;
#endif
}

// @brief 論理和を返す．
Bdd
Bdd::or_op(
  const Bdd& right
) const
{
  _check_valid();
  return mMgr->or_op(*this, right);
}

// @brief 排他的論理和を返す．
Bdd
Bdd::xor_op(
  const Bdd& right
) const
{
  _check_valid();
  return mMgr->xor_op(*this, right);
}

// @brief 論理積を計算して代入する．
Bdd&
Bdd::and_int(
  const Bdd& right
)
{
  _check_valid();
  *this = mMgr->and_op(*this, right);
  return *this;
}

// @brief 論理和を計算して代入する．
Bdd&
Bdd::or_int(
  const Bdd& right
)
{
  _check_valid();
  *this = mMgr->or_op(*this, right);
  return *this;
}

// @brief 排他的論理和を計算して代入する．
Bdd&
Bdd::xor_int(
  const Bdd& right
)
{
  _check_valid();
  *this = mMgr->xor_op(*this, right);
  return *this;
}

// @brief If-Then-Else 演算
Bdd
ite(
  const Bdd& cond,
  const Bdd& then_f,
  const Bdd& else_f
)
{
  cond._check_valid();
  return cond.mMgr->ite(cond, then_f, else_f);
}

// @brief ドントケアを利用した簡単化を行う．
Bdd
simplify(
  const Bdd& on,
  const Bdd& dc
)
{
  on._check_valid();
  return on.mMgr->simplify(on, dc);
}

// @brief コファクターを計算する．
Bdd
Bdd::cofactor(
  const BddVar& var,
  bool inv
) const
{
  _check_valid();
  return mMgr->cofactor(*this, var, inv);
}

// @brief コファクターを計算する．
Bdd
Bdd::cofactor(
  const BddLit& lit
) const
{
  return cofactor(lit.var(), lit.is_negative());
}

// @brief コファクターを計算する．
Bdd
Bdd::cofactor(
  const Bdd& cube
) const
{
  _check_valid();
  return mMgr->cofactor(*this, cube);
}

// @brief コファクターを計算して代入する．
Bdd&
Bdd::cofactor_int(
  const BddVar& var,
  bool inv
)
{
  _check_valid();
  *this = mMgr->cofactor(*this, var, inv);
  return *this;
}

// @brief コファクターを計算して代入する．
Bdd&
Bdd::cofactor_int(
  const BddLit& lit
)
{
  return cofactor_int(lit.var(), lit.is_negative());
}

/// @brief コファクターを計算して代入する．
Bdd&
Bdd::cofactor_int(
  const Bdd& cube
)
{
  _check_valid();
  *this = mMgr->cofactor(*this, cube);
  return *this;
}

// @brief 複合compose演算
Bdd
Bdd::multi_compose(
  const unordered_map<BddVar, Bdd>& compose_map
) const
{
  _check_valid();
  return mMgr->multi_compose(*this, compose_map);
}

// @brief 複合compose演算を行って代入する．
Bdd&
Bdd::multi_compose_int(
  const unordered_map<BddVar, Bdd>& compose_map
)
{
  _check_valid();
  *this = mMgr->multi_compose(*this, compose_map);
  return *this;
}

// @brief 変数順を入れ替える演算
Bdd
Bdd::remap_vars(
  const unordered_map<BddVar, BddLit>& varmap
) const
{
  _check_valid();
  return mMgr->remap_vars(*this, varmap);
}

// @brief 親のマネージャを返す．
BddMgr
Bdd::mgr() const
{
  return BddMgr{mMgr};
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
  return is_posilit();
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
Bdd::is_posilit() const
{
  if ( !is_literal() ) {
    return false;
  }
  return !root().inv();
}

// @brief 否定のリテラルの時 true を返す．
bool
Bdd::is_negalit() const
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
  else {
    auto oinv = root().inv();
    f0 = Bdd{mMgr, node->edge0() ^ oinv};
    f1 = Bdd{mMgr, node->edge1() ^ oinv};
    auto var = mMgr->level_to_var(node->level());
    return var;
  }
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
  else {
    auto var = mMgr->level_to_var(node->level());
    return var;
  }
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
  else {
    auto oinv = root().inv();
    return Bdd{mMgr, node->edge0() ^ oinv};
  }
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
  else {
    auto oinv = root().inv();
    return Bdd{mMgr, node->edge1() ^ oinv};
  }
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
  return mMgr == right.mMgr && mRoot == right.mRoot;
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
Bdd::change_root(
  DdEdge new_root
)
{
  _check_valid();

  // この順序なら new_root と mRoot が等しくても
  // 正しく動く
  mMgr->activate(new_root);
  mMgr->deactivate(root());
  mRoot = new_root.body();
}

// @brief 内容を出力する．
void
Bdd::display(
  ostream& s
) const
{
  _check_valid();
  mMgr->display(s, {*this});
}

// @brief dot 形式で出力する．
void
Bdd::gen_dot(
  ostream& s,
  const JsonValue& option
) const
{
  _check_valid();
  mMgr->gen_dot(s, {*this}, option);
}

// @brief 構造を表す整数配列を作る．
vector<SizeType>
Bdd::rep_data() const
{
  _check_valid();
  return mMgr->rep_data({*this});
}

// @brief 独自形式でバイナリダンプする．
void
Bdd::dump(
  BinEnc& s
) const
{
  _check_valid();
  mMgr->dump(s, {*this});
}

// @brief ノード数を返す．
SizeType
Bdd::size() const
{
  if ( is_invalid() ) {
    return 0;
  }
  return mMgr->bdd_size({*this});
}

// @brief 根の枝を返す．
DdEdge
Bdd::root() const
{
  return DdEdge{mRoot};
}


//////////////////////////////////////////////////////////////////////
// クラス BddVar
//////////////////////////////////////////////////////////////////////

// @brief 内容を指定したコンストラクタ
BddVar::BddVar(
  const BddMgrPtr& mgr,
  DdEdge root
) : Bdd{mgr, root}
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
    return BddVar{bdd};
  }
  return invalid();
}

// @brief 変数番号を返す．
SizeType
BddVar::id() const
{
  return mgr_ptr()->level_to_varid(level());
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
BddVar::posilit() const
{
  return BddLit{*this, false};
}

// @brief 否定のリテラルを返す．
BddLit
BddVar::negalit() const
{
  return BddLit{*this, true};
}

END_NAMESPACE_YM_DD
