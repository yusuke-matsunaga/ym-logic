
/// @file Zdd.cc
/// @brief Zdd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Zdd.h"
#include "ym/ZddMgr.h"
#include "dd/DdEdge.h"
#include "dd/ZddMgrImpl.h"
#include "dd/DdNode.h"
#include "CopyOp.h"


BEGIN_NAMESPACE_YM_DD

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
  DdEdge root
) : mMgr{mgr},
    mRoot{root.body()}
{
  if ( mMgr != nullptr ) {
    mMgr->inc();
    mMgr->activate(DdEdge{mRoot});
  }
}

// @brief コピーコンストラクタ
Zdd::Zdd(
  const Zdd& src
) : Zdd{src.mMgr, DdEdge{src.mRoot}}
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
    src.mMgr->inc();
    src.mMgr->activate(DdEdge{src.mRoot});
  }
  if ( mMgr != nullptr ) {
    mMgr->deactivate(DdEdge{mRoot});
    mMgr->dec();
  }
  mMgr = src.mMgr;
  mRoot = src.mRoot;
  return *this;
}

// @brief デストラクタ
Zdd::~Zdd()
{
  if ( mMgr != nullptr ) {
    mMgr->deactivate(DdEdge{mRoot});
    mMgr->dec();
  }
}

// @brief 親のマネージャを返す．
ZddMgr
Zdd::mgr() const
{
  return ZddMgr{mMgr};
}

// @brief 否定した関数を返す．
Zdd
Zdd::invert() const
{
  if ( mMgr != nullptr ) {
    return mMgr->invert(*this);
  }
  // 不正値の時には演算を行わない．
  return *this;
}

// @brief 自分自身を否定する．
Zdd&
Zdd::invert_int()
{
  if ( mMgr != nullptr ) {
    *this = mMgr->invert(*this);
  }
  // 不正値の時には演算を行わない．
  return *this;
}

// @brief CAP演算
Zdd
Zdd::cap(
  const Zdd& right
) const
{
  _check_valid();
  return mMgr->cap(*this, right);
}

// @brief CAP演算つき代入
Zdd&
Zdd::cap_int(
  const Zdd& right
)
{
  _check_valid();
  *this = mMgr->cap(*this, right);
  return *this;
}

// @brief CUP演算
Zdd
Zdd::cup(
  const Zdd& right
) const
{
  _check_valid();
  return mMgr->cup(*this, right);
}

// @brief CUP演算つき代入
Zdd&
Zdd::cup_int(
  const Zdd& right
)
{
  _check_valid();
  *this = mMgr->cup(*this, right);
  return *this;
}

// @brief DIFF演算
Zdd
Zdd::diff(
  const Zdd& right
) const
{
  _check_valid();
  return mMgr->diff(*this, right);
}

// @brief DIFF演算つき代入
Zdd&
Zdd::diff_int(
  const Zdd& right
)
{
  _check_valid();
  *this = mMgr->diff(*this, right);
  return *this;
}

// @brief 要素を含む集合を求める．
Zdd
Zdd::onset(
  const ZddItem& item
) const
{
  _check_valid();
  return mMgr->onset(*this, item);
}

// @brief onset を計算して代入する．
Zdd&
Zdd::onset_int(
  const ZddItem& item
)
{
  _check_valid();
  *this = mMgr->onset(*this, item);
  return *this;
}

// @brief 要素を含まない集合を求める．
Zdd
Zdd::offset(
  const ZddItem& item
) const
{
  _check_valid();
  return mMgr->offset(*this, item);
}

// @brief offset を計算して代入する．
Zdd&
Zdd::offset_int(
  const ZddItem& item
)
{
  _check_valid();
  *this = mMgr->offset(*this, item);
  return *this;
}

// @brief PRODUCT演算
Zdd
Zdd::product(
  const Zdd& right
) const
{
  _check_valid();
  return mMgr->product(*this, right);
}

// @brief PRODUCT付き代入
Zdd&
Zdd::product_int(
  const Zdd& right
)
{
  _check_valid();
  *this = mMgr->product(*this, right);
  return *this;
}

