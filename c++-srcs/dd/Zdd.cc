
/// @file Zdd.cc
/// @brief Zdd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Zdd.h"
#include "ym/ZddMgr.h"
#include "ym/ZddMgrHolder.h"
#include "ZddMgrImpl.h"
#include "DdEdge.h"
#include "DdNode.h"
#include "DdInfoMgr.h"


BEGIN_NAMESPACE_YM_DD

// @brief 内容を指定したコンストラクタ
Zdd::Zdd(
  const ZddMgrHolder& holder,
  DdEdge edge
) : ZddMgrHolder(holder),
    mRoot{edge.body()}
{
  if ( is_valid() ) {
    get()->activate(root());
  }
}

// @brief コピーコンストラクタ
Zdd::Zdd(
  const Zdd& src
) : Zdd(src, src.root())
{
  // delegate constructor パタン
}

// @brief コピー代入演算子
Zdd&
Zdd::operator=(
  const Zdd& src
)
{
  // この順序なら自分自身に代入しても正しく動作する．
  if ( src.is_valid() ) {
    src.get()->activate(src.root());
  }
  if ( is_valid() ) {
    get()->deactivate(root());
  }
  ZddMgrHolder::operator=(src);
  mRoot = src.mRoot;
  return *this;
}

// @brief デストラクタ
Zdd::~Zdd()
{
  if ( is_valid() ) {
    get()->deactivate(root());
  }
}

// @brief 親のマネージャを返す．
ZddMgr
Zdd::mgr() const
{
  return ZddMgr(*this);
}

// @brief 否定した関数を返す．
Zdd
Zdd::invert() const
{
  return _zdd(~root());
}

// @brief 自分自身を否定する．
Zdd&
Zdd::invert_int()
{
  if ( is_valid() ) {
    mRoot = (~root()).body();
  }
  return *this;
}

// @brief 共通集合演算
Zdd
Zdd::cap(
  const Zdd& right
) const
{
  auto edge = _cap(right);
  return _zdd(edge);
}

// @brief CAP演算つき代入
Zdd&
Zdd::cap_int(
  const Zdd& right
)
{
  auto edge = _cap(right);
  _change_root(edge);
  return *this;
}

// @brief ユニオン演算
Zdd
Zdd::cup(
  const Zdd& right ///< [in] オペランド
) const
{
  auto edge = _cup(right);
  return _zdd(edge);
}

// @brief CUP演算つき代入
Zdd&
Zdd::cup_int(
  const Zdd& right
)
{
  auto edge = _diff(right);
  _change_root(edge);
  return *this;
}

// @brief 集合差演算
Zdd
Zdd::diff(
  const Zdd& right
) const
{
  auto edge = _diff(right);
  return _zdd(edge);
}

// @brief DIFF演算つき代入
Zdd&
Zdd::diff_int(
  const Zdd& right
)
{
  auto edge = _diff(right);
  _change_root(edge);
  return *this;
}

// @brief 直積演算
Zdd
Zdd::product(
  const Zdd& right
) const
{
  auto edge = _product(right);
  return _zdd(edge);
}

// @brief 直積を計算して代入する．
Zdd&
Zdd::product_int(
  const Zdd& right
)
{
  auto edge = _product(right);
  _change_root(edge);
  return *this;
}

// @brief 変数を含む集合を求める．
Zdd
Zdd::onset(
  const ZddItem& item
) const
{
  auto edge = _onset(item);
  return _zdd(edge);
}

// @brief 変数を含む集合を求める．
Zdd&
Zdd::onset_int(
  const ZddItem& item
)
{
  auto edge = _onset(item);
  _change_root(edge);
  return *this;
}

// @brief 変数を含まない集合を求める．
Zdd
Zdd::offset(
  const ZddItem& item
) const
{
  auto edge = _offset(item);
  return _zdd(edge);
}

// @brief offset を計算して代入する．
Zdd&
Zdd::offset_int(
  const ZddItem& item
)
{
  auto edge = _offset(item);
  _change_root(edge);
  return *this;
}

// @brief 定数0の時 true を返す．
bool
Zdd::is_zero() const
{
  return is_valid() && root().is_zero();
}

