
/// @file Bdd.cc
/// @brief Bdd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Bdd.h"
#include "DdEdge.h"
#include "DdNode.h"
#include "BddMgrImpl.h"
#include "CopyOp.h"


BEGIN_NAMESPACE_YM_DD

// @brief 空のコンストラクタ
Bdd::Bdd(
) : mMgr{nullptr},
    mRoot{0}
{
  // 不正値となる．
}

// @brief 内容を指定したコンストラクタ
Bdd::Bdd(
  BddMgrImpl* mgr,
  DdEdge root
) : mMgr{mgr},
    mRoot{root.body()}
{
  if ( mMgr != nullptr ) {
    mMgr->activate(DdEdge{mRoot});
  }
}

// @brief コピーコンストラクタ
Bdd::Bdd(
  const Bdd& src
) : Bdd{src.mMgr, DdEdge{src.mRoot}}
{
  // いわゆる delegate constructor
}

// @brief コピー代入演算子
Bdd&
Bdd::operator=(
  const Bdd& src
)
{
  // この順序なら自分自身に代入しても正しく動作する．
  if ( src.mMgr != nullptr ) {
    src.mMgr->activate(DdEdge{src.mRoot});
  }
  if ( mMgr != nullptr ) {
    mMgr->deactivate(DdEdge{mRoot});
  }
  mMgr = src.mMgr;
  mRoot = src.mRoot;
  return *this;
}

// @brief デストラクタ
Bdd::~Bdd()
{
  if ( mMgr != nullptr ) {
    mMgr->deactivate(DdEdge{mRoot});
  }
}

// @brief 否定した関数を返す．
Bdd
Bdd::invert() const
{
  return Bdd{mMgr, ~DdEdge{mRoot}};
}

// @brief 極性をかけ合わせる．
Bdd
Bdd::operator^(
  bool inv
) const
{
  return Bdd{mMgr, DdEdge{mRoot} ^ inv};
}

// @brief 自分自身を否定する．
Bdd&
Bdd::invert_int()
{
  if ( mMgr != nullptr ) {
    // 実はこれは BddMgr を介さない．
    // ただ不正値の時には演算を行わない．
    mRoot = (~DdEdge{mRoot}).body();
  }
  return *this;
}

// @brief 極性をかけ合わせて代入する．
Bdd&
Bdd::operator^=(
  bool inv
)
{
  if ( mMgr != nullptr ) {
    // 実はこれは BddMgr を介さない．
    // ただ不正値の時には演算を行わない．
    mRoot = (DdEdge{mRoot} ^ inv).body();
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
  return mMgr->and_op(*this, right);
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
  auto mgr = cond.mMgr;
  return cond.mMgr->ite(cond, then_f, else_f);
}

// @brief コファクターを計算する．
Bdd
Bdd::cofactor(
  SizeType var,
  bool inv
) const
{
  _check_valid();
  return mMgr->cofactor(*this, var, inv);
}

// @brief コファクターを計算する．
Bdd
Bdd::cofactor(
  Literal lit
) const
{
  return cofactor(lit.varid(), lit.is_negative());
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
  SizeType var,
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
  Literal lit
)
{
  return cofactor_int(lit.varid(), lit.is_negative());
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
  const unordered_map<SizeType, Bdd>& compose_map
) const
{
  _check_valid();
  return mMgr->multi_compose(*this, compose_map);
}

// @brief 複合compose演算を行って代入する．
Bdd&
Bdd::multi_compose_int(
  const unordered_map<SizeType, Bdd>& compose_map
)
{
  _check_valid();
  *this = mMgr->multi_compose(*this, compose_map);
  return *this;
}

// @brief 変数順を入れ替える演算
Bdd
Bdd::remap_vars(
  const unordered_map<SizeType, Literal>& varmap
) const
{
  _check_valid();
  return mMgr->remap_vars(*this, varmap);
}

// @brief 定数0の時 true を返す．
bool
Bdd::is_zero() const
{
  return is_valid() && DdEdge{mRoot}.is_zero();
}

// @brief 定数1の時 true を返す．
bool
Bdd::is_one() const
{
  return is_valid() && DdEdge{mRoot}.is_one();
}

// @brief 定数の時 true を返す．
bool
Bdd::is_const() const
{
  return is_valid() && DdEdge{mRoot}.is_const();
}

// @brief 根の変数とコファクターを求める．
SizeType
Bdd::root_decomp(
  Bdd& f0,
  Bdd& f1
) const
{
  _check_valid();

  auto root = DdEdge{mRoot};
  auto node = root.node();
  if ( node == nullptr ) {
    f0 = *this;
    f1 = *this;
    return BAD_VARID;
  }
  else {
    auto oinv = root.inv();
    f0 = Bdd{mMgr, node->edge0() ^ oinv};
    f1 = Bdd{mMgr, node->edge1() ^ oinv};
    return node->index();
  }
}

// @brief 根の変数を得る．
SizeType
Bdd::root_var() const
{
  _check_valid();

  auto root = DdEdge{mRoot};
  auto node = root.node();
  if ( node == nullptr ) {
    return BAD_VARID;
  }
  else {
    return node->index();
  }
}

// @brief 負のコファクターを返す．
Bdd
Bdd::root_cofactor0() const
{
  _check_valid();

  auto root = DdEdge{mRoot};
  auto node = root.node();
  if ( node == nullptr ) {
    return *this;
  }
  else {
    auto oinv = root.inv();
    return Bdd{mMgr, node->edge0() ^ oinv};
  }
}

// @brief 正のコファクターを返す．
Bdd
Bdd::root_cofactor1() const
{
  _check_valid();

  auto root = DdEdge{mRoot};
  auto node = root.node();
  if ( node == nullptr ) {
    return *this;
  }
  else {
    auto oinv = root.inv();
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

  auto edge = DdEdge{mRoot};
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

  auto root = DdEdge{mRoot};
  return root.hash();
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
  mMgr->deactivate(DdEdge{mRoot});
  mRoot = new_root.body();
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

// @brief 内容を出力する．
void
Bdd::display(
  ostream& s
) const
{
  _check_valid();
  mMgr->display(s, {*this});
}

// @brief ノード数を返す．
SizeType
Bdd::size() const
{
  if ( mMgr == nullptr ) {
    return 0;
  }
  return mMgr->bdd_size({*this});
}

END_NAMESPACE_YM_DD