// @brief 定数0の時 true を返す．
bool
Zdd::is_zero() const
{
  return is_valid() && DdEdge{mRoot}.is_zero();
}

// @brief 定数1の時 true を返す．
bool
Zdd::is_one() const
{
  return is_valid() && DdEdge{mRoot}.is_one();
}

// @brief 定数の時 true を返す．
bool
Zdd::is_const() const
{
  return is_valid() && DdEdge{mRoot}.is_const();
}

// @brief シングルトンの時 true を返す．
bool
Zdd::is_singleton() const
{
  if ( is_invalid() ) {
    return false;
  }
  auto node = root().node();
  auto e0 = node->edge0();
  if ( !e0.is_zero() ) {
    return false;
  }
  auto e1 = node->edge1();
  if ( !e1.is_one() ) {
    return false;
  }
  return true;
}

// @brief 根の変数とコファクターを求める．
ZddItem
Zdd::root_decomp(
  Zdd& f0,
  Zdd& f1
) const
{
  _check_valid();

  auto root = DdEdge{mRoot};
  auto node = root.node();
  if ( node == nullptr ) {
    f0 = *this;
    f1 = *this;
    return ZddItem::invalid();
  }
  else {
    f0 = Zdd{mMgr, node->edge0()};
    f1 = Zdd{mMgr, node->edge1()};
    auto item = mMgr->index_to_item(node->index());
    return item;
  }
}

// @brief 根の変数を得る．
ZddItem
Zdd::root_item() const
{
  _check_valid();

  auto root = DdEdge{mRoot};
  auto node = root.node();
  if ( node == nullptr ) {
    return ZddItem::invalid();
  }
  else {
    auto item = mMgr->index_to_item(node->index());
    return item;
  }
}

// @brief 負のコファクターを返す．
Zdd
Zdd::root_cofactor0() const
{
  _check_valid();

  auto root = DdEdge{mRoot};
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

  auto root = DdEdge{mRoot};
  auto node = root.node();
  if ( node == nullptr ) {
    return *this;
  }
  else {
    return Zdd{mMgr, node->edge1()};
  }
}

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

  auto root = DdEdge{mRoot};
  return root.hash();
}

// @brief 根の枝を変更する．
void
Zdd::change_root(
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

// @brief 内容を出力する．
void
Zdd::display(
  ostream& s
) const
{
  _check_valid();
  mMgr->display(s, {*this});
}

// @brief dot 形式で出力する．
void
Zdd::gen_dot(
  ostream& s,
  const unordered_map<string, string>& attr_dict
) const
{
  _check_valid();
  mMgr->gen_dot(s, {*this}, attr_dict);
}

// @brief 構造を表す整数配列を作る．
vector<SizeType>
Zdd::rep_data() const
{
  _check_valid();
  return mMgr->rep_data({*this});
}

// @brief 独自形式でバイナリダンプする．
void
Zdd::dump(
  BinEnc& s
) const
{
  _check_valid();
  mMgr->dump(s, {*this});
}

// @brief ノード数を返す．
SizeType
Zdd::size() const
{
  _check_valid();
  return mMgr->zdd_size({*this});
}

// @brief 根の枝を返す．
DdEdge
Zdd::root() const
{
  return DdEdge{mRoot};
}


//////////////////////////////////////////////////////////////////////
// クラス ZddItem
//////////////////////////////////////////////////////////////////////

// @brief 内容を指定したコンストラクタ
ZddItem::ZddItem(
  ZddMgrImpl* mgr,
  DdEdge root
) : Zdd{mgr, root}
{
  _check_valid();
}

// @breif Zdd からの変換関数
ZddItem
ZddItem::from_zdd(
  const Zdd& zdd
)
{
  if ( zdd.is_singleton() ) {
    return ZddItem{zdd};
  }
  return invalid();
}

// @brief 要素番号を返す．
SizeType
ZddItem::id() const
{
  return _mgr()->index_to_varid(index());
}

// @brief インデックスを返す．
SizeType
ZddItem::index() const
{
  _check_valid();
  auto node = root().node();
  return node->index();
}

END_NAMESPACE_YM_DD
