
/// @file AigEdge.cc
/// @brief AigEdge の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "aig/AigEdge.h"
#include "aig/AigNode.h"


BEGIN_NAMESPACE_YM_AIG

//////////////////////////////////////////////////////////////////////
// クラス AigEdge
//////////////////////////////////////////////////////////////////////

// @brief 外部入力ノードへの枝の時 true を返す．
bool
AigEdge::is_input() const
{
  if ( is_const() ) {
    return false;
  }
  return node()->is_input();
}

// @brief 外部入力ノードへの枝の時，入力番号を返す．
SizeType
AigEdge::input_id() const
{
  if ( is_const() ) {
    throw std::invalid_argument{"input_id() is valid if is_input() == true"};
  }
  return node()->input_id();
}

// @brief ANDノードへの枝の時，true を返す．
bool
AigEdge::is_and() const
{
  if ( is_const() ) {
    return false;
  }
  return node()->is_and();
}

// @brief ANDノードへの枝の時，ファンインの枝を返す．
AigEdge
AigEdge::fanin(
  SizeType pos
) const
{
  if ( is_const() ) {
    abort();
    throw std::invalid_argument{"fanin() is valid if is_and() == true"};
  }
  return node()->fanin(pos);
}

// @brief ANDノードへの枝の時，ファンイン0の枝を返す．
AigEdge
AigEdge::fanin0() const
{
  if ( is_const() ) {
    abort();
    throw std::invalid_argument{"fanin0() is valid if is_and() == true"};
  }
  return node()->fanin0();
}

// @brief ANDノードへの枝の時，ファンイン1の枝を返す．
AigEdge
AigEdge::fanin1() const
{
  if ( is_const() ) {
    abort();
    throw std::invalid_argument{"fanin1() is valid if is_and() == true"};
  }
  return node()->fanin1();
}

// @brief ANDグループのファンインのリストを返す．
vector<AigEdge>
AigEdge::ex_fanin_list() const
{
  if ( !is_and() ) {
    abort();
    throw std::invalid_argument{"ex_fanin_list() is valid if is_and() == true"};
  }
  vector<AigEdge> fanin_list;
  auto h0 = fanin0();
  h0.ex_fanin_sub(fanin_list);
  auto h1 = fanin1();
  h1.ex_fanin_sub(fanin_list);

  // 重複を取り除く
  sort(fanin_list.begin(), fanin_list.end());
  auto rpos = fanin_list.begin();
  auto epos = fanin_list.end();
  auto wpos = rpos;
  auto prev = AigEdge::zero();
  for ( ; rpos != epos; ++ rpos ) {
    auto cur = *rpos;
    if ( cur != prev ) {
      if ( rpos != wpos ) {
	*wpos = cur;
      }
      ++ wpos;
      prev = cur;
    }
  }
  if ( wpos != epos ) {
    fanin_list.erase(wpos, epos);
  }
  return fanin_list;
}

// @brief ex_fanin_list() の下請け関数
void
AigEdge::ex_fanin_sub(
  vector<AigEdge>& fanin_list
) const
{
  if ( !inv() && is_and() ) {
    fanin0().ex_fanin_sub(fanin_list);
    fanin1().ex_fanin_sub(fanin_list);
  }
  else {
    fanin_list.push_back(*this);
  }
}

// @brief ユニークなインデックス値を返す．
SizeType
AigEdge::index() const
{
  // 普通はノード番号から作るが定数の時だけ例外
  if ( is_const() ) {
    return mPackedData;
  }
  auto val = (node()->id() + 1) * 2;
  if ( inv() ) {
    val += 1;
  }
  return val;
}

// @brief 内容を出力する．
void
AigEdge::print(
  std::ostream& s
) const
{
  if ( is_zero() ) {
    s << "--0--";
  }
  else if ( is_one() ) {
    s << "--1--";
  }
  else {
    if ( inv() ) {
      s << "~";
    }
    s << "Node#" << node()->id();
  }
}

END_NAMESPACE_YM_AIG
