
/// @file AigHandle.cc
/// @brief AigHandle の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/AigHandle.h"
#include "ym/AigMgr.h"
#include "AigMgrImpl.h"
#include "AigNode.h"
#include "AigEdge.h"


BEGIN_NAMESPACE_YM_AIG

//////////////////////////////////////////////////////////////////////
// クラス AigHandle
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
AigHandle::AigHandle(
)
{
}

// @brief コピーコンストラクタ
AigHandle::AigHandle(
  const AigHandle& src
) : AigHandle(src, src._edge())
{
  // delegate constructor パタン
  // 下のコンストラクタを利用している．
}

// @brief 内容を指定したコンストラクタ
AigHandle::AigHandle(
  const AigMgrHolder& holder,
  AigEdge edge
) : AigMgrHolder{holder}
{
  _set_edge(edge);
}

// @brief デストラクタ
AigHandle::~AigHandle()
{
}

// @brief 定数0のハンドルを返す．
AigHandle
AigHandle::zero()
{
  return AigHandle(AigMgrHolder(), AigEdge::zero());
}

// @brief 定数1のハンドルを返す．
AigHandle
AigHandle::one()
{
  return AigHandle(AigMgrHolder(), AigEdge::one());
}

// @brief マネージャを返す．
AigMgr
AigHandle::mgr() const
{
  return AigMgr(*this);
}

// @brief マネージャのポインタを返す．
AigMgrHolder
AigHandle::mgr_holder() const
{
  return AigMgrHolder(*this);
}

// @brief 反転属性を得る．
bool
AigHandle::inv() const
{
  return _edge().inv();
}

// @brief 定数0の時 true を返す．
bool
AigHandle::is_zero() const
{
  return _edge().is_zero();
}

// @brief 定数1の時 true を返す．
bool
AigHandle::is_one() const
{
  return _edge().is_one();
}

// @brief 定数の時 true を返す．
bool
AigHandle::is_const() const
{
  return _edge().is_const();
}

// @brief 外部入力ノードへのハンドルの時 true を返す．
bool
AigHandle::is_input() const
{
  return _edge().is_input();
}

// @brief 外部入力ノードへのハンドルの時，入力番号を返す．
SizeType
AigHandle::input_id() const
{
  return _edge().input_id();
}

// @brief ANDノードへのハンドルの時，true を返す．
bool
AigHandle::is_and() const
{
  return _edge().is_and();
}

// @brief ANDノードへのハンドルの時，ファンインのハンドルを返す．
AigHandle
AigHandle::fanin(
  SizeType pos
) const
{
  return edge_to_handle(_edge().fanin(pos));
}

// @brief ANDノードへのハンドルの時，ファンイン0のハンドルを返す．
AigHandle
AigHandle::fanin0() const
{
  return edge_to_handle(_edge().fanin0());
}

// @brief ANDノードへのハンドルの時，ファンイン1のハンドルを返す．
AigHandle
AigHandle::fanin1() const
{
  return edge_to_handle(_edge().fanin1());
}

// @brief ANDグループのファンインのリストを返す．
vector<AigHandle>
AigHandle::ex_fanin_list() const
{
  auto edge_list = _edge().ex_fanin_list();
  return elist_to_hlist(edge_list);
}

// @brief 論理シミュレーションを行う．
AigBitVect
AigHandle::eval(
  const vector<AigBitVect>& input_vals
) const
{
  if ( is_zero() ) {
    return 0UL;
  }
  if ( is_one() ) {
    return ~0UL;
  }
  return get()->eval(input_vals, _edge());
}

// @brief コファクター演算
AigHandle
AigHandle::cofactor(
  const vector<AigHandle>& cube
) const
{
  if ( is_const() ) {
    return *this;
  }
  auto cedge_list = hlist_to_elist(cube);
  auto edge = get()->cofactor(cedge_list, _edge());
  return edge_to_handle(edge);
}

// @brief ユニークなインデックス値を返す．
SizeType
AigHandle::index() const
{
  return _edge().index();
}

// @brief ハッシュ値を返す．
SizeType
AigHandle::hash() const
{
  auto idx = index();
  auto v = reinterpret_cast<SizeType>(get());
  return ((v * v) >> 20) + idx;
}

// @brief このノードを根とする部分グラフのサイズを返す．
SizeType
AigHandle::size() const
{
  return get()->size({_edge()});
}

// @brief 指定された複数のノードを根とする部分グラフのサイズを返す．
SizeType
AigHandle::size(
  const std::vector<AigHandle>& root_list
)
{
  vector<AigEdge> oedge_list;
  auto mgr = hlist_to_elist(root_list, oedge_list);
  return mgr->size(oedge_list);
}