// @brief 定数1の時 true を返す．
bool
Zdd::is_one() const
{
  return is_valid() && root().is_one();
}

// @brief 定数の時 true を返す．
bool
Zdd::is_const() const
{
  return is_valid() && root().is_const();
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

  auto node = root().node();
  if ( node == nullptr ) {
    f0 = *this;
    f1 = *this;
    return ZddItem::invalid();
  }
  f0 = _zdd(node->edge0());
  f1 = _zdd(node->edge1());
  auto item = _level_to_item(node->level());
  return item;
}

// @brief 根の変数を得る．
ZddItem
Zdd::root_item() const
{
  _check_valid();

  auto node = root().node();
  if ( node == nullptr ) {
    return ZddItem::invalid();
  }
  auto item = _level_to_item(node->level());
  return item;
}

// @brief 負のコファクターを返す．
Zdd
Zdd::root_cofactor0() const
{
  _check_valid();

  auto node = root().node();
  if ( node == nullptr ) {
    return *this;
  }
  return _zdd(node->edge0());
}

// @brief 正のコファクターを返す．
Zdd
Zdd::root_cofactor1() const
{
  _check_valid();

  auto node = root().node();
  if ( node == nullptr ) {
    return *this;
  }
  return _zdd(node->edge1());
}

// @brief 等価比較演算
bool
Zdd::operator==(
  const Zdd& right ///< [in] オペランド
) const
{
  return get() == right.get() && mRoot == right.mRoot;
}

// @brief ハッシュ値を返す．
SizeType
Zdd::hash() const
{
  _check_valid();

  return root().hash();
}

// @brief 根の枝を変更する．
void
Zdd::_change_root(
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
Zdd::display(
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
Zdd::rep_data() const
{
  auto edge = root();
  auto mgr = get();
  DdInfoMgr info_mgr({edge}, mgr);
  return info_mgr.rep_data();
}

// @brief 独自形式でバイナリダンプする．
void
Zdd::dump(
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
Zdd::size() const
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
Zdd::root() const
{
  return DdEdge{mRoot};
}

// @brief レベルに対応した要素を返す．
ZddItem
Zdd::_level_to_item(
  SizeType level
) const
{
  auto edge = get()->new_node(level, DdEdge::zero(), DdEdge::one());
  return _item(edge);
}

// @brief ZDDのリストからマネージャを取り出す．
ZddMgrImpl*
Zdd::_mgr(
  const vector<Zdd>& zdd_list ///< [in] BDDのリスト
)
{
  if ( zdd_list.empty() ) {
    return nullptr;
  }

  // sanity check
  auto zdd0 = zdd_list.front();
  zdd0._check_valid();
  for ( auto& zdd: zdd_list ) {
    zdd0._check_mgr(zdd);
  }
  return zdd0.get();
}

// @brief ZDDのリストから枝のリストに変換する．
vector<DdEdge>
Zdd::_conv_to_edgelist(
  const vector<Zdd>& zdd_list
)
{
  if ( zdd_list.empty() ) {
    return {};
  }

  vector<DdEdge> edge_list;
  edge_list.reserve(zdd_list.size());
  for ( auto& zdd: zdd_list ) {
    auto edge = zdd.root();
    edge_list.push_back(edge);
  }
  return edge_list;
}


//////////////////////////////////////////////////////////////////////
// クラス ZddItem
//////////////////////////////////////////////////////////////////////

// @brief 内容を指定したコンストラクタ
ZddItem::ZddItem(
  const ZddMgrHolder& holder,
  DdEdge root
) : Zdd(holder, root)
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
  return get()->level_to_varid(level());
}

// @brief レベルを返す．
SizeType
ZddItem::level() const
{
  _check_valid();
  auto node = root().node();
  return node->level();
}

// @brief レベルのリストに変換する．
vector<SizeType>
ZddItem::conv_to_levellist(
  const vector<ZddItem>& item_list
)
{
  vector<SizeType> level_list;
  level_list.reserve(item_list.size());
  for ( auto& item: item_list ) {
    auto level = item.level();
    level_list.push_back(level);
  }
  std::sort(level_list.begin(), level_list.end());
  return level_list;
}

END_NAMESPACE_YM_DD