// @brief dot 形式で出力する．
void
AigHandle::gen_dot(
  ostream& s,
  const JsonValue& option
) const
{
  get()->gen_dot(s, {_edge()}, option);
}

// @brief dot 形式で出力する．
void
AigHandle::gen_dot(
  ostream& s,
  const vector<AigHandle>& root_list,
  const JsonValue& option
)
{
  vector<AigEdge> oedge_list;
  auto mgr = hlist_to_elist(root_list, oedge_list);
  return mgr->gen_dot(s, oedge_list, option);
}

// @brief 否定したハンドルを返す．
AigHandle
AigHandle::operator~() const
{
  return edge_to_handle(~_edge());
}

// @brief 反転属性との掛け算
AigHandle
AigHandle::operator*(
  bool inv
) const
{
  return edge_to_handle(_edge() * inv);
}

// @brief 同じノードを指す反転なしのハンドルを返す．
AigHandle
AigHandle::positive_handle() const
{
  return edge_to_handle(_edge().positive_edge());
}

// @brief 同じノードを指す反転ありのハンドルを返す．
AigHandle
AigHandle::negative_handle() const
{
  return edge_to_handle(_edge().negative_edge());
}

// @brief 論理積
AigHandle
AigHandle::operator&(
  const AigHandle& right
) const
{
  if ( is_zero() ) {
    return zero();
  }
  if ( is_one() ) {
    return right;
  }
  _check_mgr(right);
  auto e1 = _edge();
  auto e2 = right._edge();
  auto ans = get()->and_op(e1, e2);
  return edge_to_handle(ans);
}

// @brief 論理積付き代入
AigHandle&
AigHandle::operator&=(
  const AigHandle& right
)
{
  if ( !is_zero() ) {
    if ( is_one() ) {
      _set_edge(right._edge());
    }
    else {
      _check_mgr(right);
      auto e1 = _edge();
      auto e2 = right._edge();
      auto ans = get()->and_op(e1, e2);
      _set_edge(ans);
    }
  }
  return *this;
}

// @brief 論理和
AigHandle
AigHandle::operator|(
  const AigHandle& right
) const
{
  if ( is_zero() ) {
    return right;
  }
  if ( is_one() ) {
    return one();
  }
  _check_mgr(right);
  auto e1 = _edge();
  auto e2 = right._edge();
  auto ans = get()->or_op(e1, e2);
  return edge_to_handle(ans);
}

// @brief 論理和付き代入
AigHandle&
AigHandle::operator|=(
  const AigHandle& right
)
{
  if ( !is_one() ) {
    if ( is_zero() ) {
      _set_edge(right._edge());
    }
    else {
      _check_mgr(right);
      auto e1 = _edge();
      auto e2 = right._edge();
      auto ans = get()->or_op(e1, e2);
      _set_edge(ans);
    }
  }
  return *this;
}

// @brief 排他的論理和
AigHandle
AigHandle::operator^(
  const AigHandle& right
) const
{
  if ( is_zero() ) {
    return right;
  }
  if ( is_one() ) {
    return ~right;
  }
  _check_mgr(right);
  auto e1 = _edge();
  auto e2 = right._edge();
  auto ans = get()->xor_op(e1, e2);
  return edge_to_handle(ans);
}

// @brief 排他的論理和付き代入
AigHandle
AigHandle::operator^=(
  const AigHandle& right
)
{
  if ( is_zero() ) {
    _set_edge(right._edge());
  }
  else if ( is_one() ) {
    _set_edge(~right._edge());
  }
  else {
    _check_mgr(right);
    auto e1 = _edge();
    auto e2 = right._edge();
    auto ans = get()->xor_op(e1, e2);
    _set_edge(ans);
  }
  return *this;
}

// @brief 等価比較関数
bool
AigHandle::operator==(
  const AigHandle& right
) const
{
  if ( !AigMgrHolder::check_mgr(right) ) {
    return false;
  }
  return _edge() == right._edge();
}

// @brief 小なり比較演算
bool
AigHandle::operator<(
  const AigHandle& right
) const
{
  _check_mgr(right);
  return _edge() < right._edge();
}

// @brief 枝を返す．
AigEdge
AigHandle::_edge() const
{
  return AigEdge{mEdge};
}

// @brief 枝をセットする．
void
AigHandle::_set_edge(
  AigEdge edge
)
{
  mEdge = edge.mPackedData;
}

END_NAMESPACE_YM_AIG
